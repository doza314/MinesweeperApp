#include "include/SFML/Graphics.hpp"
#include <iostream>
#include <cmath>
#include <random>
#include <iomanip>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>

class game
    {
        private:
            int outcome = 0;
            int difficulty = 0;
            int mines = 10;
            int leftClicks = 0;
            std::vector<std::vector<int>> safe_zone ={{0,0}, {0,0}, {0,0},
                                                      {0,0}, {0,0}, {0,0},
                                                      {0,0}, {0,0}, {0,0}}; 
            std::vector<std::vector<int>> mine_coords;
            int size[2] = {9, 9};
            int cellSize = 50;
            std::vector<std::vector<int>> toUncover;
            std::vector<std::vector<int>> board;
            std::vector<std::vector<sf::Texture>> textures;
            std::vector<std::vector<sf::Sprite>> sprites;
            std::vector<std::vector<std::vector<int>>> cell_coords;
            std::vector<std::vector<bool>> revealed;
            std::vector<std::vector<bool>> flags;

            std::vector<int> clickedCoords;

            void generateSafeZone(std::vector<int> coords) //Generates 3x3 safe zone with no bombs based on first click on board
            {
                safe_zone[0][0] = coords[0] - 1;
                safe_zone[0][1] = coords[1] - 1;
                
                safe_zone[1][0] = coords[0];
                safe_zone[1][1] = coords[1] - 1;

                safe_zone[2][0] = coords[0] + 1;
                safe_zone[2][1] = coords[1] - 1;

                safe_zone[3][0] = coords[0] - 1;
                safe_zone[3][1] = coords[1];

                safe_zone[4] = coords;

                safe_zone[5][0] = coords[0] + 1;
                safe_zone[5][1] = coords[1];

                safe_zone[6][0] = coords[0] - 1;
                safe_zone[6][1] = coords[1] + 1;

                safe_zone[7][0] = coords[0];
                safe_zone[7][1] = coords[1] + 1;

                safe_zone[8][0] = coords[0] + 1;
                safe_zone[8][1] = coords[1] + 1;
                
                for (auto coords : safe_zone)
                {
                    //std::cout << "Safe zone: " << coords[0] << " " << coords[1] << std::endl;
                }

            }

            bool checkSafeZone(int x, int y) //Checks for safe zone collisions
            {
                bool collision = false;
                for (auto coords : safe_zone)
                        {
                            if (x == coords[0] && y == coords[1])
                            {
                                collision = true;
                                return collision;
                                break;
                            }
                        }
                collision = false;
                return collision;
            }

            bool checkCollision(int x, int y) //Checks for mine collisions
            {
                bool collision = false;
                for (int j = 0; j < mine_coords.size(); j++)
                        {
                            if ((x == mine_coords[j][0] && y == mine_coords[j][1]) || checkSafeZone(x, y))
                            {
                                collision = true;
                                return collision;
                                break;
                            }
                        }
                collision = false;
                return collision;
            }

            void initialize()
            {
                srand(time(0));

                for (int i = 0; i < size[0]; i++) //Set all cells to 0
                {
                    for (int j = 0; j < size[1]; j++)
                    {
                        board[i][j] = 0;
                    }
                }

                for (int i = 0; i < mines; i++) //Initialize mine_coords to -1 so mines can potentially be placed at 0,0
                {
                    mine_coords[i] = {-1, -1};
                } 

                int mines_placed = 0;

                for (int i = 0; i < mines; i++) //Random mine placement
                {   
                    int x = rand() % size[0];
                    int y = rand() % size[1];

                    // Check for collisions
                    while (checkCollision(x, y)) {
                        x = rand() % size[0];
                        y = rand() % size[1];
                    }
                    mine_coords[i][0] = x;
                    mine_coords[i][1] = y;
                    board[x][y] = -1;
                    mines_placed++;
                    //std::cout << mines_placed << ") Mine placed at: " << mine_coords[i][0] << ", " << mine_coords[i][1] << std::endl;
                }

                for (int i = 0; i < size[0]; i++)
                {
                    for (int j = 0; j < size[1]; j++)
                    {
                        if (board[i][j] != -1)
                        {
                        int surrounding_mines = 0;

                        for (int x = -1; x <= 1; x++)
                        {
                            for (int y = -1; y <= 1; y++)
                            {
                                if ((i + x >= 0 && i + x < size[0] && j + y >= 0 && j + y < size[1]) && board[i + x][j + y] == -1)
                                {
                                    surrounding_mines++;
                                    board[i][j]++;
                                }
                            }
                        }

                        }
                    }
                }
            }

        public:

            int getWidth()
            {
                return size[0];
            }
            int getHeight()
            {
                return size[1];
            }

            void reset()
            {
                
                board.clear();
                revealed.clear();
                textures.clear();
                sprites.clear();
                cell_coords.clear();
                mine_coords.clear();
                flags.clear();

                outcome = 0;
                leftClicks = 0;     
                difficulty = 0;
                mines = 10;
                safe_zone ={{0,0}, {0,0}, {0,0},
                            {0,0}, {0,0}, {0,0},
                            {0,0}, {0,0}, {0,0}}; 
            }

            int getOutcome()
            {
                int nonMines = size[0] * size[1] - mines;
                int revealCount = 0;

                for (int i = 0; i < size[0]; i++)
                {
                    for (int j = 0; j < size[1]; j++)
                    {
                        if (board[i][j] == -1 && revealed[i][j] == true)
                        {
                            outcome = -1;
                        }
                        if (revealed[i][j] == true && board[i][j] != -1)
                        {
                            revealCount++;
                            if (revealCount == nonMines)
                            {
                                outcome = 1;
                            }
                        }

                    }
                }
                return outcome;
            }

            void setDifficulty(int d)
            {
                difficulty = d;
            }

            void blankCells()
            {
                
                for (int i = 0; i < size[0]; i++) //Set all cells to 0
                {
                    for (int j = 0; j < size[1]; j++)
                    {
                        textures[i][j].loadFromFile("src/res/blank.png");
                        board[i][j] = 0;
                        revealed[i][j] = false;
                        flags[i][j] = false;
                        cell_coords[i][j] = {i, j};
                        sprites[i][j].setTexture(textures[i][j]);
                        sprites[i][j].setScale(0.5, 0.5);
                        sprites[i][j].setPosition(i * cellSize, j * cellSize);
                    }
                }
            }

            void emptyBoard()
            {
                switch (difficulty)
                {
                    case 0:
                        size[0] = 9;
                        size[1] = 9;
                        mines = 10;
                        mine_coords.resize(mines, std::vector<int>(2));
                        board.resize(size[0], std::vector<int>(size[1]));  
                        textures.resize(size[0], std::vector<sf::Texture>(size[1]));
                        sprites.resize(size[0], std::vector<sf::Sprite>(size[1]));

                        cell_coords.resize(size[0]);  // Resize outer vector to n rows

                        for (int i = 0; i < size[0]; ++i) { 
                            cell_coords[i].resize(size[1], std::vector<int>(2));  // Resize inner vectors to 1x2
                        }

                        revealed.resize(size[0], std::vector<bool>(size[1]));
                        flags.resize(size[0], std::vector<bool>(size[1]));
                        //blankCells();
                        break;
                    case 1:
                        size[0] = 16;
                        size[1] = 16;
                        mines = 40;
                        mine_coords.resize(mines, std::vector<int>(2));
                        board.resize(size[0], std::vector<int>(size[1]));
                        textures.resize(size[0], std::vector<sf::Texture>(size[1]));
                        sprites.resize(size[0], std::vector<sf::Sprite>(size[1]));

                        cell_coords.resize(size[0]);  // Resize outer vector to n rows

                        for (int i = 0; i < size[0]; ++i) {
                            cell_coords[i].resize(size[1], std::vector<int>(2));  // Resize inner vectors to 1x2
                        }

                        revealed.resize(size[0], std::vector<bool>(size[1]));
                        flags.resize(size[0], std::vector<bool>(size[1]));
                        //blankCells();
                        break;
                    case 2:
                        size[0] = 30;
                        size[1] = 16;
                        mines = 99;
                        mine_coords.resize(mines, std::vector<int>(2));
                        board.resize(size[0], std::vector<int>(size[1]));
                        textures.resize(size[0], std::vector<sf::Texture>(size[1]));
                        sprites.resize(size[0], std::vector<sf::Sprite>(size[1]));

                        cell_coords.resize(size[0]);  // Resize outer vector to n rows
                        
                        for (int i = 0; i < size[0]; ++i) {
                            cell_coords[i].resize(size[1], std::vector<int>(2));  // Resize inner vectors to 1x2
                        }

                        revealed.resize(size[0], std::vector<bool>(size[1]));
                        flags.resize(size[0], std::vector<bool>(size[1])); 
                        break;
                }
                blankCells();
            }

            void autoClear(sf::RenderWindow& target)
            {
                std::vector<int> currentCoords;
                while (toUncover.size() > 0)
                {
                    currentCoords = toUncover[toUncover.size() - 1];
                    clear(target, toUncover[toUncover.size() - 1]);
                    for (auto it = toUncover.begin(); it != toUncover.end(); ++it) 
                    {
                        if ((*it)[0] == currentCoords[0] && (*it)[1] == currentCoords[1]) 
                        {                       
                        toUncover.erase(it);                   
                        break;
                        }
                    }
                }
            }

            void clear(sf::RenderWindow& target, std::vector<int> coords)
            {
                for (int i = -1; i < 2; i++)
                {
                    for (int j = -1; j < 2; j++)
                    {
                        if (!(i == 0 && j == 0) && coords[0] + i >= 0 && coords[0] + i < size[0] && coords[1] + j >= 0 && coords[1] + j < size[1])
                        {
                            if (board[coords[0] + i][coords[1] + j] == 0 && !revealed[coords[0] + i][coords[1] + j])
                            {   
                                toUncover.push_back({coords[0] + i, coords[1] + j});
                            }

                            revealed[coords[0] + i][coords[1] + j] = true;
                            textures[coords[0] + i][coords[1] + j].loadFromFile("src/res/" + std::to_string(board[coords[0] + i][coords[1] + j]) + ".png");
                            sprites[coords[0] + i][coords[1] + j].setTexture(textures[coords[0] + i][coords[1] + j]);
                            target.draw(sprites[coords[0] + i][coords[1] + j]); 
                            target.display();
                            
                        }
                        
                    }
                }
            }
            void drawBoard(sf::RenderTarget& target) //Draws board in window
            {
                
                for (int i = 0; i < size[1]; i++)
                {
                    for (int j = 0; j < size[0]; j++)
                    {
                        target.draw(sprites[j][i]);
                    }
                }

            }

            void printBoard() //Prints matrix to terminal
            {
                int minecount = 0;
                for (int i = 0; i < size[1]; i++)
                {
                    for (int j = 0; j < size[0]; j++)
                    {
                        //std::cout << board[j][i] << " ";
                        if (board[j][i] == -1)
                        {
                            minecount++;
                        }

                    }
                    //std::cout << std::endl;
                }
                //std::cout << minecount << " mines" << std::endl;

            }

            void checkClick(sf::RenderWindow& target)
            {
                bool clicked = false;
                bool inside = false;
                bool pressed = false;
                bool flagged = false;
                std::string textureFile;
                sf::Vector2i mousePos = sf::Mouse::getPosition(target);

                sf::Texture blankPressed;
                sf::Sprite blankPressedSprite;
                blankPressed.loadFromFile("src/res/blank pressed.png");
                blankPressedSprite.setTexture(blankPressed);
                
                while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
                {
                    mousePos = sf::Mouse::getPosition(target);
                    for (int i = 0; i < size[0]; i++)
                    {
                        for (int j = 0; j < size[1]; j++)
                        {
                            if (mousePos.x > cell_coords[i][j][0] && mousePos.x < (cell_coords[i][j][0] + 1)* cellSize && mousePos.y > cell_coords[i][j][1] && mousePos.y < (cell_coords[i][j][1] + 1)* cellSize)
                            {
                                if (!pressed)
                                {
                                    pressed = true;
                                    inside = true;
                                    clickedCoords = {cell_coords[i][j][0], cell_coords[i][j][1]};
                                    if (revealed[clickedCoords[0]][clickedCoords[1]] == false && flags[clickedCoords[0]][clickedCoords[1]] == false)
                                    {
                                        textures[clickedCoords[0]][clickedCoords[1]].loadFromFile("src/res/blank pressed.png");
                                        sprites[clickedCoords[0]][clickedCoords[1]].setTexture(textures[clickedCoords[0]][clickedCoords[1]]);
                                        target.draw(sprites[clickedCoords[0]][clickedCoords[1]]); 
                                        target.display();
                                    }                        
                                }

                            }
                        }
                    }
                    if (pressed)
                    {
                        if (mousePos.x < clickedCoords[0] * cellSize || 
                            mousePos.x > (clickedCoords[0] + 1) * cellSize || 
                            mousePos.y < clickedCoords[1] * cellSize ||
                            mousePos.y > (clickedCoords[1] + 1) * cellSize)
                        {
                            inside = false;
                        }
                        else 
                        {
                            inside = true;
                        }
                    }
                    
                }                 

                while (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                {
                    mousePos = sf::Mouse::getPosition(target);
                    for (int i = 0; i < size[0]; i++)
                    {
                        for (int j = 0; j < size[1]; j++)
                        {
                            if (mousePos.x > cell_coords[i][j][0] && mousePos.x < (cell_coords[i][j][0] + 1)* cellSize && mousePos.y > cell_coords[i][j][1] && mousePos.y < (cell_coords[i][j][1] + 1)* cellSize)
                            {
                                if (!flagged)
                                {
                                    flagged = true;
                                    if (flags[i][j] == false && revealed[i][j] == false)
                                    {
                                        textures[i][j].loadFromFile("src/res/flag.png");
                                        sprites[i][j].setTexture(textures[i][j]);
                                        target.draw(sprites[i][j]);
                                        target.display();
                                        flags[i][j] = true;
                                    }
                                    else if (flags[i][j] == true && revealed[i][j] == false)
                                    {
                                        textures[i][j].loadFromFile("src/res/blank.png");
                                        sprites[i][j].setTexture(textures[i][j]);
                                        target.draw(sprites[i][j]);
                                        target.display();
                                        flags[i][j] = false;
                                    }
                                }
                                
                            }
                        }
                    }
                }

                if (inside == true && pressed == true && revealed[clickedCoords[0]][clickedCoords[1]] == false && flags[clickedCoords[0]][clickedCoords[1]] == false) //Clicking an unrevealed unflagged cell
                {
                    if (leftClicks == 0)
                    {
                        generateSafeZone(clickedCoords);
                        initialize(); 
                        printBoard();
                    }
                    pressed = false;
                    if (board[clickedCoords[0]][clickedCoords[1]] == -1)
                    {
                        revealed[clickedCoords[0]][clickedCoords[1]] = true;
                        textures[clickedCoords[0]][clickedCoords[1]].loadFromFile("src/res/mine.png");
                        sprites[clickedCoords[0]][clickedCoords[1]].setTexture(textures[clickedCoords[0]][clickedCoords[1]]);
                        
                        for(int i = 0; i < size[0]; i++)
                        {
                            for(int j = 0; j < size[1]; j++)
                            {
                                if (flags[i][j] == true && board[i][j] != -1)
                                {
                                    textures[i][j].loadFromFile("src/res/wrong.png");
                                    sprites[i][j].setTexture(textures[i][j]);
                                    target.draw(sprites[i][j]);
                                }
                                if (board[i][j] == -1)
                                {
                                    textures[i][j].loadFromFile("src/res/mine.png");
                                    sprites[i][j].setTexture(textures[i][j]);
                                    target.draw(sprites[i][j]);
                                }
                            }
                        }
                        target.draw(sprites[clickedCoords[0]][clickedCoords[1]]);
                        target.display();
                    }
                    else
                    {
                        revealed[clickedCoords[0]][clickedCoords[1]] = true;
                        textures[clickedCoords[0]][clickedCoords[1]].loadFromFile("src/res/" + std::to_string(board[clickedCoords[0]][clickedCoords[1]]) + ".png");
                        sprites[clickedCoords[0]][clickedCoords[1]].setTexture(textures[clickedCoords[0]][clickedCoords[1]]);
                        target.draw(sprites[clickedCoords[0]][clickedCoords[1]]); 
                        target.display();
                        leftClicks++;
                        if (board[clickedCoords[0]][clickedCoords[1]] == 0)
                        {
                            clear(target, clickedCoords);
                            autoClear(target);
                        }
                    }
                }

                if (!inside && pressed == true && revealed[clickedCoords[0]][clickedCoords[1]] == false)
                {
                    pressed = false;
                    textures[clickedCoords[0]][clickedCoords[1]].loadFromFile("src/res/blank.png");
                    sprites[clickedCoords[0]][clickedCoords[1]].setTexture(textures[clickedCoords[0]][clickedCoords[1]]);
                    target.draw(sprites[clickedCoords[0]][clickedCoords[1]]);
                    target.display();
                }
                else if(!inside && pressed == true && revealed[clickedCoords[0]][clickedCoords[1]] == true)
                {
                    if (board[clickedCoords[0]][clickedCoords[1]] == -1)
                    {
                        textures[clickedCoords[0]][clickedCoords[1]].loadFromFile("src/res/mine.png");
                        sprites[clickedCoords[0]][clickedCoords[1]].setTexture(textures[clickedCoords[0]][clickedCoords[1]]);
                        target.draw(sprites[clickedCoords[0]][clickedCoords[1]]);
                        target.display();
                    }
                    else
                    {
                        textures[clickedCoords[0]][clickedCoords[1]].loadFromFile("src/res/" + std::to_string(board[clickedCoords[0]][clickedCoords[1]]) + ".png");
                        sprites[clickedCoords[0]][clickedCoords[1]].setTexture(textures[clickedCoords[0]][clickedCoords[1]]);
                        target.draw(sprites[clickedCoords[0]][clickedCoords[1]]); 
                        target.display();
                    }
                }
            }
            
    };

class button
{
    private:

    public:
        std::vector<int> coords = {0, 0};
        std::vector<int> size = {100, 50};
        bool pressed = false;
        bool clicked = false;
        bool inside = false;
        std::string textureFile = "src/res/";
        std::string names[5] = {"easy", "medium", "hard", "restart", "exit"};
        int diff = 0;
        std::string name;
        sf::Texture texture;
        sf::Sprite sprite;

        void drawButton(sf::RenderWindow& target) //Draws button in window
        {   
            if (!texture.loadFromFile(textureFile)) // Load texture from file
            {
                //std::cout << "ERROR" << std::endl;
            }

            sprite.setTexture(texture);
            sprite.setScale(0.5, 0.5);
            sprite.setPosition(coords[0], coords[1]);
            target.draw(sprite);
        }

        bool checkClick(sf::RenderWindow& target)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(target);

                while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
                {
                    if (mousePos.x > coords[0] && mousePos.x < coords[0] + size[0] && mousePos.y > coords[1] &&  mousePos.y < coords[1] + size[1])
                    {
                        pressed = true;
                        inside = true;
                        textureFile = "src/res/";

                        if (name != names[diff] + " pressed.png") 
                        {
                            name += " pressed.png";
                        }
                        textureFile += name;
                        drawButton(target);
                    }
                    

                    if (mousePos.x < coords[0] || mousePos.x > coords[0] + size[0] ||
                        mousePos.y < coords[1] || mousePos.y > coords[1] + size[1] && pressed == true)
                    {
                        inside = false;
                    }
                    
                    return false; 
                } 

                if (pressed)
                {   
                    
                    textureFile = "src/res/";
                    name = names[diff];
                    textureFile += name + ".png";
                    drawButton(target);
                    pressed = false;

                    if (inside)
                    {
                        clicked = true;
                    }
                }
                
                if (clicked) 
                {
                    clicked = false;                
                    return true; 
                }

            return false; // Not pressed or not within bounds
        }
};

void startMenu(game& G, bool& over);
void play(game& G, bool& over);
void endMenu(game& G, sf::RenderWindow& target, bool& over, sf::Time& elapsed);
