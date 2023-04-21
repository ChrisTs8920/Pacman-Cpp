#include "HiScore.h"
#include "Engine.h"
#include "Moving.h"
#include "Potter.h"
#include "Gnome.h"
#include "Traal.h"

//Made using PDCurses-3.9 on Windows 10.
//argv[1] is map file, argv[2] is hiscore file.
//Compile with: g++ *.cpp -o main.exe -lpdcurses
//Execute with: ./main.exe

/*
--NOTES
1.Program is slow because of the movement algorithms of Gnome and Traal.
2.If there is a tie between current score and a score in hiscore array, the older score is kept.
    Example: if player scores 0 points, and the lowest score in the hiscore array is also 0 points,
    current player score doesn't get saved.
3.User's name can only be up to 10 characters (using getnstr() function).
*/

/*
--KNOWN BUGS:
1.Program crashes (rarely) if a monster catches Potter.
2.Resizing is problematic.
3.Monsters can hide each other.
*/

int main(int argc, char **argv)
{
    Engine myEngine(argv[1], argv[2], argc);
    int mapHeight = myEngine.getMapHeight();
    int mapWidth = myEngine.getMapWidth();
    Potter Player(mapHeight, mapWidth);
    Traal Traal1(mapHeight, mapWidth);
    Gnome Gnome1(mapHeight, mapWidth);
    while (myEngine.gameRunning) //main loop
    {
        myEngine.mainMethod(Player, Gnome1, Traal1);
    }
    return 0;
}