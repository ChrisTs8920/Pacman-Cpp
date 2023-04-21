#ifndef Engine_h
#define Engine_h

#include <vector>
#include <string>
#include <curses.h>
#include "HiScore.h"

//telling the compiler that these classes will be declared later
class Potter;
class Traal;
class Gnome;

class Engine
{
    //private members
    std::vector<std::string> map;
    const short wallColor = 4;
    const short jewelColor = 5;
    const short backGroundColor = 6;
    const short parchmentColor = 7;
    const short colorOrange = 8;
    const short menuColor = 9;
    const short bkgMenuColor = 20;
    const short defaultPair = 21; //black and white
    const char jewelCharacter = 'J';
    const char parchmentCharacter = 'M';
    short scoreWinWidth;
    short scoreWinHeight;
    short playerScore = 0;
    short playerJewels = 0;
    bool gotCaught = false;
    bool parchmentSpawned = false;
    bool parchmentAcquired = false;
    int EntityPos[11][2]; //storing x, y for 10 jewels + 1 parchment  
    int mapHeight = 0;
    int mapWidth = 0;
    char playerName[11]; //10 characters + \0
    char difficulty;
    std::string mapName;
    std::string hiScoreName;
    HiScore objScore;
    WINDOW *scoreWindow;
    //private methods
    void initScreen();
    void initMap();
    void initScoreScreen();
    void printScore();
    void spawnParchment();
    void saveToHiScore();
    void spawnEntity(char character, int i);
    void reDrawEntities();
    void calcScore();
    void refreshScreens();
    void checkForParchment();
    void startScreen();
    void endScreen();
    void checkForResize(int ch);
    void showHiScore();
    char chooseDifficulty();

public:
    bool gameRunning = true;
    //constructor, destructor
    Engine(char *nameOfMap, char *nameOfBinary, int argc);
    ~Engine();
    //public methods
    bool collisionCheckPotter(int PosX, int PosY, int ch);
    bool collisionCheckMonster(std::vector<std::vector<int>> &V, int t);
    bool caughtP(int y, int x, int PosY, int PosX);
    void mainMethod(Potter &player, Gnome &gnome, Traal &traal);
    //getters
    int getMapHeight();
    int getMapWidth();
};

#endif