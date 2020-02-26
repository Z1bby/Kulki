#include "collisions.h"

Collisions::Collisions(){}

void Collisions::spreadBalls(std::vector <Ball> &upComingBall, std::vector <Ball> &ball, Board &board){
    for(int i=0; i<3; i++){
        ball.push_back(upComingBall[i]);

        int a = rand()%board.getAvailablePositions();
        board.setAvailablePositions(board.getAvailablePositions()-1); // decrease because just took a random position
        int counter = 0; // goes through array until it finds "int a" empty places in array
        while(a != 0 || board.getPosition(counter) != 0){ // break loop when you passed (int)a - 1 empty positions, and Positions[counter] is empty
            if(board.getPosition(counter) == 0){
                a --; // decreases a, when spots empty position
            }
            counter ++;
        }
        board.setPosition(counter,true); // this position is no longer empty
        ball[ball.size() - 1].createInBoard(counter%9,counter/9,board);
        //ball[ball.size() - 1].sprite.setTexture(ball[ball.size() - 1].texture);
        if(board.getAvailablePositions() == 0) i = 3;
    }
    for(int i=0; i<ball.size(); i++){
        ball[i].sprite.setTexture(ball[i].texture);
    }
}
void Collisions::updateUpComingBalls(std::vector <Ball> &upComingBall){
    for(int i=0; i<3; i++){
        int color = rand()%6;
        upComingBall[i].sprite.setColor(sf::Color(arrayColors[color][0],arrayColors[color][1],arrayColors[color][2]));
        upComingBall[i].setColorId(color);
        upComingBall[i].sprite.setTexture(upComingBall[i].texture);
    }
}
void Collisions::popBalls(std::vector <Ball> &ball, Board &board, int &points){
    for(int h=0; h<9; h++){
        for(int w=0; w<9; w++){
            if(board.getBoard(w,h) != 0){
                bool keepSearching = true;
                int lenght = 0;
                while(w + lenght < 9 && board.getBoard(w,h) == board.getBoard(w+lenght,h)){ // looking for - line
                    lenght ++;
                }
                if(lenght > 4){ // if a line is more than 4
                    keepSearching = false;
                    points += (lenght-4)*5;
                    for(int j=0; j<lenght; j++){ // for how long is the line ...
                        for(int k=0; k<ball.size(); k++){ // ... search balls with x and y from the line ...
                            if(ball[k].getX() == w+j && ball[k].getY() == h){
                                for(int l=k; l<ball.size() - 1; l++){ // ... and pop it ...
                                    ball[l] = ball[l+1];
                                }
                                ball.pop_back();
                            }
                        }
                        board.setBoard(w+j,h,0); // ... and set position to empty ...
                    }
                    board.setAvailablePositions(board.getAvailablePositions()+lenght); // ... and also increase number of empty positions
                    h = 8; // break the loop
                    w = 8;
                }
                lenght = 0;
                while(keepSearching && h + lenght < 9 && board.getBoard(w,h) == board.getBoard(w,h+lenght)){ // looking for | line
                    lenght ++;
                }
                if(keepSearching && lenght > 4){
                    keepSearching = false;
                    points += (lenght-4)*5;
                    for(int j=0; j<lenght; j++){
                        for(int k=0; k<ball.size(); k++){
                            if(ball[k].getX() == w && ball[k].getY() == h+j){
                                for(int l=k; l<ball.size() - 1; l++){
                                    ball[l] = ball[l+1];
                                }
                                ball.pop_back();
                            }
                        }
                        board.setBoard(w,h+j,0);
                    }
                    board.setAvailablePositions(board.getAvailablePositions()+lenght);
                    h = 8;
                    w = 8;
                }
                lenght = 0;
                while(keepSearching && w + lenght < 9 && h + lenght < 9 && board.getBoard(w,h) == board.getBoard(w+lenght,h+lenght)){ // looking for \ line
                    lenght ++;
                }
                if(keepSearching && lenght > 4){
                    keepSearching = false;
                    points += (lenght-4)*5;
                    for(int j=0; j<lenght; j++){
                        for(int k=0; k<ball.size(); k++){
                            if(ball[k].getX() == w+j && ball[k].getY() == h+j){
                                for(int l=k; l<ball.size() - 1; l++){
                                    ball[l] = ball[l+1];
                                }
                                ball.pop_back();
                            }
                        }
                        board.setBoard(w+j,h+j,0);
                    }
                    board.setAvailablePositions(board.getAvailablePositions()+lenght);
                    h = 8;
                    w = 8;
                }
                lenght = 0;
                while(keepSearching && w - lenght >= 0 && h + lenght < 9 && board.getBoard(w,h) == board.getBoard(w-lenght,h+lenght)){ // looking for / line
                    lenght ++;
                }
                if(keepSearching && lenght > 4){
                    keepSearching = false;
                    points += (lenght-4)*5;
                    for(int j=0; j<lenght; j++){
                        for(int k=0; k<ball.size(); k++){
                            if(ball[k].getX() == w-j && ball[k].getY() == h+j){
                                for(int l=k; l<ball.size() - 1; l++){
                                    ball[l] = ball[l+1];
                                }
                                ball.pop_back();
                            }
                        }
                        board.setBoard(w-j,h+j,0);
                    }
                    board.setAvailablePositions(board.getAvailablePositions()+lenght);
                    h = 8;
                    w = 8;
                }
            }//if(board != 0)
        }// for(w)
    } // for(h)
    for(int i=0; i<ball.size(); i++){
        ball[i].sprite.setTexture(ball[i].texture);
    }
}
