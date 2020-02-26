#include <SFML/Graphics.hpp> // sfml 4.2
#include <ctime>
#include <vector>
#include <iostream>
#include <fstream>
#include <windows.h>

#include "files/entity.h"
#include "files/board.h"
#include "files/ball.h"
#include "files/collisions.h"

void updatePointsText(std::string txt,int points, sf::Text &textPoints){
    std::vector <char> numbers;
    while(points > 0){
        numbers.push_back(char(points%10 + '0'));
        points /= 10;
    }

    std::string updatedText="";
    for(int i=numbers.size() - 1; i>=0; i--)
        updatedText += numbers[i];
    textPoints.setString(txt + updatedText);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode().getDesktopMode(), "Kulki" /*,sf::Style::Fullscreen*/);
    window.setSize(sf::Vector2u(window.getSize().x-16, window.getSize().y-78));
    window.setPosition(sf::Vector2i(0,0));
    //window.setFramerateLimit(240);
    srand(time(NULL));

    Board board(sf::Vector2f(30,30)); // Creating board
    Collisions collisions;

    std::vector < Ball > ball; // Creating ball vector

    std::vector <Ball> upComingBall(3); // balls which will turn up after next move
    for(int i=0; i<3; i++)
        upComingBall[i].sprite.setPosition(30 + board.texture.getSize().x * board.sprite.getScale().x + 30 + i*70,100);

    collisions.updateUpComingBalls(upComingBall);
    collisions.spreadBalls(upComingBall,ball,board);
    collisions.updateUpComingBalls(upComingBall);

    BoardTile boardTile(0,0,sf::Vector2f(30,30)); // highlights tile under mouse cursor

    Entity upComing("images/upcoming.png"); // the thing underneath "HighScore"
    upComing.sprite.setPosition(30 + board.texture.getSize().x * board.sprite.getScale().x + 30,120);

    // Loading Font
    sf::Font fontello;
    if(!fontello.loadFromFile("fonts/arial.ttf")){
        return EXIT_FAILURE;
    }
    sf::Text textPoints("Points: 0",fontello,32);
    textPoints.setPosition(30 + board.texture.getSize().x * board.sprite.getScale().x + 30,30);
    sf::Text textHighScore("HighScore: 0",fontello,32);
    textHighScore.setPosition(30 + board.texture.getSize().x * board.sprite.getScale().x + 30,70);
    sf::Text textStart("Start",fontello,32);
    textStart.setPosition(window.getSize().x/2 - 80,window.getSize().y/2);
    sf::Text textGameOver("GameOver",fontello,32);
    textGameOver.setPosition(window.getSize().x/2 - 128,window.getSize().y/2);

    int highScore = 0;
    {
        std::ifstream highScoreStream("txt/highscore.txt");
        std::string a;
        highScoreStream >> a;
        textHighScore.setString("HighScore: " + a);
        highScoreStream.close();
        for(int i=0; i<a.size(); i++){
            highScore *= 10;
            highScore += (int)(a[i] - '0');
        }
    }


    int record_moving = 0, record_bouncing = 0, sprite_position = 0; // sprite position isn't in the ball class because there is only one bouncing ball at the time
    bool pressing_mouse = false; // check if released button was pressed in last frame
    int highlighted_ball = -1; // which ball of "vector <Ball>" was just pressed
    bool moving = false; // tells if some ball is moving right now (if is, ignore the mouse)
    int step = 0; // tells which position in board.PathX/Y array to go, in order to move to highlighted position
    int points = 0;
    bool playing = false;
    bool gameover = false;

    // ====================================================[ | ]===========[ | ]====================================================
    // ====================================================| | | GAME LOOP | | |====================================================
    // ====================================================[ | ]===========[ | ]====================================================
    while(window.isOpen()){

        sf::Event event;

        if(window.pollEvent(event)){

            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
            if(event.type == sf::Event::Resized){
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height))); // prevent scaling shapes
            }
        }
        // ====================================================[ | ]==============[ | ]====================================================
        // ====================================================| | | WINDOW CLEAR | | |====================================================
        // ====================================================[ | ]==============[ | ]====================================================
        window.clear(sf::Color(128,128,128));

        if(playing){
            int record = clock(); // time
            if(highlighted_ball > -1){ // if some ball is bouncing
                if(record - record_bouncing > 80 - int(moving)*60){ // update image once in 1/10 of a second
                    sprite_position++;
                    if(sprite_position == 6){
                        sprite_position = 0;
                    }
                    ball[highlighted_ball].sprite.setTextureRect(sf::IntRect(sprite_position*70, 0, 70, 80)); // moveUpdate
                    record_bouncing = record;
                }
                if(record - record_moving >= 0){
                    if(moving){ // if the ball is on its way to the highlighted tile
                        ball[highlighted_ball].followPath(board,step);

                        if(step == board.PathX.size()){ // if ball ended walking
                            ball[highlighted_ball].uppdateToBoard(board.PathX[step-1],board.PathY[step-1],board); // update the ball to the array
                            moving = false; // ball is not moving anymore
                            step = 0; // reset steps
                            board.clearPath();  // reset path
                            ball[highlighted_ball].sprite.setTextureRect(sf::IntRect(0, 0, 70, 80)); // update the image to starting position
                            highlighted_ball = -1; // stop highliting the ball

                            int a = points;

                            // looking for balls in straight line to pop
                            collisions.popBalls(ball,board,points);
                            if(a == points){
                                collisions.spreadBalls(upComingBall,ball,board);
                                collisions.updateUpComingBalls(upComingBall);
                                collisions.popBalls(ball,board,points);
                            }

                            if(board.getAvailablePositions() == 0){
                                playing = false;
                                gameover = true;
                                textPoints.setPosition(window.getSize().x/2 - 128,window.getSize().y/2+40);
                                if(points > highScore){
                                    std::ofstream highScoreStream("txt/highscore.txt");
                                    highScoreStream << points;
                                    updatePointsText("HighScore: ",points,textHighScore);
                                    highScoreStream.close();
                                }
                            }
                            updatePointsText("Points: ",points,textPoints);

                        }//if(step == board.PathX.size())
                    } // if(moving)

                    record_moving = record;
                }
            }

            if(!moving && sf::Mouse::getPosition(window).x > board.getVector(0,0,0) && sf::Mouse::getPosition(window).x < board.getVector(8,8,2) &&
               sf::Mouse::getPosition(window).y > board.getVector(0,0,1) && sf::Mouse::getPosition(window).y < board.getVector(8,8,3) ){
                boardTile.setActive(true); // if ball isn't moving and mouse is inside board -> highlight a tile
            }
            else{
                boardTile.setActive(false);
            }

            if(!moving && sf::Mouse::isButtonPressed(sf::Mouse::Left)){ // if pressed a mouse -> tell that mouse is pressed
                pressing_mouse = true;
            }
            if(!moving && !sf::Mouse::isButtonPressed(sf::Mouse::Left)){ // if mouse isn't pressed
                if(pressing_mouse){ // if it was pressed in last frame
                    bool isThereBall = false; // check if the ball is pressed
                    for(int i=0; i<ball.size(); i++){
                        if(ball[i].getX() == boardTile.getX() && ball[i].getY() == boardTile.getY()){
                            isThereBall = true; // ball was pressed, highlight it, or unhighlight if it was highlighted
                            if(highlighted_ball == i){
                                ball[highlighted_ball].sprite.setTextureRect(sf::IntRect(0,0,70,80));
                                highlighted_ball = -1;
                            }
                            else if(highlighted_ball>-1){
                                ball[highlighted_ball].sprite.setTextureRect(sf::IntRect(0,0,70,80));
                                highlighted_ball = i;
                            }
                            else{
                                highlighted_ball = i;
                            }
                            i = ball.size(); // break the loop
                        }
                    }
                    if(!isThereBall && highlighted_ball > -1 && boardTile.getActive()){ // if an empty tile was pressed, and ball was highlighted -> move ball there
                        if(board.createPath(ball[highlighted_ball].getX(), ball[highlighted_ball].getY(), boardTile.getX(), boardTile.getY())){
                            moving = true; // if ball is able to go there
                            step = 1;
                        }
                        else{ // if ball isn't able to go to the highlighted tile
                            ball[highlighted_ball].sprite.setTextureRect(sf::IntRect(0,0,70,80)); // set it to starting position
                            highlighted_ball = -1; // stop bouncing
                        }
                    }

                } // if pressing mouse
                pressing_mouse = false;
            } // if mouse isn't pressed
            boardTile.updatePosition(window,board); // update the tile
            window.draw(board.sprite); // draw draw draw
            window.draw(boardTile.sprite);
            window.draw(upComing.sprite);

            for(int i=0; i<ball.size(); i++){
                window.draw(ball[i].sprite);
            }
            for(int i=0; i<3; i++){
                window.draw(upComingBall[i].sprite);
            }

            window.draw(textPoints);
            window.draw(textHighScore);
        } // if(playing)
        else{
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){ // if pressed a mouse -> tell that mouse is pressed
                pressing_mouse = true;
            }
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if(sf::Mouse::getPosition(window).x < window.getSize().x/2 + 80 && sf::Mouse::getPosition(window).x > window.getSize().x/2 - 80 &&
                   sf::Mouse::getPosition(window).y < window.getSize().y/2 + 128 && sf::Mouse::getPosition(window).y > window.getSize().y/2 - 128 ){
                       textStart.setColor(sf::Color(0,0,255));
                       textGameOver.setColor(sf::Color(0,0,255));
                       if(pressing_mouse){
                            if(gameover) gameover = false;
                            else playing = true;
                            textPoints.setPosition(30 + board.texture.getSize().x * board.sprite.getScale().x + 30,30);
                            ball.resize(0);
                            board.clearBoard();
                            collisions.updateUpComingBalls(upComingBall);
                            collisions.spreadBalls(upComingBall,ball,board);
                            collisions.updateUpComingBalls(upComingBall);
                            points = 0;
                            textPoints.setString("Points: 0");
                       }
                   }
                   else{
                        textStart.setColor(sf::Color(255,255,255));
                        textGameOver.setColor(sf::Color(255,255,255));
                   }
                pressing_mouse = false;
            }
            if(gameover){
                window.draw(textGameOver);
                window.draw(textPoints);
            }
            else{
                window.draw(textStart);
            }
        }
        window.display();
        //Sleep(50);
    } // gameLoop

    return 0;
}
