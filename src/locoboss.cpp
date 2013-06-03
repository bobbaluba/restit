#include "locoboss.h"

#include <cstdlib>

LocoBoss::LocoBoss(int boardWidth): boardWidth(boardWidth){
}

const SimpleAction LocoBoss::getGoal(const State &/*currentState*/){
    int rotation = rand() % 4;
    int position = rand() % boardWidth;
    return SimpleAction{rotation, position};
}
