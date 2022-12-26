// Copyright [2022] <Tim Truong>
#include "sokoban.h"
#include "status.h"

const int WIDTH_RANGE = 64;
const int HEIGHT_RANGE = 64;

Sokoban::Sokoban(void) {
    block_06.loadFromFile("Images/block_06.png");
    crate_03.loadFromFile("Images/crate_03.png");
    environment_03.loadFromFile("Images/environment_03.png");
    ground_01.loadFromFile("Images/ground_01.png");
    ground_04.loadFromFile("Images/ground_04.png");
    player_05.loadFromFile("Images/player_05.png");
    player_08.loadFromFile("Images/player_08.png");
    player_17.loadFromFile("Images/player_17.png");
    player_20.loadFromFile("Images/player_20.png");

    playerTexture = player_05;
}

void Sokoban::movePlayer(direction d) {
    // if player won, we do nothing
    if (Won()) {
        return;
    } else {
        for (size_t row = 0; row < lvldata.size() - 1; ++row) {
            for (size_t col = 0; col < lvldata[col].size() - 1; ++col) {
                if (lvldata[row][col] == '@' || lvldata[row][col] == '!') {
                    char change_tile;
                    // Determines if player is moving on either on a normal or end zone tile
                    // if thats the case, we change the tile we recently moved from based on where we
                    // we're located
                    if (lvldata[row][col] == '@')
                        change_tile = '.';
                    else
                        change_tile = '*';

                    int NextRow, NextRow2;
                    int NextCol, NextCol2;

                    updateMovement(NextRow, NextRow2, NextCol, NextCol2, row, col, playerTexture, d);

                    // if next row and col is a wall that we were walking towards we prevent movement
                    if (lvldata[NextRow][NextCol] == '#') {
                        break;
                    // if next row and col is a box, we move the player and box to that direction
                    } else if (lvldata[NextRow][NextCol] == 'O') {
                        if (lvldata[NextRow2][NextCol2] == '.') {
                            lvldata[NextRow2][NextCol2] = 'O';
                            lvldata[NextRow][NextCol] = '@';
                            lvldata[row][col] = change_tile;
                        } else if (lvldata[NextRow2][NextCol2] == '*') {
                            lvldata[NextRow2][NextCol2] = '$';
                            lvldata[NextRow][NextCol] = '@';
                            lvldata[row][col] = change_tile;
                        }
                    // if next row and col is a end zone, change tile we left from accordingly
                    } else if (lvldata[NextRow][NextCol] == '*') {
                        lvldata[NextRow][NextCol] = '!';
                        lvldata[row][col] = change_tile;
                    } else if (lvldata[NextRow][NextCol] == '$') {
                        if (lvldata[NextRow2][NextCol2] == '.') {
                            lvldata[NextRow2][NextCol2] = 'O';
                            lvldata[NextRow][NextCol] = '!';
                            lvldata[row][col] = change_tile;
                        }
                    // otherwise if its open space, move player to that particular direction
                    // and change tiles accordingly
                    } else {
                        lvldata[NextRow][NextCol] = '@';
                        lvldata[row][col] = change_tile;
                    }

                    lvlhist.push_back(lvldata);
                    moveCnt++;

                    return;
                }
            }
        }
    }
}

void Sokoban::updateMovement(int& NextR, int& NextR2, int& NextC,
int& NextC2, int curRow, int curCol, sf::Texture& playerSprite, direction d) {
    if (d == up) {
        NextR = curRow - 1;
        NextR2 = curRow - 2;
        NextC = curCol;
        NextC2 = curCol;
        playerSprite = player_08;
    } else if (d == down) {
        NextR = curRow + 1;
        NextR2 = curRow + 2;
        NextC = curCol;
        NextC2 = curCol;
        playerSprite = player_05;
    } else if (d == right) {
        NextR = curRow;
        NextR2 = curRow;
        NextC = curCol + 1;
        NextC2 = curCol + 2;
        playerSprite = player_17;
    } else if (d == left) {
        NextR = curRow;
        NextR2 = curRow;
        NextC = curCol - 1;
        NextC2 = curCol - 2;
        playerSprite = player_20;
    } else {}

    return;
}

void Sokoban::updateDrawing(sf::Sprite& sprite, sf::Texture texture,
sf::RenderTarget& target, int x, int y) const {
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(x, y));
    target.draw(sprite);

    return;
}

void Sokoban::updateDrawing(sf::Sprite& sprite, sf::Sprite& sprite2, sf::Texture texture,
sf::Texture texture2, sf::RenderTarget& target, int x, int y) const {
    sprite.setTexture(texture);
    sprite2.setTexture(texture2);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite2.setPosition(sf::Vector2f(x, y));
    target.draw(sprite);
    target.draw(sprite2);

    return;
}

void Sokoban::undo(void) {
    if (moveCnt > 0) {
        lvldata = lvlhist[moveCnt - 1];
        lvlhist.pop_back();
        moveCnt--;
    }

    return;
}

void Sokoban::reset(void) {
    while (lvlhist.size() != 1) {
        lvlhist.pop_back();
    }
    lvldata = lvlhist[0];
    playerTexture = player_05;
    moveCnt = 0;

    return;
}

bool Sokoban::Won(void) {
    int bCnt = 0, zCnt = 0;
    for (auto row : lvldata) {
        int bAmt = std::count_if(row.begin(), row.end(), [](char c) { return c == 'O'; });
        int zAmt = std::count_if(row.begin(), row.end(), [](char c) { return (c == '*' || c == '!'); });
        bCnt += bAmt;
        zCnt += zAmt;
    }

    if (bCnt == 0 && zCnt == 0)
        return TRUE;
    else
        return FALSE;
}

int Sokoban::getMapWidth(void) const {
    return mapWidth;
}

int Sokoban::getMapHeight(void) const {
    return mapHeight;
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Sprite sprite;
    sf::Sprite sprite2;
    int x = 0, y = 0;

    for (size_t row = 0; row < lvldata.size(); ++row) {
        for (size_t col = 0; col < lvldata[col].size(); ++col) {
            if (lvldata[row][col] == '#') {
                sprite.setTexture(block_06);
                sprite.setPosition(sf::Vector2f(x, y));
                target.draw(sprite);
            } else if (lvldata[row][col] == '@') {
                sprite.setTexture(ground_01);
                sprite2.setTexture(playerTexture);
                sprite.setPosition(sf::Vector2f(x, y));
                sprite2.setPosition(sf::Vector2f(x, y));
                target.draw(sprite);
                target.draw(sprite2);
            } else if (lvldata[row][col] == '.') {
                sprite.setTexture(ground_01);
                sprite.setPosition(sf::Vector2f(x, y));
                target.draw(sprite);
            } else if (lvldata[row][col] == 'O') {
                sprite.setTexture(ground_01);
                sprite2.setTexture(crate_03);
                sprite.setPosition(sf::Vector2f(x, y));
                sprite2.setPosition(sf::Vector2f(x, y));
                target.draw(sprite);
                target.draw(sprite2);
            } else if (lvldata[row][col] == '*') {
                sprite.setTexture(ground_04);
                sprite.setPosition(sf::Vector2f(x, y));
                target.draw(sprite);
            } else if (lvldata[row][col] == '$') {
                sprite.setTexture(ground_04);
                sprite2.setTexture(crate_03);
                sprite.setPosition(sf::Vector2f(x, y));
                sprite2.setPosition(sf::Vector2f(x, y));
                target.draw(sprite);
                target.draw(sprite2);
            } else if (lvldata[row][col] == '!') {
                sprite.setTexture(ground_04);
                sprite2.setTexture(playerTexture);
                sprite.setPosition(sf::Vector2f(x, y));
                sprite2.setPosition(sf::Vector2f(x, y));
                target.draw(sprite);
                target.draw(sprite2);
            }
            x += WIDTH_RANGE;
        }
        y += HEIGHT_RANGE;
        x = 0;
    }
}

std::istream& operator>>(std::istream& in, Sokoban& rhs) {
    std::string str;
    std::string width;
    std::string height;

    std::getline(in, str);
    width = (str.substr(0, str.find(' ')));
    rhs.mapWidth = stoi(width);
    height = (str.substr(str.find(' '), str.find('\n')));
    rhs.mapHeight = stoi(height);

    while (std::getline(in, str)) {
        rhs.lvldata.push_back(std::vector<char>(str.begin(), str.end()));
    }

    rhs.lvlhist.push_back(rhs.lvldata);

    return in;
}

std::ostream& operator<<(std::ostream& out, Sokoban& rhs) {
    for (size_t x = 0; x < rhs.lvldata.size(); ++x) {
        for (size_t y = 0; y < rhs.lvldata[x].size(); ++y) {
            std::cout << rhs.lvldata[x][y];
        }
        std::cout << std::endl;
    }

    return out;
}
