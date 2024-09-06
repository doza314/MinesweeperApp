#include "Minesweeper.hpp"

void startMenu(game& G, bool& over) //Title screen and difficulty prompt
{
    sf::RenderWindow window(sf::VideoMode(400,300), "MINESWEEPER");
    sf::RectangleShape background(sf::Vector2f(400, 300));
    background.setFillColor(sf::Color(255, 255, 255, 128));
    background.setPosition(0, 0);

    sf::Sprite logo;
    sf::Texture logoTexture;
    logoTexture.loadFromFile("src/res/minesweeper.png");
    logo.setTexture(logoTexture);
    logo.setPosition(-50, 0);

    button easy;
    easy.coords = { 150, 100 };
    easy.name = "easy";
    easy.diff = 0;
    easy.textureFile += easy.name + ".png";

    button medium;
    medium.coords = { 150, 160 };
    medium.name = "medium";
    medium.diff = 1;
    medium.textureFile += medium.name + ".png";

    button hard;
    hard.coords = { 150, 220 };
    hard.name = "hard";
    hard.diff = 2;
    hard.textureFile += hard.name + ".png";

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                over = true;
            }

            if (easy.checkClick(window))
            {
                G.setDifficulty(0);
                window.close();
            }
            if (medium.checkClick(window))
            {
                G.setDifficulty(1);
                window.close();
            }
            if (hard.checkClick(window))
            {
                G.setDifficulty(2);
                window.close();
            }
        }
        if(window.isOpen())
        {
            window.clear();
            window.draw(background);
            window.draw(logo);
            easy.drawButton(window);
            medium.drawButton(window);
            hard.drawButton(window);
            window.display();
        }
    }
}

void play(game& G, bool& over)
{
    
    G.emptyBoard();
   
    sf::RenderWindow window(sf::VideoMode(G.getWidth() * 50, G.getHeight() * 50), "MINESWEEPER");
    
    sf::Clock clock;
    clock.restart();
    
    while (window.isOpen())
    {
        sf::Time elapsed = clock.getElapsedTime();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                over = true;
            }            
        }
        

        G.checkClick(window);

        if (G.getOutcome() != 0)
        {
            endMenu(G, window, over, elapsed);
        }
        else 
        {
            G.drawBoard(window);
            window.display();
        }
            
        
    }
}

void endMenu(game& G, sf::RenderWindow& target, bool& over, sf::Time& elapsed)
{
    std::string windowTitle;
    int elapsedMinutes = static_cast<int>(elapsed.asSeconds() / 60);
    int elapsedSeconds = static_cast<int>(elapsed.asSeconds()) % 60;
    std::string timeText = "Time: " + std::to_string(elapsedMinutes) + "m " + std::to_string(elapsedSeconds) + "s";

    sf::Font font;
    font.loadFromFile("src/res/VCR_OSD_MONO_1.001.ttf"); // Replace with your font path
    sf::Text timeTextObject;
    timeTextObject.setFont(font);   
    timeTextObject.setString(timeText);
    timeTextObject.setPosition(90, 100);
    timeTextObject.setFillColor(sf::Color(255, 255, 255, 210));

    if (G.getOutcome() == -1)
    {
        windowTitle = "GAME OVER";
    }
    else
    {
        windowTitle = "YOU WIN!";
    }

    sf::RenderWindow window(sf::VideoMode(400,300), windowTitle);
    sf::Texture gameOverTexture;

    if (G.getOutcome() == -1)
    {   
        gameOverTexture.loadFromFile("src/res/game over.png");
    }
    else
    {
        gameOverTexture.loadFromFile("src/res/win.png");
    }

    sf::RectangleShape background(sf::Vector2f(400, 300));
    background.setFillColor(sf::Color(255, 255, 255, 128));
    background.setPosition(0, 0);

    sf::Sprite gameOver;
    
    gameOver.setTexture(gameOverTexture);
    gameOver.setPosition(-50, 0);

    button restart, exit;

    restart.coords = { 150, 160 };
    restart.diff = 3;
    restart.name = "restart";
    restart.textureFile += restart.name + ".png";

    exit.coords = { 150, 220 };
    exit.diff = 4;
    exit.name = "exit";
    exit.textureFile += exit.name + ".png";

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                over = true;
                target.close();
                window.close();
            }

            if (restart.checkClick(window))
            {
                
                G.reset();
                target.close();
                window.close();

            }
            if (exit.checkClick(window))
            {
                over = true;
                target.close();
                window.close();
            }
        } 
        if (window.isOpen())
        {
        window.clear();

        window.draw(background);
        window.draw(gameOver);
        window.draw(timeTextObject);
        restart.drawButton(window);
        exit.drawButton(window);
        window.display();
        }
    }
}

