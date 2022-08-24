
# include "../include/Workout.h"

// no references here so don't need to use rule of 5


Workout:: Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type) : id(w_id), name(w_name), price(w_price), type(w_type) {
}


int Workout::getId() const {
    return id;
}
std::string Workout::getName() const {
    return name;
}
int Workout::getPrice() const {
    return price;
}
WorkoutType Workout::getType() const {
    return type;
}

//Workout::Workout(Workout &other) : id(other.id), name(other.name), price(other.price), type(other.type) {}


