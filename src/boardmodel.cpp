#include "boardmodel.h"

BoardModel::BoardModel(int width, int height): height(height), width(width), tiles(width*height){
}

void BoardModel::clear()
{
    for (int i = 0; i < height * width; ++i){
        tiles[i] = NoShape;
    }
}
