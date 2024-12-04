#include <iostream>
#include "../include/Customer.h"
#include "algorithm"
#include "../include/Workout.h"
#include "../include/Action.h"

Customer ::Customer(std::string c_name, int c_id) : name(c_name), id(c_id)  {

}
Customer::~Customer(){}
std::string Customer::getName() const {
    return name;
}
int Customer::getId() const {
    return id;
}

SweatyCustomer::SweatyCustomer(std::string cName, int cId) : Customer(cName, cId) {
}

Customer *SweatyCustomer::clone() {
    Customer* newOne;
    newOne = new SweatyCustomer(this->getName(), this->getId());
    return newOne;
}

std::vector<int> SweatyCustomer::order(const std::vector <Workout> &workout_options) {
    std::cout << "swt orders workouts:" << std::endl;
    std::vector<int> out = std::vector<int>();
    for(Workout curr : workout_options){
        if(curr.getType() == CARDIO)
            out.push_back(curr.getId());
    }
    return out;
}

std::string SweatyCustomer::toString() const {
    std::string out;
    out.append(getName());
    out.append(" ");
    out.append(std::to_string(getId()));
    return out;
}

std::string SweatyCustomer::getType() {
    return "swt";
}


CheapCustomer::CheapCustomer(std::string cName1, int cId1)
        : Customer(cName1, cId1) {
}

Customer *CheapCustomer::clone() {
    Customer* newOne;
    newOne = new CheapCustomer(this->getName(), this->getId());
    return newOne;
}

std::vector<int> CheapCustomer::order(const std::vector <Workout> &workout_options) {
    std::vector<int> out = std::vector<int>();
    int minWorkoutIndex = 0;
    int min_price = workout_options.at(0).getPrice();
    int currID = 0;
    for(Workout curr : workout_options){
        currID++;
        if(curr.getPrice() < min_price || (curr.getPrice() == min_price && curr.getId() < min_price)){
            minWorkoutIndex = currID; //curr.getId(), curr.getName(), curr.getPrice(),curr.getType()
        }
    }
    out.push_back(workout_options.at(minWorkoutIndex).getId());
    return out;
}

std::string CheapCustomer::toString() const {
    std::string out;
    out.append(getName());
    out.append(" ");
    out.append(std::to_string(getId()));
    return out;
}

std::string CheapCustomer::getType() {
    return "chp";
}



HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {

}

Customer *HeavyMuscleCustomer::clone() {
    Customer* newOne;
    newOne = new HeavyMuscleCustomer(this->getName(), this->getId());
    return newOne;
}

std::vector<int> HeavyMuscleCustomer::order(const std::vector <Workout> &workout_options) {
    std::vector<std::pair<int, int>> customerWorkouts;// = std::vector<Workout>();
    for (unsigned int i = 0 ; i < workout_options.size();i++){
        if (workout_options.at(i).getType() == 0)
            customerWorkouts.emplace_back(workout_options.at(i).getId(), workout_options.at(i).getPrice());
    }
    std::sort(customerWorkouts.begin(), customerWorkouts.end(), [](std::pair<int,int> a, std::pair<int,int> b) {
        return a.second >= b.second;});

    std::vector<int> out; //= std::vector<int>();
    for (unsigned int i = 0; i< customerWorkouts.size();i++){
        out.push_back(customerWorkouts.at(i).first);
    }
    return out;
}


std::string HeavyMuscleCustomer::toString() const {
    std::string out;
    out.append(getName());
    out.append(" ");
    out.append(std::to_string(getId()));
    return out;
}

std::string HeavyMuscleCustomer::getType() {
    return "mcl";
}



FullBodyCustomer::FullBodyCustomer(std::string cName1, int cId1) : Customer(cName1, cId1) {
}

Customer *FullBodyCustomer::clone() {
    Customer* newOne;
    newOne = new FullBodyCustomer(this->getName(), this->getId());
    return newOne;
}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> out = std::vector<int>();

    std::vector<std::pair<int, int>> cardioWorkouts = std::vector<std::pair<int, int>>();
    std::vector<std::pair<int, int>> mixWorkouts = std::vector<std::pair<int, int>>();
    std::vector<std::pair<int, int>> anaerobicWorkouts = std::vector<std::pair<int, int>>();


    for (unsigned int i = 0; i < workout_options.size(); i++) {
        if (workout_options.at(i).getType() == CARDIO)
            cardioWorkouts.emplace_back(workout_options.at(i).getId(), workout_options.at(i).getPrice());
        else if (workout_options.at(i).getType() == MIXED)
            mixWorkouts.emplace_back(workout_options.at(i).getId(), workout_options.at(i).getPrice());
        else if (workout_options.at(i).getType() == ANAEROBIC)
            anaerobicWorkouts.emplace_back(workout_options.at(i).getId(), workout_options.at(i).getPrice());
    }
    //don't order at all because can complete the order
    if (cardioWorkouts.empty() | mixWorkouts.empty() | anaerobicWorkouts.empty()) {
        return std::vector<int>();
    } else {
        std::sort(cardioWorkouts.begin(), cardioWorkouts.end(), [](std::pair<int, int> a, std::pair<int, int> b) {
            return a.second < b.second;
        });
        std::sort(mixWorkouts.begin(), mixWorkouts.end(), [](std::pair<int, int> a, std::pair<int, int> b) {
            return a.second > b.second;
        });
        std::sort(anaerobicWorkouts.begin(), anaerobicWorkouts.end(),
                  [](std::pair<int, int> a, std::pair<int, int> b) {
                      return a.second < b.second;
                  });

        out.push_back(cardioWorkouts[0].first); //if aint' such?
        out.push_back(mixWorkouts[0].first);
        out.push_back(anaerobicWorkouts[0].first);
    }
    return out;
}

std::string FullBodyCustomer::toString() const {
    std::string out;
    out.append(getName());
    out.append(" ");
    out.append(std::to_string(getId()));
    return out;
}

std::string FullBodyCustomer::getType() {
    return "fbd";
}



