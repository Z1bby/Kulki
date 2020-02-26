#include <SFML/Graphics.hpp>
#include <vector>

#include "entity.h"

#ifndef __Installing__board__
#define __Installing__board__

class Board: public Entity{
    int board[9][9][5]; // board[x][y] [0 - top-left-x, 1 - top-left-y, 2 - bottom-right-x, 3 - bottom-right-y, 4 - what is there
    int availablePositions; // tells number of empty spots on the board
    int Positions[81]; // Positions[x] = board.getBoard(x%9,x/9), it makes easier putting balls to random spots
public:
    int Exploration[9][9];
    std::vector <int> PathX;
    std::vector <int> PathY;
    Board(sf::Vector2f vect = sf::Vector2f(30,30), std::string path = "images/board.png");
    bool setBoard(int x,int y,int a);
    int getBoard(int x,int y){if(x<9 && y<9 && x>-1 && y>-1) return board[x][y][4]; return EXIT_FAILURE;}
    int getVector(int x,int y, int a){if(x<9 && y<9 && x>-1 && y>-1 && a>-1 && a<4) return board[x][y][a]; return EXIT_FAILURE;}
    int tellX(int x);
    int tellY(int y);
    int getAvailablePositions(){return availablePositions;}
    void setAvailablePositions(int a){availablePositions = a;}
    int getPosition(int x, int y){return Positions[x+(y*9)];}
    int getPosition(int x){return Positions[x];}
    void setPosition(int x, int y, int a){Positions[x+(y*9)] = a;}
    void setPosition(int x, int a){Positions[x] = a;}

    bool createPath(int x1,int y1, int x2, int y2);
    void clearPath();
    void clearBoard();
};
#endif

#ifndef __Installing__boardTile__
#define __Installing__boardTile__

class BoardTile: public Entity{
    bool active;
    int X;
    int Y;
public:
    BoardTile(int x = 0,int y = 0,sf::Vector2f vect = sf::Vector2f(30,30), std::string path = "images/boardTile.png");
    void updatePosition(sf::RenderWindow &window, Board &board);

    void setX(int a){X = a;}
    int getX(){return X;}
    void setY(int a){Y = a;}
    int getY(){return Y;}
    void setActive(bool a){
        active = a;
        if(!a) sprite.setColor(sf::Color(255,255,255,0));
        else sprite.setColor(sf::Color(255,255,255));
    }
    bool getActive(){return active;}
};
#endif
