#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual ~Customer();
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    virtual std::string getType() = 0;
    virtual Customer* clone()=0;
    std::string getName() const;
    int getId() const;
private:
    const std::string name;
    const int id;
};


class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string cName, int cId);
    ~ SweatyCustomer(){};
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    std::string getType();
    Customer* clone();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string cName1, int cId1);
    ~ CheapCustomer(){};
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    std::string getType();
    Customer* clone();
private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    ~ HeavyMuscleCustomer(){};
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    std::string getType();
    Customer* clone();
private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string cName1, int cId1);
    ~ FullBodyCustomer() {};
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    std::string getType();
    Customer* clone();
private:
};


#endif