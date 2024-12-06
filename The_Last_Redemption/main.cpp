#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath> // Pour std::abs
int windowHauteur = 1080;
int windowLargeur = 1920;

int main()
{
    sf::RenderWindow window1(sf::VideoMode(windowLargeur, windowHauteur), "test 3");
    sf::RenderWindow window2(sf::VideoMode(windowLargeur, windowHauteur), "test 2");
    sf::RenderWindow window3(sf::VideoMode(windowLargeur, windowHauteur), "test 1");
    sf::RenderWindow window4(sf::VideoMode(windowLargeur, windowHauteur), "Menu");

    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf"))
    {
        std::cerr << "Erreur de chargement de la police!\n";
        return -1;
    }

    sf::Text title("Menu Principal", font, 50);
    title.setPosition(800, 50);

    sf::Texture startTexture;
    if (!startTexture.loadFromFile("assets/start.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite start(startTexture);
    start.setPosition(750, 0);
    sf::Vector2f startPosition = start.getPosition();

    sf::Texture paramTexture;
    if (!paramTexture.loadFromFile("assets/param.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite param(paramTexture);
    param.setPosition(750, 200);
    sf::Vector2f paramPosition = param.getPosition();

    sf::Texture quitTexture;
    if (!quitTexture.loadFromFile("assets/quitter.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite quit(quitTexture);
    quit.setPosition(750, 600);
    sf::Vector2f quitPosition = quit.getPosition();

    sf::Texture perso1Texture;
    if (!perso1Texture.loadFromFile("assets/Ellie.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite perso1(perso1Texture);
    perso1.setPosition(200, 250);
    sf::Vector2f perso1Position = perso1.getPosition();

    sf::Texture perso2Texture;
    if (!perso2Texture.loadFromFile("assets/Arthur.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite perso2(perso2Texture);
    perso2.setPosition(500, 250);
    sf::Vector2f perso2Position = perso2.getPosition();

    sf::Sprite* persoChoisis = nullptr;

    sf::Text backButton("Retour", font, 30);
    backButton.setPosition(300, 600);

    bool isMainMenu = true;
    bool isCharacterSelection = false;

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

    sf::Vector2i startPos;

    while (window4.isOpen())
    {
        sf::Event event4;
        while (window4.pollEvent(event4))
        {
            if (event4.type == sf::Event::Closed)
            {
                window4.close();
                window3.close();
                window2.close();
                window1.close();
            }
                
            if (isMainMenu)
            {
                if (event4.type == sf::Event::MouseButtonPressed && event4.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window4);
                    if (start.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        isMainMenu = false;
                        isCharacterSelection = true;
                    }
                    if (quit.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        window4.close();
                        window3.close();
                        window2.close();
                        window1.close();
                    }
                    if (param.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {

                    }
                }
            }
            else if (isCharacterSelection)
            {
                if (event4.type == sf::Event::MouseButtonPressed && event4.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window4);
                    if (perso1.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        std::cout << "Ellie est selectionne\n";
                        persoChoisis = &perso1;
                        window4.close();
                    }
                    if (perso2.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        std::cout << "Arthur est selectionne\n";
                        persoChoisis = &perso2;
                        window4.close();
                    }
                    if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        isMainMenu = true;
                        isCharacterSelection = false;
                    }
                }
            }
        }
        window4.clear();
        
        if (isMainMenu)
        {
            window4.draw(title);
            window4.draw(start);
            window4.draw(param);
            window4.draw(quit);
        }
        else if (isCharacterSelection)
        {
            window4.draw(perso1);
            window4.draw(perso2);
            window4.draw(backButton);
        }
        window4.display();
    }

    sf::Sprite* persoActuel = nullptr;
    if (persoChoisis == &perso1) 
    {
        persoActuel = &perso1;
    }
    else if (persoChoisis == &perso2)
    {
        persoActuel = &perso2;
    }

    while (window3.isOpen()) {
        sf::Event event3;
        while (window3.pollEvent(event3))
        {
            if (event3.type == sf::Event::Closed)
                window3.close();

            if (event3.type == sf::Event::KeyPressed) 
            {
                if (event3.key.code == sf::Keyboard::Enter) window3.close();
                if (event3.key.code == sf::Keyboard::Q) persoActuel->move(-5.0f, 0.0f);
                if (event3.key.code == sf::Keyboard::D) persoActuel->move(5.0f, 0.0f);
                if (event3.key.code == sf::Keyboard::Z) persoActuel->move(0.0f, -5.0f);
                if (event3.key.code == sf::Keyboard::S) persoActuel->move(0.0f, 5.0f);
                /*if (event3.key.code == sf::Keyboard::A)
                if (event3.key.code == sf::Keyboard::E)*/
            }

            /*if (event3.type == sf::Event::MouseButtonPressed)
            {
                if (event3.mouseButton.button == sf::Mouse::Left)
                {

                }
            }

            if (event3.type == sf::Event::MouseButtonPressed)
            {
                if (event3.mouseButton.button == sf::Mouse::Right)
                {

                }
            }*/
        }
       
        window3.clear();
        window3.draw(fond1);
        window3.draw(*persoActuel);
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

            if (event2.type == sf::Event::KeyPressed) 
            {
                if (event2.key.code == sf::Keyboard::Enter) window2.close();
                if (event2.key.code == sf::Keyboard::Q) persoActuel->move(-5.0f, 0.0f);
                if (event2.key.code == sf::Keyboard::D) persoActuel->move(5.0f, 0.0f);
                if (event2.key.code == sf::Keyboard::Z) persoActuel->move(0.0f, -5.0f);
                if (event2.key.code == sf::Keyboard::S) persoActuel->move(0.0f, 5.0f);
                /*if (event2.key.code == sf::Keyboard::A)
                if (event2.key.code == sf::Keyboard::E)*/
            }

            /*if (event2.type == sf::Event::MouseButtonPressed)
            {
                if (event2.mouseButton.button == sf::Mouse::Left)
                {

                }
            }

            if (event2.type == sf::Event::MouseButtonPressed)
            {
                if (event2.mouseButton.button == sf::Mouse::Right)
                {

                }
            }*/
        }
        
        window2.clear();
        window2.draw(fond2);
        window2.draw(*persoActuel);
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

            if (event1.type == sf::Event::KeyPressed) 
            {
                if (event1.key.code == sf::Keyboard::Enter) window1.close();
                if (event1.key.code == sf::Keyboard::Q) persoActuel->move(-5.0f, 0.0f);
                if (event1.key.code == sf::Keyboard::D) persoActuel->move(5.0f, 0.0f);
                if (event1.key.code == sf::Keyboard::Z) persoActuel->move(0.0f, -5.0f);
                if (event1.key.code == sf::Keyboard::S) persoActuel->move(0.0f, 5.0f);
                /*if (event1.key.code == sf::Keyboard::A)
                if (event1.key.code == sf::Keyboard::E)*/
            }

            /*if (event1.type == sf::Event::MouseButtonPressed)
            {
                if (event1.mouseButton.button == sf::Mouse::Left)
                {

                }
            }

            if (event1.type == sf::Event::MouseButtonPressed)
            {
                if (event1.mouseButton.button == sf::Mouse::Right)
                {

                }
            }*/
        }

        window1.clear();
        window1.draw(fond3);
        window1.draw(*persoActuel);
        window1.display();
    }

    return 0;
}
