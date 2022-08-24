
#include <iostream>
#include "../include/Trainer.h"
#include "algorithm"
#include "../include/Workout.h"

//constructor
Trainer::Trainer(int t_capacity) : id(0), salary(0),capacity(t_capacity),open(false),
                                   customersList(std::vector<Customer*>()), orderList(std::vector<OrderPair>()) {}

//copy constructor
Trainer::Trainer (Trainer &other) : id(0), salary(other.salary),capacity(other.capacity),open(other.open),  customersList(std::vector<Customer*>()), orderList(std::vector<OrderPair>()){
    for(Customer* curr : other.customersList) {
        customersList.push_back(curr->clone());
    }
    for (OrderPair curr : orderList)
        orderList.push_back(curr);
}

//destructor
Trainer ::~Trainer() {
    for (Customer* curr: customersList) {
        curr = nullptr;
        delete curr;
    }
    customersList.clear();
    orderList.clear();
}

//move constructor
Trainer::Trainer(Trainer &&other) : id(other.id), salary(other.salary), capacity(other.capacity), open(other.open),
                                    customersList(other.customersList), orderList(other.orderList){
    for(Customer* curr : other.customersList){
        curr = nullptr;
        delete curr;
    }
    other.customersList.clear();
    delete &other.customersList;
    other.orderList.clear();
    delete &other.orderList ;
}

//Copy assignment
Trainer &Trainer::operator=(const Trainer &other) {
    if(this != &other) {
        customersList.clear();
        for(Customer* curr : other.customersList){
            customersList.push_back(curr);
        }
        orderList.clear();
        for(OrderPair curr : other.orderList){
            orderList.push_back(curr);
        }
        id = other.id;
        salary = other.salary;
        open = other.open;
    }
    return *this;
}

//move assignment
Trainer &Trainer:: operator=(Trainer &&other)
{
    if(this != &other) {
        delete this;
//      customersList.clear();
        for(Customer* curr : other.customersList){
            customersList.push_back(curr);
            curr = nullptr;
            delete curr;
        }
        other.customersList.clear();
        delete &other.customersList;
        for(OrderPair curr : other.orderList){
            orderList.push_back(curr);
        }
        other.orderList.clear();
        delete &other.orderList;
        id  = other.id;
        salary = other.salary;
        open = other.open;
    }
    return *this;
}

int Trainer::getCapacity() const{
    return capacity;
}

void Trainer::addCustomer(Customer* customer) {
    if ((int)customersList.size() >= (int)capacity)
        perror("Capacity of trainer is already full or current session is already open");
    else
        customersList.push_back(customer);
}

void Trainer::removeCustomer(int id) {
    std::vector<Customer *> temp_customerList = std::vector<Customer *>();
    for (Customer *curr: customersList) {
        if (curr->getId() != id)
            temp_customerList.push_back(curr);
    }
    customersList.clear();
    for (Customer *curr: temp_customerList)
        customersList.push_back(curr);


    std::vector<OrderPair> temp_orderList;
    for (OrderPair curr: orderList) {
        if (curr.first != id)
            temp_orderList.push_back(curr);
    }
    orderList.clear();
    for (OrderPair curr: temp_orderList)
        orderList.push_back(curr);

}

Customer* Trainer::getCustomer(int id) {
    for(Customer* curr : customersList){
        if(curr->getId() == id){
            return curr;
        }
    }
    return nullptr;
}

std::vector<Customer*>& Trainer:: getCustomers(){
    return customersList;
}
std::vector<OrderPair>& Trainer:: getOrders() {
    return orderList;
}


int Trainer::getSalary() {
    return salary;
}
void Trainer :: setSalary(int salary){
    this->salary = salary;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids,
                    const std:: vector <Workout> &workout_options) {
    int count = 0;
    for (int workout_id : workout_ids){
        for (Workout curr: workout_options) {
            if (curr.getId() == workout_id){
                OrderPair newPair = OrderPair (customer_id,curr);
                orderList.push_back(newPair);
                count++;
                salary = salary + newPair.second.getPrice();
            }
        }
    }
}

void Trainer::openTrainer() {
    open = true;
}
void Trainer::closeTrainer() {
    open = false;
    orderList.clear();
    for(Customer* curr : customersList){
        delete curr;
    }
}

bool Trainer::isOpen(){
    return open;
}

void Trainer::setTrainerId(int id) {
        this->id = id;
}

int Trainer::getTrainerID() {
    return id;
}
