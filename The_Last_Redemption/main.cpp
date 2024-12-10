#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath> // Pour std::abs
int windowHauteur = 1080;
int windowLargeur = 1920;
const sf::Vector2f initialCharacterPosition(100.0f, 500.0f);
struct LevelObject
{
    sf::Sprite sprite;
    std::string type;
};

struct Projectile {
    sf::RectangleShape shape;
    sf::Vector2f velocity;

    Projectile(sf::Vector2f startPosition, sf::Vector2f direction) {
        shape.setSize(sf::Vector2f(10.0f, 10.0f)); // Taille du carré
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(startPosition);
        velocity = direction * 3.0f; // Vitesse plus lente du projectile
    }

    void update(float deltaTime) {
        shape.move(velocity * deltaTime);
    }

    bool isOffScreen(int windowWidth, int windowHeight) {
        sf::Vector2f pos = shape.getPosition();
        return pos.x < 0 || pos.x > windowWidth || pos.y < 0 || pos.y > windowHeight;
    }
};

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

    sf::Texture startTexture;
    if (!startTexture.loadFromFile("assets/start.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite start(startTexture);
    start.setPosition(750, 100);
    sf::Vector2f startPosition = start.getPosition();

    sf::Texture paramTexture;
    if (!paramTexture.loadFromFile("assets/param.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite param(paramTexture);
    param.setPosition(750, 300);
    sf::Vector2f paramPosition = param.getPosition();

    sf::Texture nivTexture;
    if (!nivTexture.loadFromFile("assets/niv.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite niv(nivTexture);
    niv.setPosition(750, 500);
    sf::Vector2f nivPosition = niv.getPosition();

    sf::Texture quitTexture;
    if (!quitTexture.loadFromFile("assets/quitter.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite quit(quitTexture);
    quit.setPosition(750, 700);
    sf::Vector2f quitPosition = quit.getPosition();

    sf::Texture perso1Texture;
    if (!perso1Texture.loadFromFile("assets/Ellie.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite perso1(perso1Texture);
    perso1.setPosition(600, 250);
    sf::Vector2f perso1Position = perso1.getPosition();

    sf::Texture perso2Texture;
    if (!perso2Texture.loadFromFile("assets/Arthur.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite perso2(perso2Texture);
    perso2.setPosition(1000, 250);
    sf::Vector2f perso2Position = perso2.getPosition();

    sf::Sprite* persoChoisis = nullptr;

    sf::Text backButton("Retour", font, 50);
    backButton.setPosition(850, 900);

    sf::Text title2("Choisis ton personnage", font, 50);
    title2.setPosition(700, 50);

    sf::Text title3("Parametre", font, 50);
    title3.setPosition(800, 50);

    sf::Texture diffTexture;
    if (!diffTexture.loadFromFile("assets/diff.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite diff(diffTexture);
    diff.setPosition(650, -30);
    sf::Vector2f diffPosition = diff.getPosition();

    sf::Texture musicTexture;
    if (!musicTexture.loadFromFile("assets/music.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite music(musicTexture);
    music.setPosition(650, 130);
    sf::Vector2f musicPosition = music.getPosition();

    sf::Texture videoTexture;
    if (!videoTexture.loadFromFile("assets/video.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite video(videoTexture);
    video.setPosition(650, 290);
    sf::Vector2f videoPosition = video.getPosition();

    sf::Texture controlsTexture;
    if (!controlsTexture.loadFromFile("assets/controls.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite controls(controlsTexture);
    controls.setPosition(650, 450);
    sf::Vector2f controlsPosition = controls.getPosition();

    sf::Texture languageTexture;
    if (!languageTexture.loadFromFile("assets/language.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite language(languageTexture);
    language.setPosition(650, 610);
    sf::Vector2f languagePosition = language.getPosition();

    sf::Text title4("Niveaux", font, 50);
    title4.setPosition(850, 50);

    sf::Text title5("Difficulte", font, 50);
    title5.setPosition(850, 50);

    sf::Text niv1t("Niveau 1", font, 30);
    niv1t.setPosition(500, 250);

    sf::Text niv2t("Niveau 2", font, 30);
    niv2t.setPosition(850, 250);

    sf::Text niv3t("Niveau 3", font, 30);
    niv3t.setPosition(1250, 250);

    std::string niveauDifficulte = "Normal";

    sf::Texture normalTexture;
    if (!normalTexture.loadFromFile("assets/normal.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite normal(normalTexture);
    normal.setPosition(650, -30);
    sf::Vector2f normalPosition = normal.getPosition();

    sf::Texture difficileTexture;
    if (!difficileTexture.loadFromFile("assets/difficile.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite difficile(difficileTexture);
    difficile.setPosition(650, 200);
    sf::Vector2f difficilePosition = difficile.getPosition();

    sf::Texture hardcoreTexture;
    if (!hardcoreTexture.loadFromFile("assets/hardcore.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite hardcore(hardcoreTexture);
    hardcore.setPosition(650, 430);
    sf::Vector2f hardcorePosition = hardcore.getPosition();

    sf::Texture niv1Texture;
    if (!niv1Texture.loadFromFile("assets/niv1.gif")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite niv1(niv1Texture);
    niv1.setPosition(400, 350);
    sf::Vector2f niv1Position = niv1.getPosition();

    sf::Texture niv2Texture;
    if (!niv2Texture.loadFromFile("assets/niv2.gif")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite niv2(niv2Texture);
    niv2.setPosition(770, 350);
    sf::Vector2f niv2Position = niv2.getPosition();

    sf::Texture niv3Texture;
    if (!niv3Texture.loadFromFile("assets/niv3.gif")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite niv3(niv3Texture);
    niv3.setPosition(1150, 350);
    sf::Vector2f niv3Position = niv3.getPosition();

    bool isMainMenu = true;
    bool isCharacterSelection = false;
    bool isParametre = false;
    bool isNiveaux = false;
    bool isChoosingDiff = false;
    bool isDragging = false;

    sf::Texture persoETexture;
    if (!persoETexture.loadFromFile("assets/Ellie.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite perso1Icon(persoETexture);
    perso1Icon.setPosition(-40, 0);

    sf::Texture persoATexture;
    if (!persoATexture.loadFromFile("assets/Arthur.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite perso2Icon(persoATexture);
    perso2Icon.setPosition(170, 0);

    sf::Texture runnersTexture;
    if (!runnersTexture.loadFromFile("assets/runners.gif")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite runners(runnersTexture);
    runners.setPosition(10, 300);

    sf::Texture stalkersTexture;
    if (!stalkersTexture.loadFromFile("assets/stalkers.gif")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite stalkers(stalkersTexture);
    stalkers.setPosition(250, 300);

    sf::Texture clickersTexture;
    if (!clickersTexture.loadFromFile("assets/clickers.gif")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite clickers(clickersTexture);
    clickers.setPosition(10, 600);

    std::vector<LevelObject> placedObjects;
    LevelObject* currentObject = nullptr;

    sf::Texture backTexture;
    if (!backTexture.loadFromFile("assets/back.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite backButtonMaker(backTexture);
    backButtonMaker.setPosition(1750, -40);

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

    sf::Texture mainMenuTexture;
    if (!mainMenuTexture.loadFromFile("assets/mainMenu.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite mainMenu(mainMenuTexture);

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
                        isParametre = false;
                        isNiveaux = false;
                        isChoosingDiff = false;
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
                        isMainMenu = false;
                        isCharacterSelection = false;
                        isParametre = true;
                        isNiveaux = false;
                        isChoosingDiff = false;
                    }
                    if (niv.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        isMainMenu = false;
                        isCharacterSelection = false;
                        isParametre = false;
                        isNiveaux = true;
                        isChoosingDiff = false;
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
                        isParametre = false;
                        isNiveaux = false;
                        isChoosingDiff = false;
                    }
                }
            }
            else if (isParametre)
            {
                if (event4.type == sf::Event::MouseButtonPressed && event4.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window4);
                    if (diff.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        isMainMenu = false;
                        isCharacterSelection = false;
                        isParametre = false;
                        isNiveaux = false;
                        isChoosingDiff = true;
                    }
                    if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        isMainMenu = true;
                        isCharacterSelection = false;
                        isParametre = false;
                        isNiveaux = false;
                        isChoosingDiff = false;
                    }
                }
            }
            else if (isNiveaux)
            {
                if (event4.type == sf::Event::MouseButtonPressed && event4.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window4);
                    if (perso1Icon.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        placedObjects.push_back({ sf::Sprite(persoETexture), "Ellie" });
                        currentObject = &placedObjects.back();
                        currentObject->sprite.setPosition(mousePos.x, mousePos.y);
                        isDragging = true;
                    }
                    else if (perso2Icon.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        placedObjects.push_back({ sf::Sprite(persoATexture), "Arthur" });
                        currentObject = &placedObjects.back();
                        currentObject->sprite.setPosition(mousePos.x, mousePos.y);
                        isDragging = true;
                    }
                    else if (runners.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        placedObjects.push_back({ sf::Sprite(runnersTexture), "Runner" });
                        currentObject = &placedObjects.back();
                        currentObject->sprite.setPosition(mousePos.x, mousePos.y);
                        isDragging = true;
                    }
                    else if (stalkers.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        placedObjects.push_back({ sf::Sprite(stalkersTexture), "Stalker" });
                        currentObject = &placedObjects.back();
                        currentObject->sprite.setPosition(mousePos.x, mousePos.y);
                        isDragging = true;
                    }
                    else if (clickers.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        placedObjects.push_back({ sf::Sprite(clickersTexture), "Clicker" });
                        currentObject = &placedObjects.back();
                        currentObject->sprite.setPosition(mousePos.x, mousePos.y);
                        isDragging = true;
                    }
                    else if (backButtonMaker.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        isMainMenu = true;
                        isCharacterSelection = false;
                        isParametre = false;
                        isNiveaux = false;
                        isChoosingDiff = false;
                    }
                }
                if (event4.type == sf::Event::MouseButtonReleased && event4.mouseButton.button == sf::Mouse::Left)
                {
                    isDragging = false;
                    currentObject = nullptr;
                }
                else if (event4.type == sf::Event::MouseMoved && isDragging)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window4);
                    if (currentObject)
                    {
                        currentObject->sprite.setPosition(mousePos.x, mousePos.y);
                    }
                }
            }
            else if (isChoosingDiff)
            {
                if (event4.type == sf::Event::MouseButtonPressed && event4.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window4);
                    if (normal.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        niveauDifficulte = "Normal";
                        std::cout << "Difficulte selectionne : Normal\n";
                        isMainMenu = false;
                        isCharacterSelection = false;
                        isParametre = true;
                        isNiveaux = false;
                        isChoosingDiff = false;
                    }
                    else if (difficile.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        niveauDifficulte = "Difficle";
                        std::cout << "Difficulte selectionne : Difficile\n";
                        isMainMenu = false;
                        isCharacterSelection = false;
                        isParametre = true;
                        isNiveaux = false;
                        isChoosingDiff = false;
                    }
                    else if (hardcore.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        niveauDifficulte = "Hardcore";
                        std::cout << "Difficulte selectionne : Hardcore\n";
                        isMainMenu = false;
                        isCharacterSelection = false;
                        isParametre = true;
                        isNiveaux = false;
                        isChoosingDiff = false;
                    }
                    else if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        isMainMenu = false;
                        isCharacterSelection = false;
                        isParametre = true;
                        isNiveaux = false;
                        isChoosingDiff = false;
                    }
                }
            }
        }
        window4.clear();

        if (isMainMenu)
        {
            window4.draw(mainMenu);
            window4.draw(start);
            window4.draw(param);
            window4.draw(niv);
            window4.draw(quit);
        }
        else if (isCharacterSelection)
        {
            window4.draw(title2);
            window4.draw(perso1);
            window4.draw(perso2);
            window4.draw(backButton);
        }
        else if (isParametre)
        {
            window4.draw(title3);
            window4.draw(diff);
            window4.draw(music);
            window4.draw(video);
            window4.draw(controls);
            window4.draw(language);
            window4.draw(backButton);
        }
        else if (isNiveaux)
        {
            window4.draw(fond1);
            window4.draw(perso1Icon);
            window4.draw(perso2Icon);
            window4.draw(runners);
            window4.draw(stalkers);
            window4.draw(clickers);
            window4.draw(backButtonMaker);
            for (const auto& obj : placedObjects)
            {
                window4.draw(obj.sprite);
            }
        }
        else if (isChoosingDiff)
        {
            window4.draw(title5);
            window4.draw(normal);
            window4.draw(difficile);
            window4.draw(hardcore);
            window4.draw(backButton);
        }
        window4.display();
    }

    sf::Sprite* persoActuel = nullptr;
    if (persoChoisis == &perso1)
    {
        persoChoisis->setPosition(initialCharacterPosition);
        persoActuel = &perso1;
    }
    else if (persoChoisis == &perso2)
    {
        persoChoisis->setPosition(initialCharacterPosition);
        persoActuel = &perso2;
    }

    std::vector<Projectile> projectiles;
    float cooldownTime = 0.1f;
    float timeSinceLastShot = cooldownTime;

    while (window3.isOpen()) {
        sf::Event event3;
        while (window3.pollEvent(event3))
        {
            if (event3.type == sf::Event::Closed)
            {
                window3.close();
                window2.close();
                window1.close();
            }

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

            if (event3.type == sf::Event::MouseButtonPressed)
            {
                if (event3.mouseButton.button == sf::Mouse::Left && timeSinceLastShot >= cooldownTime)
                {
                    sf::Vector2f direction(1.0f, 0.0f);
                    projectiles.emplace_back(persoActuel->getPosition(), direction);
                    timeSinceLastShot = 0.0f;
                }
            }

            /*if (event3.type == sf::Event::MouseButtonPressed)
            {
                if (event3.mouseButton.button == sf::Mouse::Right)
                {

                }
            }*/
        }
        float deltaTime = 0.1f;
        timeSinceLastShot += deltaTime;

        for (auto& proj : projectiles) {
            proj.update(deltaTime);
        }
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
            [&](Projectile& p) { return p.isOffScreen(windowLargeur, windowHauteur); }),
            projectiles.end());

        window3.clear();
        window3.draw(fond1);
        window3.draw(*persoActuel);
        for (auto& proj : projectiles) {
            window3.draw(proj.shape);
        }
        window3.display();
    }

    while (window2.isOpen()) {
        sf::Event event2;
        while (window2.pollEvent(event2)) {
            if (event2.type == sf::Event::Closed)
            {
                window2.close();
                window1.close();
            }

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

            if (event2.type == sf::Event::MouseButtonPressed)
            {
                if (event2.mouseButton.button == sf::Mouse::Left && timeSinceLastShot >= cooldownTime)
                {
                    sf::Vector2f direction(1.0f, 0.0f);
                    projectiles.emplace_back(persoActuel->getPosition(), direction);
                    timeSinceLastShot = 0.0f;
                }
            }

            /*if (event2.type == sf::Event::MouseButtonPressed)
            {
                if (event2.mouseButton.button == sf::Mouse::Right)
                {

                }
            }*/
        }

        float deltaTime = 0.1f;
        timeSinceLastShot += deltaTime;

        for (auto& proj : projectiles) {
            proj.update(deltaTime);
        }
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
            [&](Projectile& p) { return p.isOffScreen(windowLargeur, windowHauteur); }),
            projectiles.end());

        window2.clear();
        window2.draw(fond2);
        window2.draw(*persoActuel);
        for (auto& proj : projectiles) {
            window2.draw(proj.shape);
        }
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

            if (event1.type == sf::Event::MouseButtonPressed)
            {
                if (event1.mouseButton.button == sf::Mouse::Left && timeSinceLastShot >= cooldownTime)
                {
                    sf::Vector2f direction(1.0f, 0.0f);
                    projectiles.emplace_back(persoActuel->getPosition(), direction);
                    timeSinceLastShot = 0.0f;
                }
            }

            /*if (event1.type == sf::Event::MouseButtonPressed)
            {
                if (event1.mouseButton.button == sf::Mouse::Right)
                {

                }
            }*/
        }
        float deltaTime = 0.1f;
        timeSinceLastShot += deltaTime;

        for (auto& proj : projectiles) {
            proj.update(deltaTime);
        }
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
            [&](Projectile& p) { return p.isOffScreen(windowLargeur, windowHauteur); }),
            projectiles.end());

        window1.clear();
        window1.draw(fond3);
        window1.draw(*persoActuel);
        for (auto& proj : projectiles) {
            window1.draw(proj.shape);
        }
        window1.display();
    }

    return 0;
}
