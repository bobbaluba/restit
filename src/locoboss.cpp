#include "locoboss.h"

#include <cstdlib>

LocoBoss::LocoBoss(int boardWidth): boardWidth(boardWidth), t(0){
}

const SimpleAction LocoBoss::getGoal(const State &/*currentState*/){
    int rotation = rand() % 4;
    int position = rand() % boardWidth;
    return SimpleAction{rotation, position};
}
