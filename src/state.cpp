#include "state.h"

std::vector<SimpleAction> State::getLegalActions() const{
    return board.getLegalActions(currentPiece);
}

BoardModel State::applyAction(SimpleAction action, int *numLinesRemoved) const {
    return board.applyAction(action, currentPiece, numLinesRemoved);
}


