#include "state.h"

std::vector<BorisGoal> State::getLegalActions() const{
    return board.getLegalActions(currentPiece);
}

BoardModel State::applyAction(BorisGoal borisGoal, int *numLinesRemoved) const {
    return board.applyAction(borisGoal, currentPiece, numLinesRemoved);
}


