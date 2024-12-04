//
// Created by dyota on 14/11/2021.
//

#include "../include/Studio.h"
#include "string"
#include "sstream"
#include "fstream"

using namespace std;

Studio::Studio(const std::string &configFilePath): open(false), nextId(0), trainers(std::vector<Trainer*>()),
                                                   workout_options(std::vector<Workout>()), actionsLog(std::vector<BaseAction*>()) {
    std::ifstream myFileStream(configFilePath);

    //READING INPUT:
    std::vector<string> fileStrings = std::vector<string>();
    std::string LINE;
    while (!myFileStream.eof()) {
        getline(myFileStream, LINE);
        if ( LINE[0] != '#' ) //&& LINE.length() == 0
            fileStrings.push_back(LINE);
    }

    string FirstLine = fileStrings.at(0);

    int numberOfTrainers = (FirstLine[0]-'0'); //*********

    std::vector<int> trainersCapcities = std::vector<int>();     //*************
    int TrainerId_indexer = 0;
    std::string capacitiesLine = fileStrings.at(2);
    size_t i = 0;
    int startIndex = 0;
    while (i < capacitiesLine.size()) {
        if (capacitiesLine[i] == ',') {
            trainersCapcities.push_back(std::stoi(capacitiesLine.substr(startIndex, i - startIndex)));
            startIndex = i + 1;
        }
        i++;
    }
    for (size_t i = 0 ; i < trainersCapcities.size() ; i++){
    }
    trainersCapcities.push_back(stoi(capacitiesLine.substr(startIndex, i - startIndex)));

    //creating trainers instances
    for (int j = 0; j < numberOfTrainers; j++) //making the trainers
    {
        Trainer *trainer = new Trainer(trainersCapcities[j]);
        trainer->setTrainerId(TrainerId_indexer);
        TrainerId_indexer++;
        trainers.push_back(trainer);
    }

    std::string workout_option_Line; //"Yoga, Anaerobic, 90"; //some workout option from input, workout_options
    int workout_option_indexer = 0;

    for (size_t j = 4; j < fileStrings.size(); j++) {
        workout_option_Line = fileStrings.at(j);

        std::string name;
        std::string type;
        std::string price;
        std::vector<std::string> option;

        i = 0;
        startIndex = 0;
        while (i < workout_option_Line.size()) {
            if (workout_option_Line[i] == ',') {
                option.push_back((workout_option_Line.substr(startIndex, i - startIndex)));
                startIndex = i + 2;
            }
            i++;
        }
        option.push_back(workout_option_Line.substr(startIndex, i - startIndex));

        name = option[0];
        type = option[1];
        price = option[2];
        int price1 = std::stoi(price);
        WorkoutType workoutType;
        if (type == "Anaerobic")
            workoutType = ANAEROBIC;
        if (type == "Mixed")
            workoutType = MIXED;
        if (type == "Cardio")
            workoutType = CARDIO;
        workout_options.push_back(Workout(workout_option_indexer, name, price1, workoutType));
        workout_option_indexer++;

        actionsLog = std::vector<BaseAction*>();

    }
}
////            workout_options(std::vector<Workout>()), actionsLog(std::vector<BaseAction*>()) {
// Destructor
Studio :: ~Studio() {
    for(Trainer* curr : trainers){
        curr = nullptr;
        delete curr;
    }
    for(BaseAction* curr : actionsLog){
        curr = nullptr;
        delete curr;
    }
    trainers.clear();
    workout_options.clear();
    actionsLog.clear();
}

// Copy constructor
Studio:: Studio(Studio &other) : open(other.open), nextId(other.nextId), trainers(std::vector<Trainer*>()),
                                 workout_options(std::vector<Workout>()), actionsLog(std::vector<BaseAction*>()){
    for(Trainer* curr : other.trainers){
        Trainer* newOne = new Trainer(*curr);
        trainers.push_back(newOne);
    }

    for(Workout curr : other.workout_options) {
        workout_options.push_back(curr);
    }

        for(BaseAction* curr : other.actionsLog){
        BaseAction* newOne = curr->clone();
        actionsLog.push_back(newOne);
    }
}

// move constructor
Studio ::Studio(Studio &&other) : open(other.open), nextId(other.nextId), trainers(std::vector<Trainer*>()),
                                  workout_options(std::vector<Workout>()), actionsLog(std::vector<BaseAction*>()){
    for(Trainer* curr : other.trainers)
        trainers.push_back(curr);
    delete &other.trainers;

    for(Workout curr : other.workout_options)
        workout_options.push_back(curr);
    delete &other.workout_options;

    for(BaseAction* curr : other.actionsLog)
        actionsLog.push_back(curr);
    delete &other.actionsLog;
}

// Copy assignment
Studio &Studio::operator=(const Studio &other) {
    if(this != &other){
        open = other.open;
        nextId = other.nextId;
        trainers.clear();
        for(Trainer* curr : other.trainers )
            trainers.push_back(curr);

        workout_options.clear();
        for(Workout curr : other.workout_options)
            workout_options.push_back(curr);

        actionsLog.clear();
        for(BaseAction* curr : other.actionsLog)
            actionsLog.push_back(curr);
    }
    return *this;
}

// move assignment
Studio &Studio:: operator= (Studio &&other){
    if(this != &other){
        open = other.open;
        nextId = other.nextId;
        trainers.clear();
        for(Trainer* curr : other.trainers )
            trainers.push_back(curr);
        delete &other.trainers;

        workout_options.clear();
        for(Workout curr : other.workout_options)
            workout_options.push_back(curr);
        delete &other.workout_options;

        actionsLog.clear();
        for(BaseAction* curr : other.actionsLog)
            actionsLog.push_back(curr);
        delete &other.actionsLog;
    }
    return *this;
}



void Studio::start()
{
    open = true;
    std::cout << "Studio C++ is now open!!!" << std::endl;

    while(open){
        char command[500];
        std::cin.getline(command, 500);
        BaseAction* baseAction = buildAction(command);
        actionsLog.push_back(baseAction);
        baseAction->act(*this);

    }

}

BaseAction* Studio::buildAction(char* command) {
    std::string user_command = command;
    std::string command_type;
    size_t index = 0;
    while (index < user_command.size()) {
        if (user_command.at(index) == ' ') { //|| user_command.at(index) ==  '_'
            command_type = user_command.substr(0, index);
            index++;
            break;
        }
        if (index == user_command.size() - 1) {
            command_type = user_command;
            break;
        }
        index++;
    }

    if (command_type == "open") {
        int trainerId;
        size_t i = index;
        while (i < user_command.size()) {
            if (user_command.at(i) == ' ') {
                trainerId = stoi(user_command.substr(index, i - index));
                i++;
                index = i;
                break;
            }
            i++;
        }

        std::vector<std::pair<std::string, std::string>> customersInfo;

        size_t j = index;
        size_t startIndex = j;
        std::vector<std::string> names;
        std::vector<std::string> cTypes;
        while (j < user_command.size()) {
            if (user_command.at(j) == ',') {
                names.push_back(user_command.substr(startIndex, j - startIndex));
                startIndex = j + 1;
            }
            if (user_command.at(j) == ' ') {
                cTypes.push_back(user_command.substr(startIndex, j - startIndex));
                startIndex = j + 1;
            }
            j++;
        }
        cTypes.push_back((user_command.substr(startIndex, j - startIndex)));

        for (size_t z = 0; z < cTypes.size(); z++) {
            customersInfo.push_back(std::make_pair(names.at(z), cTypes.at(z)));
        }


        vector<Customer *> customers = vector<Customer*>();
        for (std::pair<std::string, std::string> pair: customersInfo) {
            customers.push_back(makeCustomer(pair));
        }

        OpenTrainer *opentrainer = new OpenTrainer(trainerId, customers); //types
        return opentrainer;
    }


    if (command_type == "order") {
        int trainerId;

        size_t i = index-1;
        while (i < user_command.size()) {
            if (user_command.at(i) == ' ') {
                trainerId = user_command.at(i+1) - '0';
                i++;
                index = i;
                break;
            }
            i++;
        }

        Order* order = new Order(trainerId);
        return order;
    }

    if (command_type == "move") {
        std::vector<int> move_info;
        int origin;
        int dest;
        int customerId;
        size_t j = index;
        size_t startIndex = j;
        while (j < user_command.size()) {
            if (user_command.at(j) == ' ') {
                cout << user_command.substr(startIndex, j - startIndex) << endl;
                move_info.push_back(stoi(user_command.substr(startIndex, j - startIndex)));
                startIndex = j + 1;
            }
            j++;
        }
        cout << user_command.substr(startIndex, j - startIndex) << endl;
        move_info.push_back(stoi(user_command.substr(startIndex, j - startIndex)));

        origin = move_info.at(0);
        dest = move_info.at(1);
        customerId = move_info.at(2);
        MoveCustomer* move =  new MoveCustomer(origin, dest, customerId);
        //were we will delete move?
        return move;
    }

    if (command_type == "close") {
        size_t trainerId;
        size_t i = index-1;
        while (i < user_command.size()) {
            if (user_command.at(i) == ' ') {
                trainerId = stoi(user_command.substr(index, i - index));
                i++;
                index = i;
                break;
            }
            i++;
        }

        Close* close =  new Close(trainerId);
        //where *close should be deleted?
        return close;
    }

    if (command_type == "closeall")
    {
        CloseAll* closeall =  new CloseAll();
        return closeall;
    }

    if (command_type == "workout_options")
    {
        PrintWorkoutOptions* printWorkoutOptions =  new PrintWorkoutOptions();
        return printWorkoutOptions;
    }

    if (command_type == "status")
    {
        size_t trainerId;
        size_t i = index-1;
        while (i < user_command.size()) {
            if (user_command.at(i) == ' ') {
                trainerId = stoi(user_command.substr(index, i - index));
                i++;
                index = i;
                break;
            }
            i++;
        }
        PrintTrainerStatus* printTrainerStatus =  new PrintTrainerStatus(trainerId);
        return printTrainerStatus;
    }

    if (command_type == "log")
    {
        PrintActionsLog* printActionsLog =  new PrintActionsLog();
        return printActionsLog;
    }

    if (command_type == "backup")
    {
        BackupStudio* backupStudio =  new BackupStudio();
        return backupStudio;
    }

    if (command_type == "restore")
    {
        RestoreStudio* restoreStudio =  new RestoreStudio();
        return restoreStudio;
    }
    return nullptr;
}


Customer *Studio::makeCustomer(std::pair<std::string, std::string> pair) {
    string type = pair.second;
    int id = getNewCustomerId();
    if (type == "swt")
        return new SweatyCustomer(pair.first,id);
    if (type == "chp"){
        return new CheapCustomer(pair.first, id);
    }
    if (type == "mcl"){
        return new HeavyMuscleCustomer(pair.first, id);
    }
    if (type == "fbd"){
        return new FullBodyCustomer(pair.first, id);
    }
    return nullptr;
}


int Studio::getNumOfTrainers() const {
    return trainers.size(); //or capacity()?
}

Trainer* Studio::getTrainer(int tid) {
    return trainers.at(tid);
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

void Studio::setActionsLog(BaseAction* baseAction){
    actionsLog.push_back(baseAction);
}

std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;   //this return returns a referance to workout_options vector?
}

std::vector<Trainer*>& Studio::getTrainers() {
    return trainers;
}
int Studio::getNewCustomerId() {
    int id = nextId;
    nextId++;
    return id;
}




bool Studio:: getStudioStatus(){
    return open;
}
void Studio::setStudioStatus(bool change){
    open = change;
}