#include "state.h"

std::vector<BorisGoal> State::getLegalBorisGoals() const{
    std::vector<BorisGoal> legalActions;
    TetrixPiece piece = currentPiece;
    for(int i=0; i<4; ++i){
        //check each x position
        for(int j = -piece.minX(); j < board.getWidth() - piece.maxX(); ++j){
            legalActions.push_back(BorisGoal{i, j});
        }
        piece = piece.rotatedLeft();
    }
    return legalActions;
}

BoardModel State::applyAction(BorisGoal borisGoal, int *numLinesRemoved) const {
    TetrixPiece piece = currentPiece;
    for(int i=0; i<borisGoal.rotation; ++i){
        piece = piece.rotatedLeft();
    }
    return board.dropPiece(piece, borisGoal.position, numLinesRemoved);
}


