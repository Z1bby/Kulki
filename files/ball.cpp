#include "ball.h"
#include "entity.h"

Ball::Ball(int x, int y,sf::Vector2f vect,sf::Color c, std::string path): Entity(path){
    X = x;
    Y = y;
    texture.loadFromFile(path);
    sprite.setPosition(vect);
    sprite.setColor(c);
    sprite.setTextureRect(sf::IntRect(0, 0, 70, 80));
}

void Ball::createInBoard(int x, int y, Board &board){
    X = x;
    Y = y;
    sprite.setPosition(sf::Vector2f(board.getVector(X,Y,0), board.getVector(X,Y,1) - 20));
    board.setBoard(X,Y,getColorId()+1);
}

void Ball::uppdateToBoard(int x, int y, Board &board){
    board.setBoard(X,Y,0);
    X = x;
    Y = y;
    sprite.setPosition(sf::Vector2f(board.getVector(X,Y,0), board.getVector(X,Y,1) - 20));
    board.setBoard(X,Y,getColorId()+1);
}

void Ball::followPath(Board &board, int &step){
    if(sprite.getPosition().x > board.getVector(board.PathX[step],board.PathY[step],0))
            sprite.move(-70,0); // if next tile is on its left, go left
        else if(sprite.getPosition().x < board.getVector(board.PathX[step],board.PathY[step],0))
            sprite.move(70,0);
        else if(sprite.getPosition().y > board.getVector(board.PathX[step],board.PathY[step],1)-20)
            sprite.move(0,-70);
        else if(sprite.getPosition().y < board.getVector(board.PathX[step],board.PathY[step],1)-20)
            sprite.move(0,70);
        else step++; // if ball position == next tile position, go to another tile
}
