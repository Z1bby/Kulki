#include <SFML/Graphics.hpp>

#include "board.h"
#include "ball.h"

#ifndef __InstallingSFML__collisions__
#define __InstallingSFML__collisions__

class Collisions{
public:
    int arrayColors[6][3] = { {255,0,0}, {255,255,0}, {255,0,255}, {0,255,255}, {0,255,0}, {0,0,255}};

    Collisions();
    void updateUpComingBalls(std::vector <Ball> &upComingBall);
    void spreadBalls(std::vector <Ball> &upComingBall, std::vector <Ball> &ball, Board &board);
    void popBalls(std::vector <Ball> &ball, Board &board, int &points);

};
#endif
