#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const sf::Vector2f initialCharacterPosition(100.0f, 500.0f);
const float SPEED = 1.0f;
sf::Text gameOverText;
sf::Text victoireText;

// Dimensions de la fenêtre
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

float clamp(float value, float min, float max) {
    return (value < min) ? min : (value > max ? max : value);
}


class Nuage {
public:
    Nuage(sf::Texture& nuageA, sf::Texture& nuageB)
        : nuageATexture(nuageA),
        nuageBTexture(nuageB) {}

    void spawnNuage() {
        const int nombreDeZones = 5;
        int zoneHauteur = WINDOW_HEIGHT / nombreDeZones;

        int zoneChoisie = rand() % nombreDeZones;

        int positionY = zoneChoisie * zoneHauteur + (rand() % (zoneHauteur - 50)); // Décalage de 50px pour éviter les collisions sur le bord de la zone

        // Placez les nuages
        sf::Sprite nuageA;
        nuageA.setColor(sf::Color(255, 255, 255, 128)); // RGBA (rouge, vert, bleu, opacité)
        if (rand() % 2 == 0)
            nuageA.setTexture(nuageATexture);
        else
            nuageA.setTexture(nuageBTexture);
        nuageA.setPosition(WINDOW_WIDTH + rand() % 200, positionY);
        nuages.push_back(nuageA);

        // Créez le deuxième nuage dans une autre zone aléatoire
        zoneChoisie = rand() % nombreDeZones;
        positionY = zoneChoisie * zoneHauteur + (rand() % (zoneHauteur - 50));

        sf::Sprite nuageB;
        nuageB.setColor(sf::Color(255, 255, 255, 170)); // RGBA (rouge, vert, bleu, opacité)
        if (rand() % 2 == 0)
            nuageB.setTexture(nuageATexture);
        else
            nuageB.setTexture(nuageBTexture);
        nuageB.setPosition(WINDOW_WIDTH + rand() % 200, positionY);
        nuages.push_back(nuageB);
    }


    void updateNuages() {
        for (auto& nuage : nuages) {
            nuage.move(-2 - rand() % 2, 0);
        }

        nuages.erase(
            std::remove_if(nuages.begin(), nuages.end(), [](const sf::Sprite& n) {
                return n.getPosition().x < -n.getGlobalBounds().width;
                }),
            nuages.end()
        );
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& nuage : nuages) {
            window.draw(nuage);
        }
    }
    const std::vector<sf::Sprite>& getNuages() const {
        return nuages;
    }

private:
    sf::Sprite creerNuage(sf::Texture& texture1, sf::Texture& texture2) {
        sf::Sprite nuage;
        if (rand() % 2 == 0)
            nuage.setTexture(texture1);
        else
            nuage.setTexture(texture2);

        nuage.setPosition(rand() % 1920, rand() % 1080);
        return nuage;
    }

    sf::Texture& nuageATexture;
    sf::Texture& nuageBTexture;
    std::vector<sf::Sprite> nuages;
};

class Slider {
public:
    Slider(float x, float y, float width, float height, float initialValue = 0.0f) {
        bar.setSize(sf::Vector2f(width, height));
        bar.setFillColor(sf::Color::White);
        bar.setPosition(x, y);

        knob.setRadius(height / 2);
        knob.setFillColor(sf::Color::Red);
        knob.setOrigin(knob.getRadius(), knob.getRadius());

        // Position initiale du bouton
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;

        float initialX = x + (initialValue / 100.0f) * width;
        knob.setPosition(initialX, y + height / 2);
    }

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (knob.getGlobalBounds().contains(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }))) {
                dragging = true;
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            dragging = false;
        }

        if (dragging && event.type == sf::Event::MouseMoved) {
            float mouseX = window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y }).x;
            float newX = clamp(mouseX, x, x + width);
            knob.setPosition(newX, y + height / 2);
        }
    }

    float getValue() const {
        return (knob.getPosition().x - x) / width * 100.0f; // Retourne la valeur entre 0 et 100
    }

    void draw(sf::RenderWindow& window) {
        window.draw(bar);
        window.draw(knob);
    }

private:
    sf::RectangleShape bar;
    sf::CircleShape knob;
    float x, y, width, height;
    bool dragging = false;
};



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


// Vitesse du joueur et des projectiles
const float PLAYER_SPEED = 5.0f;
const float PROJECTILE_SPEED = 7.0f;
const float ENEMY_SPEED = 1.0f; // Vitesse réduite pour les ennemis
const float BOSS_SPEED = 0.5f;
const float ENEMY_PROJECTILE_SPEED = 3.0f;
const float BOSS_PROJECTILE_SPEED = 5.0f;

const int MAX_ACTIVE_ENEMIES = 4;
const int MAX_ACTIVE_BOSSES = 1;
const int TOTAL_ENEMIES_TO_KILL = 5;
const int TOTAL_BOSS_TO_KILL = 1;

// Fonction pour détecter les collisions
bool checkCollision(const sf::FloatRect& a, const sf::FloatRect& b) {
    return a.intersects(b);
}

int main() {
    // Initialisation de SFML
    sf::RenderWindow window3(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "niveau1");
    sf::RenderWindow window4(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Menu");
    window3.setFramerateLimit(60);
    window4.setFramerateLimit(60);

    Slider effectsSlider(200, 200, 400, 10, 0.0f);
    Slider musicSlider(200, 300, 400, 10, 0.0f);
    Slider ambientSlider(200, 400, 400, 10, 0.0f);

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("assets/nature.wav")) {
        std::cerr << "Erreur lors du chargement du son d'effet\n";
        return -1;
    }
    sf::Sound ambientSound;
    ambientSound.setBuffer(buffer);
    ambientSound.setLoop(true);
    ambientSound.setVolume(0);

    sf::SoundBuffer buffer2;
    if (!buffer2.loadFromFile("assets/runners_sound.wav")) {
        std::cerr << "Erreur lors du chargement du son d'effet\n";
        return -1;
    }
    sf::Sound effectSound;
    effectSound.setBuffer(buffer2);
    effectSound.setLoop(true);
    effectSound.setVolume(0);

    sf::SoundBuffer buffer3;
    if (!buffer3.loadFromFile("assets/stalkers_sound.wav")) {
        std::cerr << "Erreur lors du chargement du son d'effet\n";
        return -1;
    }
    sf::Sound effectSound2;
    effectSound2.setBuffer(buffer3);
    effectSound2.setLoop(true);
    effectSound2.setVolume(0);

    sf::Music music;
    if (!music.openFromFile("assets/electro.ogg")) {
        std::cerr << "Erreur lors du chargement de la musique\n";
        return -1;
    }
    music.setVolume(0);
    music.setLoop(true);

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
    settingsButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, settingsStartY + 2 * (buttonHeight + spacing)), sf::Vector2f(buttonWidth, buttonHeight), "Controles", font);
    settingsButtons.emplace_back(sf::Vector2f((1920 - buttonWidth) / 2, settingsStartY + 3 * (buttonHeight + spacing)), sf::Vector2f(buttonWidth, buttonHeight), "Retour", font);

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

    sf::Text musiqueText;
    musiqueText.setFont(font);
    musiqueText.setCharacterSize(24);
    musiqueText.setFillColor(sf::Color::White);
    musiqueText.setString(
        "Effets :\n\n\n"
        "Musique : \n\n\n\n"
        "Ambiance : "
    );
    musiqueText.setPosition(50, 185);

    sf::Text backButton("Retour", font, 50);
    backButton.setPosition(850, 900);

    sf::Text title2("Choisis ton personnage", font, 50);
    title2.setPosition(700, 50);

    sf::Text title3("Parametres", font, 50);
    title3.setPosition(830, 170);

    sf::Text title5("Difficulte", font, 50);
    title5.setPosition(830, 170);

    std::string niveauDifficulte = "Normal";

    bool isMainMenu = true;
    bool isCharacterSelection = false;
    bool isParametre = false;
    bool isNiveaux = false;
    bool isChoosingDiff = false;
    bool isControles = false;
    bool isMusique = false;
    bool isDragging = false;
    bool ambientPlaying = false;
    bool musicPlaying = false;
    bool effectPlaying = false;
    bool effectPlaying2 = false;
    bool bossSpawned = false;
    bool stalkersSpawned = false;
    bool clickersSpawned = false;

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

    sf::Texture runnersIconTexture;
    if (!runnersIconTexture.loadFromFile("assets/runners.gif")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite runnersIcon(runnersIconTexture);
    runnersIcon.setPosition(10, 300);

    sf::Texture stalkersIconTexture;
    if (!stalkersIconTexture.loadFromFile("assets/stalkers.gif")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite stalkersIcon(stalkersIconTexture);
    stalkersIcon.setPosition(250, 300);

    sf::Texture clickersIconTexture;
    if (!clickersIconTexture.loadFromFile("assets/clickers.gif")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite clickersIcon(clickersIconTexture);
    clickersIcon.setPosition(10, 600);

    std::vector<LevelObject> placedObjects;
    LevelObject* currentObject = nullptr;

    sf::Texture backTexture;
    if (!backTexture.loadFromFile("assets/back.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite backButtonMaker(backTexture);
    backButtonMaker.setPosition(1750, -40);

    // Chargement des textures
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


    sf::Texture nuageATexture, nuageBTexture;
    if (!nuageATexture.loadFromFile("assets/nuageA.png") ||
        !nuageBTexture.loadFromFile("assets/nuageB.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    Nuage nuageManager(nuageATexture, nuageBTexture);

    sf::Texture enemy1Texture;
    if (!enemy1Texture.loadFromFile("assets/runners.gif"))
    {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }

    sf::Texture enemy2Texture;
    if (!enemy2Texture.loadFromFile("assets/stalkers.gif"))
    {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }

    sf::Texture enemy3Texture;
    if (!enemy3Texture.loadFromFile("assets/clickers.gif"))
    {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }

    sf::Texture boss1Texture;
    if (!boss1Texture.loadFromFile("assets/bloater.gif"))
    {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }

    sf::Texture map1Texture;
    if (!map1Texture.loadFromFile("assets/map1.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite map1(map1Texture);

    sf::Texture fond1Texture;
    if (!fond1Texture.loadFromFile("assets/map1.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite fond1(fond1Texture), fond1bis(fond1Texture);
    fond1.setPosition(0, 0);
    fond1bis.setPosition(WINDOW_WIDTH, 0);

    sf::Texture mainMenuTexture;
    if (!mainMenuTexture.loadFromFile("assets/mainMenu.png")) {
        std::cout << "Erreur de chargement de l'image!" << std::endl;
        return -1;
    }
    sf::Sprite mainMenu(mainMenuTexture);

    int playerHealth = 100;

    // Vecteur pour stocker les projectiles du joueur
    std::vector<sf::RectangleShape> playerProjectiles;

    // Structure pour un ennemi
    struct Enemy {
        sf::Sprite sprite;
        int health;
        sf::Vector2f direction;
    };

    struct Boss
    {
        sf::Sprite sprite;
        int health;
        sf::Vector2f direction;
    };

    // Structure pour un projectile
    struct Projectile {
        sf::RectangleShape shape;
        sf::Vector2f direction;
    };

    // Vecteur pour stocker les ennemis
    std::vector<Enemy> runners;
    std::vector<Enemy> stalkers;
    std::vector<Enemy> clickers;
    std::vector<Boss> bosses;

    // Vecteur pour stocker les projectiles des ennemis
    std::vector<Projectile> enemyProjectiles;
    std::vector<Projectile> bossProjectiles;

    // Horloges pour gérer les tirs et les ennemis
    sf::Clock projectileClock;
    sf::Clock enemySpawnClock;
    sf::Clock enemyShootClock;
    sf::Clock bossShootClock;
    sf::Clock spawnClock;

    // Seed pour la génération aléatoire
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    int runnersKilled = 0;
    int stalkersKilled = 0;
    int clickersKilled = 0;
    int bossKilled = 0;
    bool victoire = false;

    while (window4.isOpen())
    {
        sf::Event event4;
        while (window4.pollEvent(event4))
        {
            if (event4.type == sf::Event::Closed)
            {
                window4.close();
                window3.close();
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
                                isParametre = false;
                                isNiveaux = false;
                                isCharacterSelection = true;
                                isChoosingDiff = false;
                                isControles = false;
                                isMusique = false;
                            }
                            if (button.text.getString() == "Quitter")
                            {
                                window4.close();
                                window3.close();
                            }
                            if (button.text.getString() == "Parametres")
                            {
                                isMainMenu = false;
                                isParametre = true;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = false;
                                isMusique = false;
                            }
                            if (button.text.getString() == "Niveaux")
                            {
                                isMainMenu = false;
                                isParametre = false;
                                isNiveaux = true;
                                isChoosingDiff = false;
                                isControles = false;
                                isMusique = false;
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
                        isMusique = false;
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
                                isParametre = false;
                                isNiveaux = false;
                                isChoosingDiff = true;
                                isControles = false;
                                isMusique = false;
                            }
                            if (button.text.getString() == "Controles")
                            {
                                isMainMenu = false;
                                isParametre = false;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = true;
                                isMusique = false;
                            }
                            if (button.text.getString() == "Musique")
                            {
                                isMainMenu = false;
                                isParametre = false;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = false;
                                isMusique = true;
                            }
                            if (button.text.getString() == "Retour")
                            {
                                isMainMenu = true;
                                isParametre = false;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = false;
                                isMusique = false;
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
                    else if (runnersIcon.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        placedObjects.push_back({ sf::Sprite(runnersIconTexture), "Runner" });
                        currentObject = &placedObjects.back();
                        currentObject->sprite.setPosition(mousePos.x, mousePos.y);
                        isDragging = true;
                    }
                    else if (stalkersIcon.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        placedObjects.push_back({ sf::Sprite(stalkersIconTexture), "Stalker" });
                        currentObject = &placedObjects.back();
                        currentObject->sprite.setPosition(mousePos.x, mousePos.y);
                        isDragging = true;
                    }
                    else if (clickersIcon.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        placedObjects.push_back({ sf::Sprite(clickersIconTexture), "Clicker" });
                        currentObject = &placedObjects.back();
                        currentObject->sprite.setPosition(mousePos.x, mousePos.y);
                        isDragging = true;
                    }
                    else if (backButtonMaker.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        isMainMenu = true;
                        isParametre = false;
                        isNiveaux = false;
                        isChoosingDiff = false;
                        isMusique = false;
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
                                isParametre = true;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = false;
                                isMusique = false;
                            }
                            if (button.text.getString() == "Difficile")
                            {
                                isMainMenu = false;
                                isParametre = true;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = false;
                                isMusique = false;
                            }
                            if (button.text.getString() == "Hardcore")
                            {
                                isMainMenu = false;
                                isParametre = true;
                                isNiveaux = false;
                                isChoosingDiff = false;
                                isControles = false;
                                isMusique = false;
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
                    isParametre = true;
                    isNiveaux = false;
                    isChoosingDiff = false;
                    isControles = false;
                    isMusique = false;
                }
            }
            else if (isMusique)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    isMainMenu = false;
                    isParametre = true;
                    isNiveaux = false;
                    isChoosingDiff = false;
                    isControles = false;
                    isMusique = false;
                }

                effectsSlider.handleEvent(event4, window4);
                musicSlider.handleEvent(event4, window4);
                ambientSlider.handleEvent(event4, window4);

                float ambientVolume = ambientSlider.getValue();
                float musicVolume = musicSlider.getValue();
                float effectVolume = effectsSlider.getValue();
                float effectVolume2 = effectsSlider.getValue();

                ambientSound.setVolume(ambientVolume);
                if (ambientVolume > 0 && !ambientPlaying) {
                    ambientSound.play();
                    ambientPlaying = true;
                }
                else if (ambientVolume == 0 && ambientPlaying) {
                    ambientSound.stop();
                    ambientPlaying = false;
                }

                effectSound.setVolume(effectVolume);
                if (effectVolume > 0 && !effectPlaying) {
                    effectSound.play();
                    effectPlaying = true;
                }
                else if (effectVolume == 0 && effectPlaying) {
                    effectSound.stop();
                    effectPlaying = false;
                }

                effectSound2.setVolume(effectVolume2);
                if (effectVolume2 > 0 && !effectPlaying2) {
                    effectSound2.play();
                    effectPlaying2 = true;
                }
                else if (effectVolume2 == 0 && effectPlaying2) {
                    effectSound2.stop();
                    effectPlaying2 = false;
                }

                music.setVolume(musicVolume);
                if (musicVolume > 0 && !musicPlaying) {
                    music.play();
                    musicPlaying = true;
                }
                else if (musicVolume == 0 && musicPlaying) {
                    music.pause();
                    musicPlaying = false;
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
        else if (isParametre)
        {
            window4.draw(title3);
            for (auto& button : settingsButtons)
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
        else if (isNiveaux)
        {
            window4.draw(fond1);
            window4.draw(perso1Icon);
            window4.draw(perso2Icon);
            window4.draw(runnersIcon);
            window4.draw(stalkersIcon);
            window4.draw(clickersIcon);
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
        else if (isMusique)
        {
            window4.draw(musiqueText);
            effectsSlider.draw(window4);
            musicSlider.draw(window4);
            ambientSlider.draw(window4);
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

    sf::RectangleShape healthBar(sf::Vector2f(100, 20)); // Bar de vie (largeur, hauteur)
    healthBar.setFillColor(sf::Color::Green); // Changer la couleur en verte
    healthBar.setPosition(10, WINDOW_HEIGHT - 30);

    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(WINDOW_WIDTH / 2 - gameOverText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2 - gameOverText.getGlobalBounds().height / 2);

    victoireText.setFont(font);
    victoireText.setString("Victoire !");
    victoireText.setCharacterSize(50);
    victoireText.setFillColor(sf::Color::Yellow);
    victoireText.setPosition(WINDOW_WIDTH / 2 - victoireText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2 - victoireText.getGlobalBounds().height / 2);

    if (playerHealth <= 0) {
        gameOverText.setString("Game Over");
    }

    while (window3.isOpen()) {
        sf::Event event3;
        while (window3.pollEvent(event3)) {
            if (event3.type == sf::Event::Closed)
                window3.close();
        }

        // Déplacement du joueur
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && persoActuel->getPosition().x > 0) {
            persoActuel->move(-PLAYER_SPEED, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && persoActuel->getPosition().x + persoActuel->getGlobalBounds().width < WINDOW_WIDTH) {
            persoActuel->move(PLAYER_SPEED, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && persoActuel->getPosition().y > 0) {
            persoActuel->move(0, -PLAYER_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && persoActuel->getPosition().y + persoActuel->getGlobalBounds().height < WINDOW_HEIGHT) {
            persoActuel->move(0, PLAYER_SPEED);
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && projectileClock.getElapsedTime().asMilliseconds() > 300) {
            // Calcul de la direction du tir vers la souris
            sf::Vector2f mousePosition = window3.mapPixelToCoords(sf::Mouse::getPosition(window3));
            sf::Vector2f playerPosition = persoActuel->getPosition() + sf::Vector2f(persoActuel->getGlobalBounds().width / 2, persoActuel->getGlobalBounds().height / 2);
            sf::Vector2f direction = mousePosition - playerPosition;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length != 0) direction /= length;

            // Création du projectile
            sf::RectangleShape projectile(sf::Vector2f(10, 10));
            projectile.setFillColor(sf::Color::Red);
            projectile.setPosition(playerPosition);

            playerProjectiles.push_back(projectile);
            projectileClock.restart();
        }

        while (runners.size() < MAX_ACTIVE_ENEMIES && runnersKilled < TOTAL_ENEMIES_TO_KILL) {
            sf::Sprite enemy1Sprite;
            enemy1Sprite.setTexture(enemy1Texture);

            float xPosition = WINDOW_WIDTH - enemy1Sprite.getGlobalBounds().width - (std::rand() % 200);
            float yPosition = static_cast<float>(std::rand() % (WINDOW_HEIGHT - static_cast<int>(enemy1Sprite.getGlobalBounds().height)));
            enemy1Sprite.setPosition(xPosition, yPosition);

            // Initialisation de la direction aléatoire pour l'ennemi
            sf::Vector2f randomDirection(static_cast<float>(std::rand() % 3 - 1), static_cast<float>(std::rand() % 3 - 1));
            if (randomDirection.x == 0 && randomDirection.y == 0) randomDirection.x = 1; // Assurer qu'il y a toujours une direction

            runners.push_back({ enemy1Sprite, 5, randomDirection });
        }

        if (!stalkersSpawned && runnersKilled >= TOTAL_ENEMIES_TO_KILL && runners.empty()) {
            while (stalkers.size() < MAX_ACTIVE_ENEMIES && stalkersKilled < TOTAL_ENEMIES_TO_KILL)
            {
                sf::Sprite enemy2Sprite;
                enemy2Sprite.setTexture(enemy2Texture);

                float xPosition = WINDOW_WIDTH - enemy2Sprite.getGlobalBounds().width - (std::rand() % 200);
                float yPosition = static_cast<float>(std::rand() % (WINDOW_HEIGHT - static_cast<int>(enemy2Sprite.getGlobalBounds().height)));
                enemy2Sprite.setPosition(xPosition, yPosition);

                // Initialisation de la direction aléatoire pour l'ennemi
                sf::Vector2f randomDirection(static_cast<float>(std::rand() % 3 - 1), static_cast<float>(std::rand() % 3 - 1));
                if (randomDirection.x == 0 && randomDirection.y == 0) randomDirection.x = 1; // Assurer qu'il y a toujours une direction

                stalkers.push_back({ enemy2Sprite, 7, randomDirection });
            }
        }

        if (!clickersSpawned && stalkersKilled >= TOTAL_ENEMIES_TO_KILL && stalkers.empty()) {
            while (clickers.size() < MAX_ACTIVE_ENEMIES && clickersKilled < TOTAL_ENEMIES_TO_KILL)
            {
                sf::Sprite enemy3Sprite;
                enemy3Sprite.setTexture(enemy3Texture);

                float xPosition = WINDOW_WIDTH - enemy3Sprite.getGlobalBounds().width - (std::rand() % 200);
                float yPosition = static_cast<float>(std::rand() % (WINDOW_HEIGHT - static_cast<int>(enemy3Sprite.getGlobalBounds().height)));
                enemy3Sprite.setPosition(xPosition, yPosition);

                // Initialisation de la direction aléatoire pour l'ennemi
                sf::Vector2f randomDirection(static_cast<float>(std::rand() % 3 - 1), static_cast<float>(std::rand() % 3 - 1));
                if (randomDirection.x == 0 && randomDirection.y == 0) randomDirection.x = 1; // Assurer qu'il y a toujours une direction

                clickers.push_back({ enemy3Sprite, 10, randomDirection });
            }
        }

        healthBar.setSize(sf::Vector2f(playerHealth * 10.0f, 20.0f));

        // Déplacement des projectiles
        for (auto& projectile : playerProjectiles) {
            projectile.move(PROJECTILE_SPEED, 0);
        }

        for (auto& projectile : enemyProjectiles) {
            projectile.shape.move(projectile.direction * ENEMY_PROJECTILE_SPEED);
        }

        // Supprimer les projectiles hors de l'écran
        playerProjectiles.erase(
            std::remove_if(playerProjectiles.begin(), playerProjectiles.end(), [](const sf::RectangleShape& p) {
                return p.getPosition().x > WINDOW_WIDTH || p.getPosition().x < 0 || p.getPosition().y < 0 || p.getPosition().y > WINDOW_HEIGHT;
                }),
            playerProjectiles.end()
        );

        enemyProjectiles.erase(
            std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(), [](const Projectile& p) {
                return p.shape.getPosition().x + p.shape.getSize().x < 0 || p.shape.getPosition().y < 0 || p.shape.getPosition().y > WINDOW_HEIGHT;
                }),
            enemyProjectiles.end()
        );

        // Déplacement des ennemis aléatoires
        for (auto& enemy1 : runners) {
            // Mouvement aléatoire
            enemy1.sprite.move(enemy1.direction * ENEMY_SPEED);

            // Réfléchir s'il atteint un bord
            if (enemy1.sprite.getPosition().x < 0 || enemy1.sprite.getPosition().x + enemy1.sprite.getGlobalBounds().width > WINDOW_WIDTH) {
                enemy1.direction.x = -enemy1.direction.x;
            }
            if (enemy1.sprite.getPosition().y < 0 || enemy1.sprite.getPosition().y + enemy1.sprite.getGlobalBounds().height > WINDOW_HEIGHT) {
                enemy1.direction.y = -enemy1.direction.y;
            }

            // Gestion des tirs des ennemis vers le joueur
            if (enemyShootClock.getElapsedTime().asMilliseconds() > 1000) {
                sf::RectangleShape projectile(sf::Vector2f(10, 10));
                projectile.setFillColor(sf::Color::Green);
                projectile.setPosition(enemy1.sprite.getPosition() + sf::Vector2f(enemy1.sprite.getGlobalBounds().width / 2, enemy1.sprite.getGlobalBounds().height / 2));
                sf::Vector2f enemy1Pos = enemy1.sprite.getPosition() + sf::Vector2f(enemy1.sprite.getGlobalBounds().width / 2, enemy1.sprite.getGlobalBounds().height / 2);
                sf::Vector2f playerPos = persoActuel->getPosition() + sf::Vector2f(persoActuel->getGlobalBounds().width / 2, persoActuel->getGlobalBounds().height / 2);
                sf::Vector2f direction = playerPos - enemy1Pos;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length != 0) direction /= length;

                enemyProjectiles.push_back({ projectile, direction });
                enemyShootClock.restart();
            }
        }

        for (auto& enemy2 : stalkers) {
            // Mouvement aléatoire
            enemy2.sprite.move(enemy2.direction * ENEMY_SPEED);

            // Réfléchir s'il atteint un bord
            if (enemy2.sprite.getPosition().x < 0 || enemy2.sprite.getPosition().x + enemy2.sprite.getGlobalBounds().width > WINDOW_WIDTH) {
                enemy2.direction.x = -enemy2.direction.x;
            }
            if (enemy2.sprite.getPosition().y < 0 || enemy2.sprite.getPosition().y + enemy2.sprite.getGlobalBounds().height > WINDOW_HEIGHT) {
                enemy2.direction.y = -enemy2.direction.y;
            }

            // Gestion des tirs des ennemis vers le joueur
            if (enemyShootClock.getElapsedTime().asMilliseconds() > 1000) {
                sf::RectangleShape projectile(sf::Vector2f(10, 10));
                projectile.setFillColor(sf::Color::Green);
                projectile.setPosition(enemy2.sprite.getPosition() + sf::Vector2f(enemy2.sprite.getGlobalBounds().width / 2, enemy2.sprite.getGlobalBounds().height / 2));
                sf::Vector2f enemy2Pos = enemy2.sprite.getPosition() + sf::Vector2f(enemy2.sprite.getGlobalBounds().width / 2, enemy2.sprite.getGlobalBounds().height / 2);
                sf::Vector2f playerPos = persoActuel->getPosition() + sf::Vector2f(persoActuel->getGlobalBounds().width / 2, persoActuel->getGlobalBounds().height / 2);
                sf::Vector2f direction = playerPos - enemy2Pos;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length != 0) direction /= length;

                enemyProjectiles.push_back({ projectile, direction });
                enemyShootClock.restart();
            }
        }

        for (auto& enemy3 : clickers) {
            // Mouvement aléatoire
            enemy3.sprite.move(enemy3.direction * ENEMY_SPEED);

            // Réfléchir s'il atteint un bord
            if (enemy3.sprite.getPosition().x < 0 || enemy3.sprite.getPosition().x + enemy3.sprite.getGlobalBounds().width > WINDOW_WIDTH) {
                enemy3.direction.x = -enemy3.direction.x;
            }
            if (enemy3.sprite.getPosition().y < 0 || enemy3.sprite.getPosition().y + enemy3.sprite.getGlobalBounds().height > WINDOW_HEIGHT) {
                enemy3.direction.y = -enemy3.direction.y;
            }

            // Gestion des tirs des ennemis vers le joueur
            if (enemyShootClock.getElapsedTime().asMilliseconds() > 1000) {
                sf::RectangleShape projectile(sf::Vector2f(10, 10));
                projectile.setFillColor(sf::Color::Green);
                projectile.setPosition(enemy3.sprite.getPosition() + sf::Vector2f(enemy3.sprite.getGlobalBounds().width / 2, enemy3.sprite.getGlobalBounds().height / 2));
                sf::Vector2f enemy3Pos = enemy3.sprite.getPosition() + sf::Vector2f(enemy3.sprite.getGlobalBounds().width / 2, enemy3.sprite.getGlobalBounds().height / 2);
                sf::Vector2f playerPos = persoActuel->getPosition() + sf::Vector2f(persoActuel->getGlobalBounds().width / 2, persoActuel->getGlobalBounds().height / 2);
                sf::Vector2f direction = playerPos - enemy3Pos;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length != 0) direction /= length;

                enemyProjectiles.push_back({ projectile, direction });
                enemyShootClock.restart();
            }
        }

        // Mise à jour de la barre de vie du joueur
        healthBar.setSize(sf::Vector2f(playerHealth * 10.0f, 20.0f));

        // Vérification des collisions projectiles-ennemis
        for (auto it1 = playerProjectiles.begin(); it1 != playerProjectiles.end(); ) {
            bool hit1 = false;
            for (auto enemy1It = runners.begin(); enemy1It != runners.end(); ) {
                if (checkCollision(it1->getGlobalBounds(), enemy1It->sprite.getGlobalBounds())) {
                    enemy1It->health -= 1;
                    if (enemy1It->health <= 0) {
                        enemy1It = runners.erase(enemy1It);
                        ++runnersKilled;
                    }
                    else {
                        ++enemy1It;
                    }
                    it1 = playerProjectiles.erase(it1);
                    hit1 = true;
                    break;
                }
                else {
                    ++enemy1It;
                }
            }
            if (!hit1) {
                ++it1;
            }
        }

        for (auto it2 = playerProjectiles.begin(); it2 != playerProjectiles.end(); ) {
            bool hit2 = false;
            for (auto enemy2It = stalkers.begin(); enemy2It != stalkers.end(); ) {
                if (checkCollision(it2->getGlobalBounds(), enemy2It->sprite.getGlobalBounds())) {
                    enemy2It->health -= 1;
                    if (enemy2It->health <= 0) {
                        enemy2It = stalkers.erase(enemy2It);
                        ++stalkersKilled;
                    }
                    else {
                        ++enemy2It;
                    }
                    it2 = playerProjectiles.erase(it2);
                    hit2 = true;
                    break;
                }
                else {
                    ++enemy2It;
                }
            }
            if (!hit2) {
                ++it2;
            }
        }

        for (auto it3 = playerProjectiles.begin(); it3 != playerProjectiles.end(); ) {
            bool hit3 = false;
            for (auto enemy3It = clickers.begin(); enemy3It != clickers.end(); ) {
                if (checkCollision(it3->getGlobalBounds(), enemy3It->sprite.getGlobalBounds())) {
                    enemy3It->health -= 1;
                    if (enemy3It->health <= 0) {
                        enemy3It = clickers.erase(enemy3It);
                        ++clickersKilled;
                    }
                    else {
                        ++enemy3It;
                    }
                    it3 = playerProjectiles.erase(it3);
                    hit3 = true;
                    break;
                }
                else {
                    ++enemy3It;
                }
            }
            if (!hit3) {
                ++it3;
            }
        }

        // Vérification des collisions projectiles-joueur
        for (auto it = enemyProjectiles.begin(); it != enemyProjectiles.end(); ) {
            if (checkCollision(it->shape.getGlobalBounds(), persoActuel->getGlobalBounds())) {
                playerHealth -= 1;
                it = enemyProjectiles.erase(it);
            }
            else {
                ++it;
            }
        }

        if (!bossSpawned && clickersKilled >= TOTAL_ENEMIES_TO_KILL && clickers.empty()) {
            sf::Sprite bossSprite;
            bossSprite.setTexture(boss1Texture);

            float xPosition = WINDOW_WIDTH - bossSprite.getGlobalBounds().width - (std::rand() % 200);
            float yPosition = static_cast<float>(std::rand() % (WINDOW_HEIGHT - static_cast<int>(bossSprite.getGlobalBounds().height)));
            bossSprite.setPosition(xPosition, yPosition);

            // Initialisation de la direction aléatoire pour le boss
            sf::Vector2f randomDirection(static_cast<float>(std::rand() % 3 - 1), static_cast<float>(std::rand() % 3 - 1));
            if (randomDirection.x == 0 && randomDirection.y == 0) randomDirection.x = 1; // Assurer qu'il y a toujours une direction

            bosses.push_back({ bossSprite, 15, randomDirection });
            bossSpawned = true; // Le boss a été spawn, ne pas le refaire
        }

        healthBar.setSize(sf::Vector2f(playerHealth * 10.0f, 20.0f));

        // Déplacement des projectiles
        for (auto& projectile : playerProjectiles) {
            projectile.move(PROJECTILE_SPEED, 0);
        }

        for (auto& projectile : bossProjectiles) {
            projectile.shape.move(projectile.direction * BOSS_PROJECTILE_SPEED);
        }

        // Supprimer les projectiles hors de l'écran
        playerProjectiles.erase(
            std::remove_if(playerProjectiles.begin(), playerProjectiles.end(), [](const sf::RectangleShape& p) {
                return p.getPosition().x > WINDOW_WIDTH || p.getPosition().x < 0 || p.getPosition().y < 0 || p.getPosition().y > WINDOW_HEIGHT;
                }),
            playerProjectiles.end()
        );

        bossProjectiles.erase(
            std::remove_if(bossProjectiles.begin(), bossProjectiles.end(), [](const Projectile& p) {
                return p.shape.getPosition().x + p.shape.getSize().x < 0 || p.shape.getPosition().y < 0 || p.shape.getPosition().y > WINDOW_HEIGHT;
                }),
            bossProjectiles.end()
        );

        // Déplacement du boss aléatoires
        for (auto& boss : bosses) {
            // Mouvement aléatoire
            boss.sprite.move(boss.direction * BOSS_SPEED);

            // Réfléchir s'il atteint un bord
            if (boss.sprite.getPosition().x < 0 || boss.sprite.getPosition().x + boss.sprite.getGlobalBounds().width > WINDOW_WIDTH) {
                boss.direction.x = -boss.direction.x;
            }
            if (boss.sprite.getPosition().y < 0 || boss.sprite.getPosition().y + boss.sprite.getGlobalBounds().height > WINDOW_HEIGHT) {
                boss.direction.y = -boss.direction.y;
            }

            // Gestion des tirs des ennemis vers le joueur
            if (bossShootClock.getElapsedTime().asMilliseconds() > 1000) {
                const int numProjectiles = 7;       // Nombre total de projectiles dans l'arc
                const float arcAngle = 90.0f;       // Angle total de l'arc en degrés
                const float angleStep = arcAngle / (numProjectiles - 1); // Espace entre les projectiles

                sf::Vector2f bossPos = boss.sprite.getPosition() +
                    sf::Vector2f(boss.sprite.getGlobalBounds().width / 2,
                        boss.sprite.getGlobalBounds().height / 2);

                sf::Vector2f playerPos = persoActuel->getPosition() +
                    sf::Vector2f(persoActuel->getGlobalBounds().width / 2,
                        persoActuel->getGlobalBounds().height / 2);

                sf::Vector2f directionToPlayer = playerPos - bossPos;
                float baseAngle = atan2(directionToPlayer.y, directionToPlayer.x) * 180.0f / M_PI;

                // Créer chaque projectile dans l'arc
                for (int i = 0; i < numProjectiles; ++i) {
                    float angle = baseAngle - (arcAngle / 2.0f) + (i * angleStep); // Angle pour ce projectile

                    // Convertir l'angle en radians pour calculer la direction
                    float radians = angle * M_PI / 180.0f;
                    sf::Vector2f direction(cos(radians), sin(radians));

                    // Initialiser le projectile
                    sf::RectangleShape projectile(sf::Vector2f(10, 10));
                    projectile.setFillColor(sf::Color::Green);
                    projectile.setPosition(bossPos);

                    // Ajouter le projectile avec sa direction dans le vecteur
                    bossProjectiles.push_back({ projectile, direction });
                }

                bossShootClock.restart();
            }
        }

        // Mise à jour de la barre de vie du joueur
        healthBar.setSize(sf::Vector2f(playerHealth * 10.0f, 20.0f));

        // Vérification des collisions projectiles-ennemis
        for (auto it = playerProjectiles.begin(); it != playerProjectiles.end(); ) {
            bool hit = false;
            for (auto bossIt = bosses.begin(); bossIt != bosses.end(); ) {
                if (checkCollision(it->getGlobalBounds(), bossIt->sprite.getGlobalBounds())) {
                    bossIt->health -= 1;
                    if (bossIt->health <= 0) {
                        bossIt = bosses.erase(bossIt);
                        ++bossKilled;
                    }
                    else {
                        ++bossIt;
                    }
                    it = playerProjectiles.erase(it);
                    hit = true;
                    break;
                }
                else {
                    ++bossIt;
                }
            }
            if (!hit) {
                ++it;
            }
        }

        // Vérification des collisions projectiles-joueur
        for (auto it = bossProjectiles.begin(); it != bossProjectiles.end(); ) {
            if (checkCollision(it->shape.getGlobalBounds(), persoActuel->getGlobalBounds())) {
                playerHealth -= 1;
                it = bossProjectiles.erase(it);
            }
            else {
                ++it;
            }
        }
        if (bossKilled >= TOTAL_BOSS_TO_KILL) {
            victoire = true;
        }
        // Vérification de la santé du joueur
        if (playerHealth <= 0) {
            gameOverText.setString("Game Over");
        }

        fond1.move(-SPEED, 0);
        fond1bis.move(-SPEED, 0);

        if (fond1.getPosition().x + WINDOW_WIDTH < 0)
            fond1.setPosition(WINDOW_WIDTH - 1, 0);
        if (fond1bis.getPosition().x + WINDOW_WIDTH < 0)
            fond1bis.setPosition(WINDOW_WIDTH - 1, 0);
        if (spawnClock.getElapsedTime().asSeconds() > 2.0f) {
            nuageManager.spawnNuage();
            spawnClock.restart();
        }

        // Affichage
        window3.clear();
        nuageManager.updateNuages();
        window3.draw(fond1);
        window3.draw(fond1bis);
        window3.draw(*persoActuel);
        window3.draw(healthBar);
        for (const auto& projectile : playerProjectiles) {
            window3.draw(projectile);
        }
        for (const auto& enemy1 : runners) {
            window3.draw(enemy1.sprite);
        }
        for (const auto& enemy2 : stalkers) {
            window3.draw(enemy2.sprite);
        }
        for (const auto& enemy3 : clickers) {
            window3.draw(enemy3.sprite);
        }
        for (const auto& projectile : enemyProjectiles) {
            window3.draw(projectile.shape);
        }
        for (const auto& projectile : bossProjectiles) {
            window3.draw(projectile.shape);
        }
        for (const auto& boss : bosses) {
            window3.draw(boss.sprite);
        }
        nuageManager.draw(window3);
        if (playerHealth <= 0) {
            window3.clear();
            window3.draw(map1);
            // Afficher le fond semi-transparent
            sf::RectangleShape semiTransparentRect(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            semiTransparentRect.setFillColor(sf::Color(0, 0, 0, 150)); // Noir semi-transparent
            window3.draw(semiTransparentRect);

            // Afficher "Game Over"
            window3.draw(gameOverText);

            sf::Text instructionText;
            instructionText.setFont(font);
            instructionText.setString("Appuyez sur ECHAP pour quitter");
            instructionText.setCharacterSize(20);
            instructionText.setFillColor(sf::Color::White);
            instructionText.setPosition(WINDOW_WIDTH / 2 - instructionText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2 + 50);
            window3.draw(instructionText);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window3.close();
            }
        }
        else if (victoire) {
            window3.clear();

            // Afficher le fond semi-transparent
            sf::RectangleShape semiTransparentRect(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            semiTransparentRect.setFillColor(sf::Color(0, 0, 0, 150)); // Noir semi-transparent
            window3.draw(semiTransparentRect);

            // Afficher "Victoire !"
            window3.draw(victoireText);

            sf::Text instructionText;
            instructionText.setFont(font);
            instructionText.setString("Appuyez sur ECHAP pour quitter");
            instructionText.setCharacterSize(20);
            instructionText.setFillColor(sf::Color::White);
            instructionText.setPosition(WINDOW_WIDTH / 2 - instructionText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2 + 50);
            window3.draw(instructionText);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window3.close();
            }
        }
        
        window3.display();
    }
    return 0;
}
