#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include "Trainer.h"

enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction{
public:
    BaseAction();
    virtual ~BaseAction();
    virtual BaseAction* clone()=0;
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    ~OpenTrainer();
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
   // std::string args;
	const int trainerId;
	std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    ~Order() override = default ;
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    ~MoveCustomer() override = default;
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    ~Close() override = default;
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    ~CloseAll() override = default;
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    ~PrintWorkoutOptions() override = default;
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    ~PrintTrainerStatus() override = default;
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    ~PrintActionsLog() override = default;
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    ~BackupStudio() override = default;
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    ~RestoreStudio() override = default;
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
};

#endif