#include "state.h"

const std::vector<SimpleAction> State::getLegalActions() const{
    return board.getLegalActions(currentPiece);
}

const BoardModel State::applyAction(SimpleAction action, int *numLinesRemoved) const {
    return board.applyAction(action, currentPiece, numLinesRemoved);
}


