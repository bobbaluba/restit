#ifndef LOCOBOSS_H
#define LOCOBOSS_H

//boris implements the strategy pattern, and this is his strategy

#include "bossofboris.h"

class LocoBoss : public BossOfBoris{
private:
    const int boardWidth;
public:
    LocoBoss(int boardWidth);
    virtual BorisGoal getGoal();
};

#endif // LOCOBOSS_H
