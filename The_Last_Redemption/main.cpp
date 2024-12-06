#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath> // Pour std::abs
int windowHauteur = 1080;
int windowLargeur = 1920;

int main() {
	sf::RenderWindow window1(sf::VideoMode(windowLargeur, windowHauteur), "test 3");
	sf::RenderWindow window2(sf::VideoMode(windowLargeur, windowHauteur), "test 2");
	sf::RenderWindow window3(sf::VideoMode(windowLargeur, windowHauteur), "test 1");

    sf::Texture fond1Texture;
    if (!fond1Texture.loadFromFile("assets/map1.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite fond1(fond1Texture);

    sf::Texture fond2Texture;
    if (!fond2Texture.loadFromFile("assets/map2.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite fond2(fond2Texture);

    sf::Texture fond3Texture;
    if (!fond3Texture.loadFromFile("assets/map3.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite fond3(fond3Texture);

    while (window3.isOpen()) {
        sf::Event event3;
        while (window3.pollEvent(event3)) {
            if (event3.type == sf::Event::Closed)
                window3.close();

            if (event3.type == sf::Event::KeyPressed) {
                if (event3.key.code == sf::Keyboard::Enter) {
                    window3.close();

                }
            }
        }
        window3.clear();
        window3.draw(fond1);
        window3.display();
    }

    while (window2.isOpen()) {
        sf::Event event2;
        while (window2.pollEvent(event2)) {
            if (event2.type == sf::Event::Closed)
                window2.close();

            if (event2.type == sf::Event::KeyPressed) {
                if (event2.key.code == sf::Keyboard::Enter) {
                    window2.close();

                }
            }
        }
        window2.clear();
        window2.draw(fond2);
        window2.display();
    }

    while (window1.isOpen()) {
        sf::Event event1;
        while (window1.pollEvent(event1)) {
            if (event1.type == sf::Event::Closed)
                window1.close();

            if (event1.type == sf::Event::KeyPressed) {
                if (event1.key.code == sf::Keyboard::Enter) {
                    window1.close();

                }
            }
        }
        window1.clear();
        window1.draw(fond3);
        window1.display();
    }

    return 0;
}