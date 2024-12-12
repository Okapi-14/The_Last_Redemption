#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath> // Pour std::abs
int windowHauteur = 1080;
int windowLargeur = 1920;
const sf::Vector2f initialCharacterPosition(100.0f, 500.0f);

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    bool isHovered;

    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& label, sf::Font& font) {
        shape.setPosition(position);
        shape.setSize(size);
        shape.setFillColor(sf::Color(139, 0, 0)); // Rouge foncé

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);

        // Centrer le texte dans le bouton
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            position.x + (size.x - textBounds.width) / 2 - textBounds.left,
            position.y + (size.y - textBounds.height) / 2 - textBounds.top
        );

        isHovered = false;
    }

    void update(const sf::Vector2i& mousePos) {
        if (shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            if (!isHovered) {
                shape.setFillColor(sf::Color::Black); // Couleur noire quand survolé
                isHovered = true;
            }
        }
        else {
            if (isHovered) {
                shape.setFillColor(sf::Color(139, 0, 0)); // Retour au rouge foncé
                isHovered = false;
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
};


struct LevelObject
{
    sf::Sprite sprite;
    std::string type;
};

struct Projectile {
    sf::RectangleShape shape;
    sf::Vector2f velocity;

    Projectile(sf::Vector2f startPosition, sf::Vector2f direction) {
        shape.setSize(sf::Vector2f(10.0f, 10.0f));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(startPosition);
        velocity = direction * 2.0f;
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
    sf::RenderWindow window1(sf::VideoMode(windowLargeur, windowHauteur), "niveau1");
    sf::RenderWindow window2(sf::VideoMode(windowLargeur, windowHauteur), "niveau 2");
    sf::RenderWindow window3(sf::VideoMode(windowLargeur, windowHauteur), "niveau 1");
    sf::RenderWindow window4(sf::VideoMode(windowLargeur, windowHauteur), "Menu");

    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf"))
    {
        std::cerr << "Erreur de chargement de la police!\n";
        return -1;
    }

    std::vector<Button> mainButtons;
    std::vector<Button> settingsButtons;
    std::vector<Button> diffButtons;
    bool isInSettings = false;

    const float buttonWidth = 300;
    const float buttonHeight = 70;
    const float startY = 400; // Position verticale de départ pour les boutons
    const float spacing = 20; // Espacement entre les boutons

    // Boutons principaux
    mainButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, startY), sf::Vector2f(buttonWidth, buttonHeight), "Jouer", font);
    mainButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, startY + buttonHeight + spacing), sf::Vector2f(buttonWidth, buttonHeight), "Parametres", font);
    mainButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, startY + 2 * (buttonHeight + spacing)), sf::Vector2f(buttonWidth, buttonHeight), "Niveaux", font);
    mainButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, startY + 3 * (buttonHeight + spacing)), sf::Vector2f(buttonWidth, buttonHeight), "Quitter", font);

    // Boutons des paramètres
    const float settingsStartY = 300;
    settingsButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, settingsStartY), sf::Vector2f(buttonWidth, buttonHeight), "Difficulte", font);
    settingsButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, settingsStartY + buttonHeight + spacing), sf::Vector2f(buttonWidth, buttonHeight), "Musique", font);
    settingsButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, settingsStartY + 2 * (buttonHeight + spacing)), sf::Vector2f(buttonWidth, buttonHeight), "Video", font);
    settingsButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, settingsStartY + 3 * (buttonHeight + spacing)), sf::Vector2f(buttonWidth, buttonHeight), "Controles", font);
    settingsButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, settingsStartY + 4 * (buttonHeight + spacing)), sf::Vector2f(buttonWidth, buttonHeight), "Retour", font);

    const float diffStartY = 300;
    diffButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, settingsStartY), sf::Vector2f(buttonWidth, buttonHeight), "Normal", font);
    diffButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, settingsStartY + 2 * (buttonHeight + spacing)), sf::Vector2f(buttonWidth, buttonHeight), "Difficile", font);
    diffButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, settingsStartY + 4 * (buttonHeight + spacing)), sf::Vector2f(buttonWidth, buttonHeight), "Hardcore", font);

    sf::Text controlsText;
    controlsText.setFont(font);
    controlsText.setCharacterSize(24);
    controlsText.setFillColor(sf::Color::White);
    controlsText.setString(
        "Z : Haut\n\n"
        "Q : Gauche\n\n"
        "S : Bas\n\n"
        "D : Droite\n\n"
        "Clic Gauche : Tirer\n\n"
        "Clic Droit : Tirs speciaux\n\n"
        "A : Changer d'items\n\n"
        "E : Changer d'items speciaux"
    );
    controlsText.setPosition(100, 200);

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

    sf::Text title3("Parametres", font, 50);
    title3.setPosition(830, 170);

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
    title5.setPosition(830, 170);

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
    bool isControles = false;
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
                    for (auto& button : mainButtons)
                    {
                        if (button.shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))if (button.shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                        {
                            if (button.text.getString() == "Jouer")
                            {
                                isMainMenu = false;
                                isCharacterSelection = true;
                                isParametre = false;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = false;
                            }
                            if (button.text.getString() == "Quitter")
                            {
                                window4.close();
                                window3.close();
                                window2.close();
                                window1.close();
                            }
                            if (button.text.getString() == "Parametres")
                            {
                                isMainMenu = false;
                                isCharacterSelection = false;
                                isParametre = true;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = false;
                            }
                            if (button.text.getString() == "Niveaux")
                            {
                                isMainMenu = false;
                                isCharacterSelection = false;
                                isParametre = false;
                                isNiveaux = true;
                                isChoosingDiff = false;
                                isControles = false;
                            }
                        }
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
                        isControles = false;
                    }
                }
            }
            else if (isParametre)
            {
                if (event4.type == sf::Event::MouseButtonPressed && event4.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window4);
                    for (auto& button : settingsButtons)
                    {
                        if (button.shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                        {
                            if (button.text.getString() == "Difficulte")
                            {
                                isMainMenu = false;
                                isCharacterSelection = false;
                                isParametre = false;
                                isNiveaux = false;
                                isChoosingDiff = true;
                                isControles = false;
                            }
                            if (button.text.getString() == "Controles")
                            {
                                isMainMenu = false;
                                isCharacterSelection = false;
                                isParametre = false;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = true;
                            }
                            if (button.text.getString() == "Retour")
                            {
                                isMainMenu = true;
                                isCharacterSelection = false;
                                isParametre = false;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = false;
                            }
                        }
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
                    for (auto& button : diffButtons)
                    {
                        if (button.shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                        {
                            if (button.text.getString() == "Normal")
                            {
                                isMainMenu = false;
                                isCharacterSelection = false;
                                isParametre = true;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = false;
                            }
                            if (button.text.getString() == "Difficile")
                            {
                                isMainMenu = false;
                                isCharacterSelection = false;
                                isParametre = true;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = false;
                            }
                            if (button.text.getString() == "Hardcore")
                            {
                                isMainMenu = false;
                                isCharacterSelection = false;
                                isParametre = true;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = false;
                            }
                        }
                    }
                }
            }
            else if (isControles)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
                {
                    isMainMenu = false;
                    isCharacterSelection = false;
                    isParametre = true;
                    isNiveaux = false;
                    isChoosingDiff = false;
                    isControles = false;
                }
            }
        }
        sf::Vector2i mousePos = sf::Mouse::getPosition(window4);
        for (auto& button : mainButtons) 
        {
            button.update(mousePos);
        }

        window4.clear();

        if (isMainMenu)
        {
            window4.draw(mainMenu);

            for (auto& button : mainButtons)
            {
                button.draw(window4);
            }
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
            for (auto& button : settingsButtons)
            {
                button.draw(window4);
            }
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
            for (auto& button : diffButtons)
            {
                button.draw(window4);
            }
        }
        else if (isControles)
        {
            window4.draw(controlsText);
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
