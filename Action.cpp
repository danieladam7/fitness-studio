
#include "../include/Action.h"
#include "../include/Trainer.h"
#include <string>
#include <iostream>
#include "../include/Customer.h"
#include "../include/Studio.h"

using namespace std;
extern Studio *backup;

BaseAction::BaseAction() : errorMsg(""), status(ActionStatus()) {}
BaseAction::~BaseAction() = default;

void BaseAction::complete() {
    status = COMPLETED;
}
void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

//class OpenTrainer
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id), customers(std::vector<Customer*>()){
    for(Customer* curr : customersList){
        customers.push_back(curr);
    }
} //or shuld i push Customer to the vector customers one by one?

BaseAction* OpenTrainer::clone(){
    BaseAction* copy = new OpenTrainer(trainerId, customers);
    return copy;
}
OpenTrainer::~OpenTrainer(){
    for(Customer* curr : customers){
        curr = nullptr;
        delete curr;
    }
    customers.clear();
}

//give the trainer his customers AND
void OpenTrainer::act(Studio &studio) {
    if(studio.getTrainer(trainerId) == nullptr|| studio.getTrainer(trainerId)->isOpen()){
        this->error("Workout session doesn't exist or is already open!");
        cout << getErrorMsg()<< endl;
    }
    else{
        studio.getTrainer(trainerId)->openTrainer();
        for (Customer* curr : customers) {
            studio.getTrainer(trainerId)->addCustomer(curr);
        }
        this->complete();
    }
}

std::string OpenTrainer::toString() const {
    std::string output;
    output.append("open ");
    output.append(std::to_string(trainerId));
    output.append(" ");
    std::string customersAndTypes;
    for (Customer* curr : customers){
        if (curr == customers.at(customers.size()-1)) {
            customersAndTypes = customersAndTypes + curr->getName() + "," + curr->getType();
        }
        else
            customersAndTypes = customersAndTypes + curr->getName() + "," + curr->getType() + " ";
    }
    output.append(customersAndTypes);
    output.append(" ");
    if(getStatus() == COMPLETED)
        output.append("COMPLETED");
    else {
        output.append("Error: ");
        output.append(getErrorMsg());
    }
    return output; //example: "open 2 Shalom,swt Dan,chp Alice,mcl.......
}

//class Order
Order::Order(int id) :  trainerId(id) {
}
BaseAction* Order::clone(){
    BaseAction* copy = new Order(trainerId);
    return copy;
}
//starts a session, each customer will choose what workouts he is gonna' do according to his strategy
void Order::act(Studio &studio) { //prints the toString() methods
    Trainer* trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || trainerId < 0 || trainerId >= studio.getNumOfTrainers() || !trainer->isOpen()) {
        this->error("Can't order because trainer doesn't exist or is closed!");
        cout << getErrorMsg() <<endl;
    }
    else {
        std::vector<Customer *> customers = studio.getTrainer(trainerId)->getCustomers();
        //get customers orders.

        for (Customer *customer: customers) {
            std::vector<int> customerOrders = customer->order(studio.getWorkoutOptions());
            for (int currentWorkout: customerOrders) {
                std::cout << customer->getName() << ' ' << "Is Doing "
                          << studio.getWorkoutOptions().at(currentWorkout).getName() << endl;
            }
            trainer->order(customer->getId(), customerOrders, studio.getWorkoutOptions());
        }
        this->complete();
    }
}

std::string Order::toString() const { //see example in page 5
    std::string output;
    output.append("order ");
    output.append(to_string(trainerId));
    output.append(" ");
    if (getStatus() == COMPLETED){
        output.append("COMPLETED");
    }
    else {
        output.append(getErrorMsg());
        output.append("Error: ");
    }

    return output;
}

//class MoveCustomer
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {}
BaseAction* MoveCustomer::clone(){
    BaseAction* copy = new MoveCustomer(srcTrainer, dstTrainer, id);
    return copy;
}
void MoveCustomer::act(Studio &studio) {
    if(studio.getTrainer(srcTrainer) == nullptr || studio.getTrainer(srcTrainer)->getCustomer(id) == nullptr || studio.getTrainer(dstTrainer)== nullptr) {
        this->error("Cannot move customer");
        cout << getErrorMsg() << endl;
        return;
    }
    else if(!studio.getTrainer(srcTrainer)->isOpen() || !studio.getTrainer(dstTrainer)->isOpen() || (int)studio.getTrainer(dstTrainer)->getCustomers().size()>=(int)studio.getTrainer(dstTrainer)->getCapacity()) {
        this->error("Cannot move customer");
        cout << getErrorMsg() << endl;
        return;
    }
    else {
        studio.getTrainer(dstTrainer)->addCustomer(studio.getTrainer(srcTrainer)->getCustomer(id));
        std::vector<std::pair<int, Workout>> orderPairs = studio.getTrainer(srcTrainer)->getOrders();
        //for(OrderPair curr : studio.getTrainer(srcTrainer)->getOrders()){
        for (OrderPair curr: orderPairs) {
            if (curr.first == id) {
                studio.getTrainer(dstTrainer)->getOrders().push_back(curr);
                studio.getTrainer(dstTrainer)->setSalary(
                        studio.getTrainer(dstTrainer)->getSalary() + curr.second.getPrice());
                studio.getTrainer(srcTrainer)->setSalary(
                        studio.getTrainer(srcTrainer)->getSalary() - curr.second.getPrice());
            }
        }
        studio.getTrainer(srcTrainer)->removeCustomer(id);
        if (studio.getTrainer(srcTrainer)->getCustomers().empty()) {
            BaseAction *baseAction = new Close(srcTrainer);
            studio.setActionsLog(baseAction);
            baseAction->act(studio);
        }
        this->complete();
    }
}
std::string MoveCustomer::toString() const {
    std::string output;
    output.append("move ");
    output.append(to_string(srcTrainer));
    output.append(" ");
    output.append(to_string(dstTrainer));
    output.append(" ");
    output.append(to_string(id));
    output.append(" ");
    if (getStatus() == COMPLETED){
        output.append(" COMPLETED");
    }
    else {
        output.append(getErrorMsg());
        output.append("Error: ");
    }
    return  output;
}

// class Close
Close::Close(int id) : trainerId(id){}
BaseAction* Close::clone(){
    BaseAction* copy = new Close(trainerId);
    return copy;
}
void Close::act(Studio &studio) {
    if (!studio.getTrainer(trainerId)->isOpen() || studio.getTrainer(trainerId) == nullptr) {
        this->error("Trainer does not exist or is not open!");
        std::cout<< getErrorMsg() << endl;
    }
    else {
        cout << "Trainer " << trainerId << " closed. Salary " << studio.getTrainer(trainerId)->getSalary() << " NIS.";
        studio.getTrainer(trainerId)->closeTrainer();
        this->complete();
    }
}
std::string Close::toString() const {
    std::string output;
    output.append("close ");
    output.append(to_string(trainerId));
    if (getStatus() == COMPLETED){
        output.append(" COMPLETED");
    }
    else {
        output.append("Error: ");
        output.append(getErrorMsg());
    }
    return output;
}


//class Close All
CloseAll::CloseAll() {
}
BaseAction* CloseAll::clone(){
    BaseAction* copy = new CloseAll();
    return copy;
}

void CloseAll::act(Studio &studio) {
    if(!studio.getStudioStatus()) {
        this->complete();
        return;
    }
    else{
        for(Trainer* curr : studio.getTrainers()){
            if(curr->isOpen())
                curr->closeTrainer();
        }
        studio.setStudioStatus(false);
        this->complete();
    }
}
std::string CloseAll::toString() const {
    std::string output;
    output.append("closeall");
    if (getStatus() == COMPLETED){
        output.append(" COMPLETED");
    }
    else {
        output.append(getErrorMsg());
        output.append("Error: ");
    }
    return output;
}

//class PrintWorkoutOptions
PrintWorkoutOptions ::PrintWorkoutOptions() {
}
BaseAction* PrintWorkoutOptions::clone(){
    BaseAction* copy = new PrintWorkoutOptions();
    return copy;
}
void PrintWorkoutOptions::act(Studio &studio) {
    for(Workout curr : studio.getWorkoutOptions())
        cout << curr.getName() << ", " << curr.getType() << ", " << curr.getPrice() <<endl;
    this->complete();
}

std::string PrintWorkoutOptions::toString() const {
    std::string output;
    output.append("workout_options");
    if (getStatus() == COMPLETED){
        output.append(" COMPLETED");
    }
    else {
        output.append(getErrorMsg());
        output.append("Error: ");
    }
    return output;
}


PrintTrainerStatus ::PrintTrainerStatus(int id) : trainerId(id) {}
BaseAction* PrintTrainerStatus::clone(){
    BaseAction* copy = new PrintTrainerStatus(trainerId);
    return copy;
}
void PrintTrainerStatus::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);
    if(trainer->isOpen()) {
        cout << "Trainer " << trainerId << " status: " << "open" << endl;
        cout << "Customers:" << endl;
        for(Customer* currCust : trainer->getCustomers())
            cout << currCust->getId() << " " << currCust->getName() << endl;
        cout << "Orders:" << endl;
        for(OrderPair currPair : trainer->getOrders())
            cout << currPair.second.getName() << ", " << currPair.second.getPrice() << "NIS, " << currPair.first <<endl;

        cout << "Current Trainer's Salary: " <<  trainer->getSalary() <<endl;
        this->complete();
    }
    else
        cout << "Trainer " << trainerId << " status:" << " closed" <<endl;


}
std::string PrintTrainerStatus::toString() const {
    std::string output;
    output.append("status ");
    output.append(to_string(trainerId));
    if (getStatus() == COMPLETED){
        output.append(" COMPLETED");
    }
    else {
        output.append(getErrorMsg());
        output.append("Error: ");
    }
    return output;
}

PrintActionsLog ::PrintActionsLog() {}
BaseAction* PrintActionsLog::clone(){
    BaseAction* copy = new PrintActionsLog();
    return copy;
}
void PrintActionsLog::act(Studio &studio) {
    for(BaseAction* curr : studio.getActionsLog()){
        cout << curr->toString() << endl;
    }
}
std::string PrintActionsLog::toString() const {
    return "";
}

BackupStudio ::BackupStudio() {
}

BaseAction* BackupStudio::clone() {
    BaseAction *copy = new BackupStudio();
    return copy;
}

void BackupStudio::act(Studio &studio) {
    delete backup;
    backup = new Studio(studio);
    this->complete();
}
std::string BackupStudio::toString() const {
    std::string output;
    output.append("backup");
    if (getStatus() == COMPLETED){
        output.append(" COMPLETED");
    }
    else {
        output.append(getErrorMsg());
        output.append("Error: ");
    }
    return output;
}


RestoreStudio::RestoreStudio() {}
BaseAction* RestoreStudio::clone() {
    BaseAction *copy = new RestoreStudio();
    return copy;
}

void RestoreStudio::act(Studio &studio) {
    if (backup == nullptr) {
        this->error("No backup available!");
        cout << getErrorMsg()<< endl;
    }
    else{
        studio = *backup;
        this->complete();
    }

}
    std::string RestoreStudio::toString() const {
    std::string output;
        output.append("restore ");
        if (getStatus() == COMPLETED){
            output.append(" COMPLETED");
        }
        else {
            output.append(getErrorMsg());
            output.append("Error: ");
        }
        return output;
    }

