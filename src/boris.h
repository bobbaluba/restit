#ifndef BORIS_H
#define BORIS_H

#include <cstdlib>
#include <vector>

class Boris{
public:
    enum Action {
        MOVE_LEFT, MOVE_RIGHT, ROTATE_CCW, DROP
    };
private:
    std::vector<Boris::Action> plan;
    void getNextGoal();
    int goalPosition;
    int goalRotation;
public:
    Boris();
    Boris::Action getNextAction();
};

#endif // BORIS_H
