#include <curses.h>
#include <stdlib.h>
#include "Engine.h"
#include "Moving.h"
#include "Potter.h"

Potter::Potter(int mapHeight, int mapWidth)
{
    color = 1;
    Character = 'P';
    init_pair(color, COLOR_GREEN, COLOR_BLACK); //init a color pair
    attron(COLOR_PAIR(color) | A_BOLD);         //enable color for Potter
    placeOnMap(Character, mapWidth, mapHeight);
}

bool Potter::moveEntity(int ch, Engine *myEngine)
{
    attron(COLOR_PAIR(color) | A_BOLD); //re-enable color after move
    bool tmp = false;
    if (ch == KEY_LEFT && myEngine->collisionCheckPotter(PosX - 1, PosY, ch))
    {
        mvaddch(PosY, --PosX, Character); //move P left
        mvaddch(PosY, PosX + 1, ' ');     //replace 'leftover' P with whitespace
        tmp = true;
    }
    if (ch == KEY_RIGHT && myEngine->collisionCheckPotter(PosX + 1, PosY, ch))
    {
        mvaddch(PosY, ++PosX, Character); //move P right
        mvaddch(PosY, PosX - 1, ' ');
        tmp = true;
    }
    if (ch == KEY_UP && myEngine->collisionCheckPotter(PosX, PosY - 1, ch))
    {
        mvaddch(--PosY, PosX, Character); //move P up
        mvaddch(PosY + 1, PosX, ' ');
        tmp = true;
    }
    if (ch == KEY_DOWN && myEngine->collisionCheckPotter(PosX, PosY + 1, ch))
    {
        mvaddch(++PosY, PosX, Character); //move P down
        mvaddch(PosY - 1, PosX, ' ');
        tmp = true;
    }
    if (ch == 32) //space character in ascii
        tmp = true; //skip turn (for turn based movement)
    attroff(COLOR_PAIR(color) | A_BOLD);
    move(PosY, PosX); //move cursor to potter (cursor is hidden)
    return tmp;
}