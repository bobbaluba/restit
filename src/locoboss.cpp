#include "locoboss.h"

#include <cstdlib>

LocoBoss::LocoBoss(int boardWidth): boardWidth(boardWidth){
}

BorisGoal LocoBoss::getGoal(const State &/*currentState*/){
    int rotation = rand() % 4;
    int position = rand() % boardWidth;
    return BorisGoal{rotation, position};
}
