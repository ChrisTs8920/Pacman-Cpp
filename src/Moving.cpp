#include <curses.h>
#include <stdlib.h>
#include "Engine.h"
#include "Moving.h"

void Moving::placeOnMap(char xCharacter, int mapWidth, int mapHeight)
{
    while (true)
    {
        PosX = rand() % mapWidth;
        PosY = rand() % mapHeight;
        if ((mvinch(PosY, PosX) & A_CHARTEXT) == ' ') //make sure it doesn't spawn inside a wall or an entity
            mvaddch(PosY, PosX, xCharacter);
        else
            continue;
        break;
    }
}

int Moving::getPosY()
{
    return PosY;
}

int Moving::getPosX()
{
    return PosX;
}