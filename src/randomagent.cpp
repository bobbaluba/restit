#include "randomagent.h"

#include <cstdlib>

RandomAgent::RandomAgent(int boardWidth): boardWidth(boardWidth){
}

const SimpleAction RandomAgent::getGoal(const State &/*currentState*/){
    int rotation = rand() % 4;
    int position = rand() % boardWidth;
    return SimpleAction{rotation, position};
}
