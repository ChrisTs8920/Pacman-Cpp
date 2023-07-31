#include <curses.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <windows.h>
#include <vector>
#include "Engine.h"
#include "Moving.h"
#include "Potter.h"
#include "Gnome.h"
#include "Traal.h"

void Engine::initScreen()
{
    initscr();            //init curses screen
    raw();                //disables line buffering
    keypad(stdscr, TRUE); //keypad for use of arrow keys
    start_color();        //init colors

    //initialize new colors
    init_color(colorOrange, 988, 498, 30);  
    init_color(bkgMenuColor, 130, 130, 130);

    //initialize color pairs
    init_pair(wallColor, COLOR_CYAN, COLOR_BLACK);
    init_pair(jewelColor, COLOR_YELLOW, COLOR_BLACK);
    init_pair(backGroundColor, COLOR_WHITE, COLOR_BLUE);
    init_pair(parchmentColor, colorOrange, COLOR_BLACK);
    init_pair(menuColor, COLOR_WHITE, bkgMenuColor);
    init_pair(defaultPair, COLOR_WHITE, COLOR_BLACK);

    if (!has_colors())
    {
        printw("Terminal doesn't support colors. press any key to exit..");
        getch();
        exit(1);
    }
    curs_set(0); // hides cursor
}

void Engine::initMap()
{
    srand(time(0));
    try
    {
        std::string line;
        std::ifstream fileIn("map.txt");
        if (!fileIn)
            throw -1;

        attron(COLOR_PAIR(wallColor) | A_BOLD);
        while (getline(fileIn, line)) //storing map
        {
            map.push_back(line);
            mapWidth = line.length();
            mapHeight++;
        }

        scoreWinHeight = mapHeight; //setting up score window sizes
        scoreWinWidth = 30;

        //resize_term(mapHeight + 1, mapWidth + scoreWinWidth + 2); //resize terminal to fit map and score window
        for (int i = 0; i < map.size(); i++)                      //printing map
            printw("%s\n", map[i].c_str());
        attroff(COLOR_PAIR(wallColor) | A_BOLD);

        for (int i = 0; i < 10; i++) //spawn 10 jewels
        {
            attron(COLOR_PAIR(jewelColor) | A_BOLD);
            spawnEntity(jewelCharacter, i);
            attroff(COLOR_PAIR(jewelColor) | A_BOLD);
        }
        fileIn.close();
    }
    catch (int err)
    {
        clear();
        printw("Map file not found, press any key to exit..");
        getch();
        exit(1);
    }
}

void Engine::initScoreScreen()
{
    scoreWindow = newwin(scoreWinHeight, scoreWinWidth, 0, mapWidth + 1);
    wbkgd(scoreWindow, COLOR_PAIR(backGroundColor));
    box(scoreWindow, 0, 0);
    wrefresh(scoreWindow);
}

Engine::Engine(char *nameOfMap, char *nameOfBinary, int argc)
{
    //try
    //{
        //if (argc == 3)
        //{
            //converting char* to std::string
            //std::string tmpName(nameOfMap);
            //std::string tmpbin(nameOfBinary);

            //storing map file name and binary file name to Engine
            //mapName = tmpName;
            //hiScoreName = tmpbin;

            initScreen();
            startScreen();
            initMap();
            initScoreScreen();
        //}
        //else if (argc < 3)
           // throw -1;
        //else
            //throw 1;
    //}
    /*catch (int e)
    {
        if (e == -1)
        {
            std::cout << "Not enough arguments. Enter any key to exit.";
            std::cin >> e;
            exit(1);
        }
        if (e == 1)
        {
            std::cout << "Too many arguments. Enter any key to exit.";
            std::cin >> e;
            exit(1);
        }
    }*/
}

Engine::~Engine()
{
    delwin(scoreWindow);
    endwin();
}

bool Engine::collisionCheckPotter(int PosX, int PosY, int ch)
{
    //check for wall
    bool i = false;
    if ((mvinch(PosY, PosX) & A_CHARTEXT) != '*' && ch == KEY_LEFT) //collision for left movement
        i = true;
    if ((mvinch(PosY, PosX) & A_CHARTEXT) != '*' && ch == KEY_RIGHT) //collision for right movement
        i = true;
    if ((mvinch(PosY, PosX) & A_CHARTEXT) != '*' && ch == KEY_UP) //collision for up movement
        i = true;
    if ((mvinch(PosY, PosX) & A_CHARTEXT) != '*' && ch == KEY_DOWN) //collision for down movement
        i = true;

    //check for Jewel
    if ((mvinch(PosY, PosX) & A_CHARTEXT) == jewelCharacter)
    {
        for (int i = 0; i < 10; i++)
        {
            if (PosY == EntityPos[i][0] && PosX == EntityPos[i][1]) //if player gets jewel
            {
                //send jewel off-screen
                EntityPos[i][0] = -10;
                EntityPos[i][1] = -10;
            }
        }
        calcScore();
    }

    //check for Parchment
    if ((mvinch(PosY, PosX) & A_CHARTEXT) == parchmentCharacter)
        parchmentAcquired = true;

    return i;
}

bool Engine::collisionCheckMonster(std::vector<std::vector<int>> &V, int t)
{
    if ((mvinch(V[t][0], V[t][1]) & A_CHARTEXT) == '*')
        return 0;
    else
        return 1;
}

bool Engine::caughtP(int Py, int Px, int PosY, int PosX)
{
    if (PosY == Py && PosX == Px)
    {
        gotCaught = true;
        return gotCaught;
    }
    else
        return gotCaught;
}

void Engine::mainMethod(Potter &player, Gnome &gnome, Traal &traal)
{
    int timer;
    if (difficulty == '2') //setups for real-time movement
    {
        timer = clock();
        halfdelay(1); //waits for 1 tenth of a second for input, else getch() returns ERR and program continues
    }

    int y, x;
    int ch = getch(); //Player Input
    int tmp;
    checkForResize(ch); //checks if user resizes window
    if (ch == 27)       //27 is the ASCII code for esc key
    {
        gameRunning = false;
        return;
    }
    x = player.getPosX(); //get Player's updated x and y pos
    y = player.getPosY();

    //real-time movement
    if (difficulty == '2')
    {
        if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT) //accept as input only arrow keys
            player.moveEntity(ch, this);
        if (clock() - 50 > timer) //monsters move every const time interval
        {
            gnome.moveEntity(y, x, this);
            traal.moveEntity(y, x, this);
        }
    }

    //turn-based movement
    if (difficulty == '1')
    {
        if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT || ch == 32) //accept as input only arrow keys and space
        {
            tmp = player.moveEntity(ch, this);
            if (tmp) //if player moved
            {
                gnome.moveEntity(y, x, this);
                traal.moveEntity(y, x, this);
            }
        }
    }

    reDrawEntities();
    refreshScreens();
    checkForParchment();
    endScreen();
}

void Engine::calcScore()
{
    playerScore += 10;
    playerJewels++;
}

void Engine::refreshScreens()
{
    printScore();
    wrefresh(scoreWindow);
    refresh();
}

void Engine::printScore()
{
    if (parchmentAcquired)
    {
        mvwprintw(scoreWindow, (scoreWinHeight / 2) - 2, (scoreWinWidth / 4), "YOU WON!   ");
        mvwprintw(scoreWindow, (scoreWinHeight / 2), (scoreWinWidth / 4), "Your score: %d", playerScore);
    }
    else
    {
        mvwprintw(scoreWindow, (scoreWinHeight / 2) - 2, (scoreWinWidth / 4), "POINTS: %d", playerScore);
        mvwprintw(scoreWindow, (scoreWinHeight / 2), (scoreWinWidth / 4), "JEWELS: %d/10", playerJewels);
    }
    if (gotCaught)
    {
        mvwprintw(scoreWindow, (scoreWinHeight / 2) - 2, (scoreWinWidth / 4), "YOU LOST!   ");
        mvwprintw(scoreWindow, (scoreWinHeight / 2), (scoreWinWidth / 4), "Your score: %d", playerScore);
    }
}

void Engine::spawnParchment()
{
    attron(COLOR_PAIR(parchmentColor) | A_BOLD);
    spawnEntity(parchmentCharacter, 10);
    attroff(COLOR_PAIR(parchmentColor) | A_BOLD);
}

void Engine::checkForParchment()
{
    if (playerJewels == 10 && !parchmentSpawned)
    {
        parchmentSpawned = true;
        spawnParchment();
    }
}

char Engine::chooseDifficulty()
{
    clear();
    attron(A_BOLD);
    while (true)
    {
        mvprintw((LINES / 3), COLS / 2 - 15, "Choose movement type:");
        mvprintw((LINES / 3) + 2, COLS / 2 - 15, "1.Turn-based movement (easier)");
        mvprintw((LINES / 3) + 4, COLS / 2 - 15, "2.Real-time movement (harder)");
        char option = getch();
        if (option == '1' || option == '2')
            return option;
    }
    attroff(A_BOLD);
}

void Engine::startScreen()
{
    bkgd(COLOR_PAIR(menuColor)); //set background color gray
    attron(A_BOLD);
    mvprintw((LINES / 2) - 2, (COLS - 15) / 2, "ENTER NAME: ");
    getnstr(playerName, 10); //reads only up to 10 characters from input
    attroff(A_BOLD);
    noecho(); //disable echo after player typed name
    clear();

    //menu
    while (true)
    {
        clear();
        attron(A_BOLD);
        mvprintw((LINES / 3), COLS / 2 - 6, "1.PLAY");
        mvprintw((LINES / 3) + 2, COLS / 2 - 6, "2.HISCORES");
        mvprintw((LINES / 3) + 4, COLS / 2 - 6, "3.EXIT");
        mvprintw((LINES - 1), 1, "Version: 1.0");
        mvprintw((LINES - 1), COLS - 27, "Made by: Chris Tsoychlakis");
        attroff(A_BOLD);
        refresh();
        char i = getch();
        if (i == '1')
        {
            difficulty = chooseDifficulty();
            break;
        }
        if (i == '2')
            showHiScore();
        if (i == '3')
            exit(0);
    }
    bkgd(COLOR_PAIR(defaultPair));
    clear();
}

void Engine::endScreen()
{
    if (parchmentAcquired)
    {
        playerScore += 100;
        saveToHiScore();
        while (true)
        {
            //randomly spawn green ones and zeros
            init_pair(1, COLOR_GREEN, COLOR_BLACK);
            attron(COLOR_PAIR(1) | A_BOLD);
            Sleep(1);
            int Y = rand() % mapHeight;
            int X = rand() % mapWidth;
            mvaddch(Y, X, rand() % 2 + '0'); // + '0' makes rand result an ascii character
            attroff(COLOR_PAIR(1) | A_BOLD);
            refreshScreens();
        }
    }
    if (gotCaught)
    {
        saveToHiScore();
        while (true)
        {
            //randomly spawn red ones and zeros
            init_pair(9, COLOR_RED, COLOR_BLACK);
            attron(COLOR_PAIR(9) | A_BOLD);
            Sleep(1);
            int Y = rand() % mapHeight;
            int X = rand() % mapWidth;
            mvaddch(Y, X, rand() % 2 + '0'); // + '0' makes rand result an ascii character
            attroff(COLOR_PAIR(9) | A_BOLD);
            refreshScreens();
        }
    }
}

void Engine::checkForResize(int ch)
{
    if (ch == KEY_RESIZE)
        resize_term(0, 0); //Resize properly (?)
    wbkgd(scoreWindow, COLOR_PAIR(backGroundColor)); //re-draw scores screen
    box(scoreWindow, 0, 0);
}

void Engine::saveToHiScore() //save current name and score to object of class HiScore, then save to file
{
    objScore << playerName << playerScore;
    objScore.saveToFile(hiScoreName);
}

void Engine::spawnEntity(char character, int i)
{
    while (true)
    {
        int PosX = rand() % mapWidth;
        int PosY = rand() % mapHeight;
        if ((mvinch(PosY, PosX) & A_CHARTEXT) == ' ') //make sure entity doesn't spawn inside a wall or another entity
        {
            mvaddch(PosY, PosX, character);
            //store entity's coordinates
            EntityPos[i][0] = PosY;
            EntityPos[i][1] = PosX;
        }
        else
            continue;
        break;
    }
}

void Engine::reDrawEntities()
{
    for (int i = 0; i < 10; i++)
    {
        attron(COLOR_PAIR(jewelColor) | A_BOLD);
        mvaddch(EntityPos[i][0], EntityPos[i][1], jewelCharacter);
        attroff(COLOR_PAIR(jewelColor) | A_BOLD);
    }
    if (parchmentSpawned)
    {
        attron(COLOR_PAIR(parchmentColor) | A_BOLD);
        mvaddch(EntityPos[10][0], EntityPos[10][1], parchmentCharacter);
        attroff(COLOR_PAIR(parchmentColor) | A_BOLD);
    }
}

int Engine::getMapHeight()
{
    return mapHeight;
}

int Engine::getMapWidth()
{
    return mapWidth;
}

void Engine::showHiScore() //shows score to screen (reads the score from file)
{
    try
    {
        std::ifstream in("HiScore.dat", std::ios::binary);
        if (!in)
            throw -1;
        //read from file
        in.read((char *)objScore.getArray(), sizeof(struct Score[5]));
        in.close();
        clear();
        char temp[10];
        attron(A_BOLD);
        for (int i = 0; i < 5; i++)
        {
            if (objScore.getArray()[i].name[0] == '\0')
                strcpy(temp, "-EMPTY-");
            else
                strcpy(temp, objScore.getArray()[i].name);
            //print name, score
            mvprintw((LINES / 3) + i * 2, (COLS - 15) / 2, "%d. %s  ", i + 1, temp);
            printw("%d", objScore.getArray()[i].score);
        }
        mvprintw((LINES / 3) + 6 * 2, (COLS - 27) / 2, "Press any key to go back..");
        attroff(A_BOLD);
        getch();
        refresh();
    }
    catch (int e)
    {
        clear();
        printw("Scores file not found, press any key to exit..");
        getch();
        exit(1);
    }
}