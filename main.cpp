#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <chrono>
#include <map>
#include "functions.h"


using namespace std;


int main() {

    texturemanager texture;

    string name;
    float numcols;
    float numrows;
    float width;
    float height;
    float nummines;
    float numtiles;
    string inputName = "|";
    int numletters = 0;
    const char *letter;
    bool lose = false;
    bool gamestatus = false;
    bool mousePressed = false;
    bool debugMode = false;
    bool win = false;
    bool showleader = false;
    bool updated = false;
    bool revealboard = false;
//   cout << width << endl;
    read(width, height, nummines, numtiles, numcols, numrows);
//    cout << width << endl;
//    cout << height << endl;
    Welcome(width, height, gamestatus, name);

    int count = nummines;
    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper");

    texturemanager textures;
    vector<Player> players = readleader("files/leaderboard.txt");
    vector<vector<tile *>> board = create_board(numcols, numrows, nummines);
    if (gamestatus) {
        auto start_time = chrono::high_resolution_clock::now();
        auto pauseTime = chrono::high_resolution_clock::now();
        auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();

        bool paused = false;
        sf::Texture& digitsText = textures.digits;
        sf::Sprite digits;
        digits.setTexture(digitsText);

        map<int, sf::Sprite> digitsMap = parseDigits(digits);

        sf::Texture& pauseText = textures.pause;
        sf::Sprite pauseBttn;
        pauseBttn.setTexture(pauseText);
        pauseBttn.setPosition(numcols * 32 - 240, 32 * (numrows + 0.5));

        sf::Texture& playText = textures.play;
        sf::Sprite playBttn;
        playBttn.setTexture(playText);
        playBttn.setPosition(numcols * 32 - 240, 32 * (numrows + 0.5));


        sf::Texture& leadertext = textures.leaderboard;
        sf::Sprite leaderbttn;
        leaderbttn.setTexture(leadertext);
        leaderbttn.setPosition(numcols * 32 - 176, 32 * (numrows + 0.5));
        int total_time = 0;

        while (window.isOpen()) {
            window.clear();

            draw(window, board, textures, numcols, numrows, lose, win);
            sf::Sprite pause;
            pause.setPosition(numcols * 32 - 240, 32 * (numrows + 0.5));
            pause.setTexture(textures.pause);
            window.draw(pause);
            sf::Sprite debug;
            debug.setPosition(numcols * 32 - 304, 32 * (numrows + 0.5));
            debug.setTexture(textures.debug);
            window.draw(debug);

            sf::FloatRect bounds = debug.getLocalBounds();
            sf::RectangleShape debugButton;
            debugButton.setSize(sf::Vector2f(bounds.width, bounds.height));
            debugButton.setPosition(numcols * 32 - 304, 32 * (numrows + 0.5));
            auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
            int total_time = game_duration.count();
            int minutes;
            int seconds;

            if(!paused) {

                total_time =  total_time - elapsed_paused_time; //
                minutes = total_time / 60;
                seconds = total_time % 60;
            }

            sf::Sprite face;
            face.setPosition(numcols/2.0f * 32 - 32, 32 * (numrows + 0.5));
            if (lose) {
                face.setTexture(textures.lose);
            } else if (win){
                face.setTexture(textures.win);
            }
            else{
                face.setTexture(textures.happy);
            }
            window.draw(face);
            sf::RectangleShape reset;
            sf::FloatRect resetbounds = face.getLocalBounds();
            reset.setSize(sf::Vector2f(resetbounds.width, resetbounds.height));
            reset.setPosition(numcols/2.0f * 32 - 32, 32 * (numrows + 0.5));
            int minutes0 = minutes / 10 % 10;
            int minutes1 = minutes % 10;
            int seconds0 = seconds / 10 % 10;
            int seconds1 = seconds % 10;
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    int tileX = mousePosition.x / 32.0f;
                    int tileY = mousePosition.y / 32.0f;
                    if (debugButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x),
                                                               static_cast<float>(mousePosition.y)) && !lose) {

                        debugMode = !debugMode;

                        if (debugMode) {

                            for (int i = 0; i < board.size(); i++) {
                                for (int j = 0; j < board[i].size(); j++) {
                                    if (board[i][j]->mine) {
                                        board[i][j]->revealed = true;
                                    }
                                }
                            }
                        } else {
                            for (int i = 0; i < board.size(); i++) {
                                for (int j = 0; j < board[i].size(); j++) {
                                    if (board[i][j]->mine) {
                                        board[i][j]->revealed = false;
                                    }
                                }
                            }
                        }

                    } else if (reset.getGlobalBounds().contains(static_cast<float>(mousePosition.x),
                                                                static_cast<float>(mousePosition.y))) {
                        if (lose == true) {
                            lose = false;
                            board = create_board(numcols, numrows, nummines);
                            count = nummines;
                            paused = false;
                            start_time = chrono::high_resolution_clock::now();
                            pauseTime = chrono::high_resolution_clock::now();
                            elapsed_paused_time = chrono::duration_cast<chrono::seconds>(
                                    chrono::high_resolution_clock::now() - pauseTime).count();
                            removeasterisks("files/leaderboard.txt", players);
                        } else if (win == true) {
                            win = false;
                            board = create_board(numcols, numrows, nummines);
                            count = nummines;
                            paused = false;
                            start_time = chrono::high_resolution_clock::now();
                            pauseTime = chrono::high_resolution_clock::now();
                            elapsed_paused_time = chrono::duration_cast<chrono::seconds>(
                                    chrono::high_resolution_clock::now() - pauseTime).count();
                            updated = false;
                            removeasterisks("files/leaderboard.txt", players);

                        }
                        lose = false;
                        win = false;
                        paused = false;
                        board = create_board(numcols, numrows, nummines);
                        count = nummines;
                        start_time = chrono::high_resolution_clock::now();
                        pauseTime = chrono::high_resolution_clock::now();
                        elapsed_paused_time = chrono::duration_cast<chrono::seconds>(
                                chrono::high_resolution_clock::now() - pauseTime).count();
                        removeasterisks("files/leaderboard.txt", players);
                    }
                    if (pauseBttn.getGlobalBounds().contains(static_cast<float>(mousePosition.x),
                                                             static_cast<float>(mousePosition.y)) && !lose && !win) {
                        paused = !paused;

                        if (paused) {
                            cout << "paused" << endl;
                            pauseTime = chrono::high_resolution_clock::now();

                        } else {

                            auto unPausedTime = chrono::high_resolution_clock::now();
                            elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime -
                                                                                          pauseTime).count(); //Addition is necessary for when hitting the pause button more than once

                        }
                    }
                    if (leaderbttn.getGlobalBounds().contains(static_cast<float>(mousePosition.x),
                                                              static_cast<float>(mousePosition.y)) && !lose && !win) {
                        showleader = true;
                        revealboard = !revealboard;

                        leaderboardwindow(numcols, numrows, total_time, winner, name, showleader);

                        showleader = false;
                    } else if (tileX >= 0 && tileX < board.size() && tileY >= 0 && tileY < board[0].size() && !lose &&
                               !win) {
                        if (event.mouseButton.button == sf::Mouse::Left && !mousePressed) {
                            if (board[tileX][tileY]->mine) {
                                lose = true;

                                gamestatus = false;
                                pauseTime = chrono::high_resolution_clock::now();
                                paused = true;
                                board[tileX][tileY]->revealed = true;
                                for (int i = 0; i < board.size(); i++) {
                                    for (int j = 0; j < board[i].size(); j++) {
                                        if (board[i][j]->mine) {
                                            board[i][j]->revealed = true;
                                        }
                                    }
                                }
                            } else {
                                recursive(board, tileX, tileY);
                                mousePressed = true;
                            }
                        }

                        if (event.mouseButton.button == sf::Mouse::Right && !board[tileX][tileY]->flag) {

                            board[tileX][tileY]->flag = !board[tileX][tileY]->flag;
                            count--;
                            cout << count << endl;
                        } else if (event.mouseButton.button == sf::Mouse::Right && board[tileX][tileY]->flag) {
                            board[tileX][tileY]->flag = !board[tileX][tileY]->flag;
                            count++;
                            cout << count << endl;
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                    mousePressed = false;
                }

                if (!lose && event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Left) {
                    if (winner(board) && !showleader) {
                        pauseTime = chrono::high_resolution_clock::now();
                        win = true;
                        paused = true;
                        Player winningplayer(to_string(total_time), name);
                        if (!updated){

                            addwinner("files/leaderboard.txt",winningplayer,players);
                            updated = true;
                        }
                        leaderboardwindow(numcols, numrows, total_time, winner, name, showleader);


                    }

                }
            }



            digitsMap[minutes0].setPosition((numcols * 32) - 97, 32 * (numrows + 0.5) +16);
            window.draw(digitsMap[minutes0]);

            digitsMap[minutes1].setPosition((numcols * 32) - 97 + 21, 32 * (numrows + 0.5) +16);
            window.draw(digitsMap[minutes1]);

            digitsMap[seconds0].setPosition((numcols * 32) - 54 , 32 * (numrows + 0.5) +16);
            window.draw(digitsMap[seconds0]);

            digitsMap[seconds1].setPosition((numcols * 32) - 54 + 21, 32 * (numrows + 0.5) +16);
            window.draw(digitsMap[seconds1]);

            window.draw(pauseBttn);

            int tens = abs(count) / 10;
            int ones = abs(count) - tens * 10;

            if (count < 0){
                digitsMap[10].setPosition(12, 32 * ( numrows + 0.5) + 16);
                window.draw(digitsMap[10]);
            }
            else{
                digitsMap[0].setPosition(12, 32 * ( numrows + 0.5) + 16);
                window.draw(digitsMap[0]);
            }
            digitsMap[tens].setPosition(33, 32 * (numrows + 0.5) + 16);
            window.draw(digitsMap[tens]);
            digitsMap[ones].setPosition(33 + 21, 32 * (numrows + 0.5) + 16);
            window.draw(digitsMap[ones]);

            if(paused){
                window.draw(playBttn);
            }

            window.display();

            }


        }
    }
