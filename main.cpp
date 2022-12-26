// Copyright [2022] <Tim Truong>
#include "sokoban.h"

const int FR_LIMIT = 120;
const int WIDTH_RANGE = 64;
const int HEIGHT_RANGE = 64;

int main(int argc, char* argv[]) {
    if (argc > 2 || argc < 2) {
        std::cout << "Not the correct number of command line arguments" << std::endl;
        exit(1);
    } else {
        Sokoban sokoban;
        sf::Clock clock;

        std::string file_name = argv[1];
        std::ifstream file(file_name);
        file >> sokoban;

        sf::RenderWindow window;
        window.create(sf::VideoMode(sokoban.getMapWidth() * WIDTH_RANGE, sokoban.getMapHeight() * HEIGHT_RANGE), "Sokoban");
        window.setFramerateLimit(FR_LIMIT);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    case sf::Event::KeyPressed:
                        switch (event.key.code) {
                            case sf::Keyboard::W:
                            case sf::Keyboard::Up:
                                sokoban.movePlayer(up);
                                break;
                            case sf::Keyboard::A:
                            case sf::Keyboard::Left:
                                sokoban.movePlayer(left);
                                break;
                            case sf::Keyboard::S:
                            case sf::Keyboard::Down:
                                sokoban.movePlayer(down);
                                break;
                            case sf::Keyboard::D:
                            case sf::Keyboard::Right:
                                sokoban.movePlayer(right);
                                break;
                            case sf::Keyboard::R:
                                sokoban.reset();
                                break;
                            case sf::Keyboard::Z:
                            case sf::Keyboard::Backspace:
                                sokoban.undo();
                                break;
                            case sf::Keyboard::Escape:
                                window.close();
                                break;
                            default:
                                break;
                        }
                    std::cout << sokoban << std::endl;
                    default:
                        break;
                }
            }

            sf::Time time;
            int seconds = clock.getElapsedTime().asSeconds();
            int minutes = seconds / 60;
            seconds -= minutes * 60;
            std::string timeStr;

            if (minutes < 10) {
                timeStr = '0';
            }

            if (seconds < 10) {
                timeStr += std::to_string(minutes) + ":0" + std::to_string(seconds);
            } else {
                timeStr += std::to_string(minutes) + ":" + std::to_string(seconds);
            }

            sf::Font font;
            font.loadFromFile("arial.ttf");
            sf::Text text(timeStr, font);
            text.setScale(0.85, 0.85);
            text.setPosition(sf::Vector2f(5, 15));

            // Win Msg
            sf::Text winText("Congratulations! You Won!", font);
            winText.setCharacterSize(32);
            winText.setStyle(sf::Text::Bold);
            winText.setPosition(sf::Vector2f(128, 128));

            // Drawing the map and sprites
            window.clear();
            window.draw(sokoban);
            window.draw(text);
            if (sokoban.Won()) {
                window.draw(winText);
            }
            window.display();
        }

        return 0;
    }
}
