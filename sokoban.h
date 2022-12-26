// Copyright [2022] <Tim Truong>
#ifndef SOKOBAN_H_
#define SOKOBAN_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

typedef enum {up, left, down, right} direction;

class Sokoban: public sf::Drawable {
    friend std::istream& operator>>(std::istream& in, Sokoban& rhs);
    friend std::ostream& operator<<(std::ostream& out, Sokoban& rhs);

 public:
    Sokoban();
    void movePlayer(direction d);
    void undo();
    void reset();
    bool Won();
    void updateMovement(int& NextR, int& NextR2, int& NextC,
    int& NextC2, int curRow, int curCol, sf::Texture& playerSprite, direction d);
    void updateDrawing(sf::Sprite& sprite, sf::Texture texture, sf::RenderTarget& target, int x, int y) const;
    void updateDrawing(sf::Sprite& sprite, sf::Sprite& sprite2, sf::Texture texture,
    sf::Texture texture2, sf::RenderTarget& target, int x, int y) const;
    int getMapWidth(void) const;
    int getMapHeight(void) const;

 private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    std::vector<std::vector<char>> lvldata;
    std::vector<std::vector<std::vector<char>>> lvlhist;
    int moveCnt = 0;
    int mapWidth = 0;
    int mapHeight = 0;
    sf::Texture block_06;
    sf::Texture crate_03;
    sf::Texture environment_03;
    sf::Texture ground_01;
    sf::Texture ground_04;
    sf::Texture player_05;
    sf::Texture player_08;
    sf::Texture player_17;
    sf::Texture player_20;
    sf::Texture playerTexture;
};

#endif  // SOKOBAN_H_
