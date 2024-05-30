#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <map>
#include <chrono>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

sf::RectangleShape reset;

class tile{
public:
    bool flag = false;
    bool revealed = false;
    bool mine = false;
    bool debug = false;
    bool pause = false;
    int nearby = 0;
    tile(){}
};

class texturemanager{
public:
    sf::Texture mine, hidden, revealed, happy, win, lose, digits, debug, pause, play, leaderboard, flag;
    map<string,sf::Texture> files;
    texturemanager(){
        for (int i = 1; i < 9; i++){
            files[to_string(i)].loadFromFile("images/number_"+ to_string(i) +".png");
        }
        mine.loadFromFile("images/mine.png");
        hidden.loadFromFile("images/tile_hidden.png");
        revealed.loadFromFile("images/tile_revealed.png");
        happy.loadFromFile("images/face_happy.png");
        win.loadFromFile("images/face_win.png");
        lose.loadFromFile("images/face_lose.png");
        flag.loadFromFile("images/flag.png");
        digits.loadFromFile("images/digits.png");
        debug.loadFromFile("images/debug.png");
        pause.loadFromFile("images/pause.png");
        play.loadFromFile("images/play.png");
        leaderboard.loadFromFile("images/leaderboard.png");
    }
};

class Player{
public:
    string name;
    string time;

    Player(const string& t, const string& n) : time(t), name(n) {}
    void pop_back() {
        if (!name.empty()) {
            name.pop_back();
        }
    }
};

vector<vector<tile*>> create_board(int col, int row, int mines){
    int count = 0;
    vector<vector<tile*>> board;
    for (int i = 0; i < col; i++){
        vector<tile*> tiles;
        for (int j = 0; j < row; j++){
            tiles.push_back(new tile());
        }
        board.push_back(tiles);
    }
    while (count < mines){
        int y = rand() % row;
        int x = rand() % col;

        if (!board[x][y] -> mine){
            board[x][y] -> mine = true;
            count += 1;
        }
    }
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            int mins = 0;
            if (!board[i][j]->mine) {

                for (int left = -1; left < 2; left++) {
                    for (int right = -1; right < 2; right++) {
                        int x = i + left;
                        int y = j + right;


                        if (x >= 0 && y >= 0 && x < board.size() && y < board[0].size()) {
                            if (board[x][y]->mine) {
                                mins += 1;
                            }
                        }
                    }
                }
                board[i][j]->nearby = mins;
            }
        }
    }
    return board;
}

void read(float &width, float &height, float &nummines, float &numtiles, float &numcols, float&numrows){
    ifstream file("files/config.cfg");

    file >> numcols;
    file >> numrows;
    file >> nummines;

    width = numcols * 32;
    height = numrows * 32 + 100;

    numtiles = numrows * numcols;
}

void add(string &name, int&numletters, char letter){
    const char* line = "|";
    if (numletters == 10){
        cout << "Limit reached" << endl;
    }

    else if(numletters == 0){
        name.clear();
        name.push_back(toupper(letter));
        name.push_back(*line);
        numletters += 1;
    }

    else{
        name.pop_back();
        name.push_back(tolower(letter));
        name.push_back(*line);
        numletters += 1;
    }
}

void remove(string &name, int &numletters){
    const char* line = "|";
    if (numletters != 0){
        name.pop_back();
        name.pop_back();
        name.push_back(*line);
        numletters -= 1;
    }
}

void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void Welcome(float width, float height, bool &gamestate, string &name){
    string inputName = "|";
    int numletters = 0;

    const char* letter;

    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper");
    sf::Font font;
    font.loadFromFile("files/font.ttf");
    if (!font.loadFromFile("files/font.ttf")){
        cout << "error" << endl;
    }

    sf::Text welcome_text;
    welcome_text.setFont(font);
    welcome_text.setString("Welcome to Minesweeper!");
    welcome_text.setCharacterSize(24);
    welcome_text.setFillColor(sf::Color::White);
    welcome_text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::FloatRect textRect = welcome_text.getLocalBounds();
    welcome_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    welcome_text.setPosition(sf::Vector2f(width/2.0f, height/2.0f - 150));

    sf::Text input_text;
    input_text.setFont(font);
    input_text.setString("Enter your name: ");
    input_text.setCharacterSize(20);

    setText(input_text,width/2.0f, height/2.0f - 75);

    sf::Text input;
    input.setFont(font);
    input.setStyle(sf::Text::Bold);
    input.setFillColor(sf::Color::Yellow);
    input.setCharacterSize(18);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                window.close();
            }

            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::A){
                    letter = "a";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::B){
                    letter = "b";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::C){
                    letter = "c";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::D){
                    letter = "d";
                    add(inputName, numletters, *letter);
                }if (event.key.code == sf::Keyboard::E){
                    letter = "e";
                    add(inputName, numletters, *letter);
                }if (event.key.code == sf::Keyboard::F){
                    letter = "f";
                    add(inputName, numletters, *letter);
                }if (event.key.code == sf::Keyboard::G){
                    letter = "g";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::H){
                    letter = "h";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::I){
                    letter = "i";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::J){
                    letter = "j";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::K){
                    letter = "k";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::L){
                    letter = "l";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::M){
                    letter = "m";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::N){
                    letter = "n";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::O){
                    letter = "o";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::P){
                    letter = "p";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::Q){
                    letter = "q";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::R){
                    letter = "r";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::S){
                    letter = "s";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::T){
                    letter = "t";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::U){
                    letter = "u";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::V){
                    letter = "v";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::W){
                    letter = "w";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::X){
                    letter = "x";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::Y){
                    letter = "y";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::Z){
                    letter = "z";
                    add(inputName, numletters, *letter);
                }
                if (event.key.code == sf::Keyboard::BackSpace){
                    remove(inputName, numletters);
                }
                if (event.key.code == sf::Keyboard::Enter){
                    cout << "Enter is pressed" << endl;
                    if (numletters != 0){
                        inputName.pop_back();
                        name = inputName;
                        window.close();
                        gamestate = true;
                    }
                }
                else{}
            }

            input.setString(inputName);
            setText(input, width/2.0f, height/2.0f - 45);
            window.clear(sf::Color::Blue);
            window.draw(welcome_text);
            window.draw(input);
            window.draw(input_text);
            window.display();
        }
    }
}

bool compareTimes(const string& time1, const string& time2) {

    int minutes1, seconds1, minutes2, seconds2;
    sscanf(time1.c_str(), "%d:%d", &minutes1, &seconds1);
    sscanf(time2.c_str(), "%d:%d", &minutes2, &seconds2);


    if (minutes1 != minutes2) {
        return minutes1 < minutes2;
    }

    return seconds1 < seconds2;
}

vector<Player> readleader(const string &filename){
    vector<Player> players;

    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                size_t commaPos = line.find(',');
                if (commaPos != string::npos) {
                    string timeStr = line.substr(0, commaPos);
                    string name = line.substr(commaPos + 1);

                    players.emplace_back(Player{timeStr, name});
                } else {
                    cerr << "Error: Invalid format in line: " << line << endl;
                }
            }
        }
        file.close();

       
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return compareTimes(a.time, b.time);
        });
    }

    return players;
}

void addwinner(const string& filename, Player& winner, vector<Player>& players) {
    for (auto& player : players) {
        player.name.erase(remove(player.name.begin(), player.name.end(), '*'), player.name.end());
    }

    ofstream file(filename, ios::app);

    if (file.is_open()) {
       
        size_t colonPos = winner.time.find(':');
        int totalSeconds = 0;

        if (colonPos != string::npos) {
          
            stringstream ss(winner.time);
            string token;
            getline(ss, token, ':');
            totalSeconds += stoi(token) * 60; 
            getline(ss, token, ':');
            totalSeconds += stoi(token); 
        } else {
           
            totalSeconds = stoi(winner.time);
        }

       
        bool overtaken = false;
        for (auto& player : players) {
            int playerSeconds = 0;
            size_t colonPosPlayer = player.time.find(':');

            if (colonPosPlayer != string::npos) {
               
                stringstream ssPlayer(player.time);
                string token;
                getline(ssPlayer, token, ':');
                playerSeconds += stoi(token) * 60;
                getline(ssPlayer, token, ':');
                playerSeconds += stoi(token); 
                cout << playerSeconds << endl;
            } else {
               
                playerSeconds = stoi(player.time);
            }

            if (totalSeconds < playerSeconds) {
                overtaken = true;

            }
        }
        if (overtaken){
            winner.name += "*";
        }

        
        if (overtaken) {
            players.push_back(winner);
        }

       
        file << setw(2) << setfill('0') << totalSeconds / 60 << ":" << setw(2) << setfill('0') << totalSeconds % 60
             << "," << winner.name << endl;

        file.close();
    } else {
        cerr << "Error: Unable to open file " << filename << endl;
    }
}


void removeasterisks(const string& filename, vector<Player>& players) {
    for (auto& player : players) {

        if (!player.name.empty() && player.name.back() == '*') {

            player.name.pop_back();
        }
    }

  
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& player : players) {

            cout << player.time << endl;

            if (player.time.length() < 3){
                int totalSeconds = stoi(player.time);
                int minutes = totalSeconds / 60;
                int seconds = totalSeconds % 60;
                int minutes0 = minutes / 10 % 10;
                int minutes1 = minutes % 10;
                int seconds0 = seconds / 10 % 10;
                int seconds1 = seconds % 10;

                file << minutes0 << minutes1 << ":" << seconds0 << seconds1 << "," << player.name << endl;
            }
            else{
                file << player.time << "," << player.name << endl;
            }

        }
        file.close();
    } else {
        cerr << "Error: Unable to open file " << filename << endl;
    }
}

void leaderboardwindow(int cols, int rows, int winningtime, bool winner, string name, bool showleaders) {
    vector<Player> players = readleader("files/leaderboard.txt");


    float width = cols * 16;
    float height = rows * 16 + 50;
    sf::RenderWindow leaderboard(sf::VideoMode(width, height), "Minesweeper");
    sf::Event event;
    sf::Font font;
    font.loadFromFile("files/font.ttf");

    sf::Text leadtext;
    leadtext.setFont(font);
    leadtext.setString("LEADERBOARD");
    leadtext.setCharacterSize(20);
    leadtext.setFillColor(sf::Color::White);
    leadtext.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(leadtext, width / 2.0f, height / 2.0f - 120);

    vector<sf::Text> rankTexts;
    vector<sf::Text> timeTexts;
    vector<sf::Text> nameTexts;

    float startX = width / 2.0f - 120.0f;
    float startY = height / 2.0f - 80;   

    for (size_t rank = 1; rank <= 5 && rank <= players.size(); ++rank) {
        const auto& player = players[rank - 1];

        sf::Text rankText;
        rankText.setFont(font);
        rankText.setString(to_string(rank) + ".");
        rankText.setCharacterSize(18);
        rankText.setFillColor(sf::Color::White);
        rankText.setStyle(sf::Text::Bold);
        rankText.setPosition(startX, startY);
        rankTexts.push_back(rankText);

        sf::Text timeText;
        timeText.setFont(font);
        timeText.setString(player.time);
        timeText.setCharacterSize(18);
        timeText.setFillColor(sf::Color::White);
        timeText.setStyle(sf::Text::Bold);
        timeText.setPosition(startX + 75.0f, startY);  
        timeTexts.push_back(timeText);

        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(player.name);
        nameText.setCharacterSize(18);
        nameText.setFillColor(sf::Color::White);
        nameText.setStyle(sf::Text::Bold);
        
        float offsetX = nameText.findCharacterPos(0).x;
        nameText.setPosition(startX + 175.0f - offsetX, startY);

        nameTexts.push_back(nameText);

        startY += rankText.getLocalBounds().height + 20;
    }

    while (leaderboard.isOpen()) {
        while (leaderboard.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                leaderboard.close();
            }

            leaderboard.clear(sf::Color::Blue);
            leaderboard.draw(leadtext);

            for (size_t i = 0; i < rankTexts.size(); ++i) {
                leaderboard.draw(rankTexts[i]);
                leaderboard.draw(timeTexts[i]);
                leaderboard.draw(nameTexts[i]);
            }

            leaderboard.display();
        }
    }
}

void draw(sf::RenderWindow &window, vector<vector<tile*>> &board, texturemanager &textures, int cols, int rows, bool lose, bool win) {

    window.clear(sf::Color::White);
    const float tileSize = 32.0f;

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            sf::Sprite tileSprite;

            tileSprite.setPosition(i * tileSize, j * tileSize);


            sf::Sprite play;


            sf::Sprite leader;
            leader.setPosition(cols * 32 - 176, 32 * (rows + 0.5));
            leader.setTexture(textures.leaderboard);
            window.draw(leader);

            sf::Sprite face;
            face.setPosition(cols/2.0f * 32 - 32, 32 * (rows + 0.5));
            if (lose) {
                face.setTexture(textures.lose);
            } else if (win){
               face.setTexture(textures.win);
            }
            else{
                face.setTexture(textures.happy);
            }
            window.draw(face);

            sf::FloatRect resetbounds = face.getLocalBounds();
            reset.setSize(sf::Vector2f(resetbounds.width, resetbounds.height));
            reset.setPosition(cols/2.0f * 32 - 32, 32 * (rows + 0.5));

            if (board[i][j]->revealed) {
                if (board[i][j]->mine) {
                    tileSprite.setTexture(textures.mine);
                    sf::Sprite rev(textures.revealed);
                    rev.setPosition(i * tileSize, j * tileSize);
                    window.draw(rev);
                } else {
                    tileSprite.setTexture(textures.revealed);
                }
            } else if (board[i][j]->flag && !board[i][j]->revealed) {
                sf::Sprite flagSprite(textures.flag);
                flagSprite.setPosition(i * tileSize, j * tileSize);
                sf::Sprite rev(textures.hidden);
                rev.setPosition(i * tileSize, j * tileSize);
                window.draw(rev);
                window.draw(flagSprite);


            } else {
                tileSprite.setTexture(textures.hidden);
            }

            window.draw(tileSprite);
            if (board[i][j]->revealed && !board[i][j]->mine) {
                int nearbyCount = board[i][j]->nearby;
                sf::Sprite number;
                number.setTexture(textures.files[to_string(nearbyCount)]);

                float x = i * tileSize + (tileSize - number.getLocalBounds().width) / 2.0f;
                float y = j * tileSize + (tileSize - number.getLocalBounds().height) / 2.0f;
                number.setPosition(x, y);

                window.draw(number);
            }
        }
    }
}

void recursive(vector<vector<tile*>> &board, int x, int y) {
    if (board[x][y]->revealed || board[x][y]->flag) {
        return;
    }

    board[x][y]->revealed = true;

    if (board[x][y]->nearby == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int newX = x + i;
                int newY = y + j;

                if (newX >= 0 && newX < board.size() && newY >= 0 && newY < board[0].size()) {

                    if (!board[newX][newY]->revealed && !board[newX][newY]->flag) {
                        recursive(board, newX, newY);
                    }
                }
            }
        }
    }

}

map<int, sf::Sprite> parseDigits(sf::Sprite digits){
    map<int, sf::Sprite> digitsMap;

    for(int i = 0; i < 11; i++){
        sf::IntRect rect(i*21,0,21,32);
        digits.setTextureRect(rect);
        sf::Sprite sprite = digits;
        digitsMap.emplace(i, sprite);
    }

    return digitsMap;
}

bool winner(const vector<vector<tile*>> &board){
    for (const auto &row : board) {
        for (const auto &tile : row) {
            if (!tile->mine && !tile->revealed) {
                return false;
            }
        }
    }
    return true;
}
