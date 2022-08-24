
all: compile link
# clean compile link

link: bin/main.o bin/Studio.o bin/Trainer.o bin/Action.o  bin/Customer.o bin/Workout.o
	@echo 'Linking'
	g++ -o bin/studio bin/main.o bin/Studio.o bin/Trainer.o bin/Action.o  bin/Customer.o bin/Workout.o


compile: src/Action.cpp src/Customer.cpp src/main.cpp src/Studio.cpp src/Trainer.cpp src/Workout.cpp

	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Studio.o src/Studio.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Trainer.o src/Trainer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Workout.o src/Workout.cpp

clean:
	@echo 'Cleaning'
	rm -f bin/*