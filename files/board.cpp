#include "board.h"

// ============================ BOARD =================================

Board::Board(sf::Vector2f vect, std::string path): Entity(path){
    texture.loadFromFile(path);
    sprite.setTexture(texture);
    sprite.setPosition(vect);

    for(int h=0; h<9; h++){
        for(int w=0; w<9; w++){
            board[w][h][0] = sprite.getPosition().x + w*70*sprite.getScale().x;
            board[w][h][1] = sprite.getPosition().y + h*70*sprite.getScale().y;
            board[w][h][2] = sprite.getPosition().x + (w+1)*70*sprite.getScale().x;
            board[w][h][3] = sprite.getPosition().y + (h+1)*70*sprite.getScale().y;
            board[w][h][4] = 0;
            Exploration[w][h] = -1;
        }
    }
    availablePositions = 81;
    for(int i=0; i<81; i++)
        Positions[i] = 0; // false = there are no balls
}

int Board::tellX(int x){
    for(int i=1; i<9; i++){
        if(board[i][0][0] > x) return i-1;
    }
    return 8;
}
int Board::tellY(int y){
    for(int i=1; i<9; i++){
        if(board[0][i][1] > y) return i-1;
    }
    return 8;
}

bool Board::setBoard(int x,int y, int a){
    if(x<9 && y<9 && x>-1 && y>-1){
        board[x][y][4] = a;
        Positions[x+(y*9)] = a;
    }
    else return EXIT_FAILURE;
}

bool Board::createPath(int x1,int y1, int x2, int y2){
    Exploration[x1][y1] = 0;
    int step = 1;

    bool foundTarget = false;
    bool findingTarget = true;

    while(findingTarget){
        findingTarget = false;

        for(int h=0; h<9; h++)
            for(int w=0; w<9; w++)
                if(Exploration[w][h] == step-1){
                    bool keepSearching = true;
                    if(h>0){
                        if(w == x2 && h - 1 == y2){
                            foundTarget = true;
                            Exploration[w][h-1] = step;
                            keepSearching = false;
                            w=9; h=9;
                            findingTarget = false;
                        }
                        else if(board[w][h-1][4] == 0 && Exploration[w][h-1] == -1){
                            Exploration[w][h-1] = step;
                            findingTarget = true;
                        }
                    }
                    if(keepSearching && w>0)
                        if(w - 1 == x2 && h == y2){
                            foundTarget = true;
                            Exploration[w-1][h] = step;
                            keepSearching = false;
                            w=9; h=9;
                            findingTarget = false;
                        }
                        else if(board[w-1][h][4] == 0 && Exploration[w-1][h] == -1){
                            Exploration[w-1][h] = step;
                            findingTarget = true;
                        }
                    if(h<8){
                        if(w == x2 && h + 1 == y2){
                            foundTarget = true;
                            Exploration[w][h+1] = step;
                            keepSearching = false;
                            w=9; h=9;
                            findingTarget = false;
                        }
                        else if(board[w][h+1][4] == 0 && Exploration[w][h+1] == -1){
                            Exploration[w][h+1] = step;
                            findingTarget = true;
                        }
                    }
                    if(keepSearching && w<8)
                        if(w + 1 == x2 && h == y2){
                            foundTarget = true;
                            Exploration[w+1][h] = step;
                            keepSearching = false;
                            w=9; h=9;
                            findingTarget = false;
                        }
                        else if(board[w+1][h][4] == 0 && Exploration[w+1][h] == -1){
                            Exploration[w+1][h] = step;
                            findingTarget = true;
                        }
                } // if(exploration[x][y] == previous step)
        step++;
    }// while(finding target)
    if(!foundTarget){
        clearPath();
        return false;
    }

    PathX.resize(step);
    PathY.resize(step);
    step --;
    PathX[step] = x2;
    PathY[step] = y2;
    while(Exploration[x2][y2] != 0){
        bool keepSearching = true;
        if(y2>0)
            if(Exploration[x2][y2-1] == step){
            y2--;
            PathX[step] = x2;
            PathY[step] = y2;
            keepSearching = false;
        }
        if(keepSearching && x2>0)
            if(Exploration[x2-1][y2] == step){
            x2--;
            PathX[step] = x2;
            PathY[step] = y2;
            keepSearching = false;
        }
        if(keepSearching && y2<8)
            if(Exploration[x2][y2+1] == step){
            y2++;
            PathX[step] = x2;
            PathY[step] = y2;
            keepSearching = false;
        }
        if(keepSearching && x2<8)
            if(Exploration[x2+1][y2] == step){
            x2++;
            PathX[step] = x2;
            PathY[step] = y2;
            keepSearching = false;
        }
        step --;
    }// while exploration[x][y] != 0
    return true;
}
void Board::clearPath(){
    PathX.resize(0);
    PathY.resize(0);
    for(int h=0; h<9; h++)
        for(int w=0; w<9; w++)
            Exploration[w][h] = -1;
}
void Board::clearBoard(){
    for(int h=0; h<9; h++){
        for(int w=0; w<9; w++){
            board[w][h][4] = 0;
        }
    }
    availablePositions = 81;
    for(int i=0; i<81; i++)
        Positions[i] = 0; // false = there are no balls
    availablePositions = 81;
}
// ============================ BOARD TILE =================================

BoardTile::BoardTile(int x, int y, sf::Vector2f vect, std::string path): Entity(path){
    X = x;
    Y = y;
    texture.loadFromFile(path);
    sprite.setTexture(texture);
    sprite.setPosition(vect);
    active = false;
    sprite.setColor(sf::Color(255,255,255,0));
}
void BoardTile::updatePosition(sf::RenderWindow &window, Board &board){
    X = board.tellX(sf::Mouse::getPosition(window).x);
    Y = board.tellY(sf::Mouse::getPosition(window).y);
    sprite.setPosition(sf::Vector2f(board.getVector(X,Y,0), board.getVector(X,Y,1)));
}
