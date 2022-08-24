#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "../include/Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    // rule of 5
    ~Studio();
    Studio(Studio &other);
    Studio(Studio &&other);

    Studio(const Studio &&other);
    Studio& operator= (const Studio &other);
    Studio& operator= (Studio &&other);    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    std::vector<Trainer*>& getTrainers();
    void setActionsLog(BaseAction* baseAction);
    bool getStudioStatus();
    void setStudioStatus(bool change);



private:
    bool open;
    int nextId;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int getNewCustomerId();
    Customer* makeCustomer(std::pair<std::string, std::string>);
    BaseAction* buildAction(char* command);
};

#endif