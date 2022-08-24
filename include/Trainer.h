#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "../include/Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();//will translate to function order(......
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    void setSalary(int salary);
    bool isOpen();
    void setTrainerId(int id);
    int getTrainerID();


    Trainer (Trainer &other);
    ~Trainer();
    Trainer(Trainer &&other);
    Trainer& operator= (const Trainer &other);
    Trainer& operator=(Trainer &&other);
    Customer* copy(Customer* customer);

private:
    int id;
    int salary;
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    void clear();
};


#endif