#ifndef STATE_H
#define STATE_H

#include "simpleaction.h"
#include "gamemodel.h"

#include <vector>

class State{
private:
    GameModel gameModel;
public:
    explicit State(const BoardModel& board, const TetrixPiece& currentPiece, const TetrixPiece& nextPiece):
        gameModel(board,currentPiece, nextPiece)
    {}
    const std::vector<SimpleAction> getLegalActions() const { return gameModel.getBoard().getLegalActions(gameModel.getCurrentPiece()); }
    const BoardModel applyAction(const SimpleAction& action, int *numLinesRemoved) const { return gameModel.getBoard().applyAction(action, gameModel.getCurrentPiece(), numLinesRemoved); }
    const TetrixPiece& getCurrentPiece() const { return gameModel.getCurrentPiece(); }
    const TetrixPiece& getNextPiece() const { return gameModel.getNextPiece(); }
};

#endif // STATE_H
