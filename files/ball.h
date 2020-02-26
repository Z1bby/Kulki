#include <SFML/Graphics.hpp>

#include "entity.h"
#include "board.h"

#ifndef __Installing__ball__
#define __Installing__ball__

class Ball: public Entity{
    int X;
    int Y;
    int colorId;
public:
    Ball(int x = 0,int y = 0, sf::Vector2f vect = sf::Vector2f(30,10), sf::Color c = sf::Color(0,0,255),std::string path = "images/ballSheet.png");

    void createInBoard(int x, int y, Board &board);
    void uppdateToBoard(int x, int y, Board &board);
    void followPath(Board &board, int &step);
    int getX(){return X;}
    int getY(){return Y;}
    int getColorId(){return colorId;}
    void setColorId(int a){colorId = a;}
};

#endif
