#include "Minesweeper.hpp"

    bool over = false;
    game G;

int main() 
{
    
    
    //G.printBoard();

    while (!over)
    {
        startMenu(G, over);
        if (over)
        {
            break;
        }
        play(G, over);     
    }

    return 0;       
}
