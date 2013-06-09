#ifndef TETRONIMO_H
#define TETRONIMO_H

#include <array>
#include <cstdlib>

enum TetronimoShape { NoShape, ZShape, SShape, LineShape, TShape, SquareShape,
                   LShape, MirroredLShape };

class Tetronimo{
public:
    explicit Tetronimo(TetronimoShape shape);
    explicit Tetronimo() : Tetronimo(NoShape){}
    static const Tetronimo getRandomPiece(){ return Tetronimo(TetronimoShape(rand() % 7 + 1)); }

    TetronimoShape shape() const { return pieceShape; }
    int x(int index) const { return coords[index][0]; }
    int y(int index) const { return coords[index][1]; }
    int minX() const;
    int maxX() const;
    int minY() const;
    int maxY() const;
    const Tetronimo rotatedLeft() const;
    const Tetronimo rotatedRight() const;

private:
    void setX(int index, int x) { coords[index][0] = x; }
    void setY(int index, int y) { coords[index][1] = y; }

    TetronimoShape pieceShape;
    std::array<std::array<int, 2>, 4> coords;
};

#endif
