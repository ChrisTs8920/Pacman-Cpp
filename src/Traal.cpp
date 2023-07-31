#include <curses.h>
#include <algorithm>
#include "Engine.h"
#include "Moving.h"
#include "Traal.h"

Traal::Traal(int mapHeight, int mapWidth)
{
    color = 2;
    Character = 'T';
    mapY = mapHeight;
    mapX = mapWidth;
    init_pair(color, COLOR_MAGENTA, COLOR_BLACK); //init a color pair
    attron(COLOR_PAIR(color) | A_BOLD);           //enable color for Traal
    placeOnMap(Character, mapWidth, mapHeight);
}

void Traal::moveEntity(int y, int x, Engine *myEngine)
{
    if (calcPath(y, x, *myEngine)) //if found a path
    {
        PosY = pathToFollow[1][0];
        PosX = pathToFollow[1][1];
        if (!myEngine->caughtP(y, x, PosY, PosX)) //if didn't caught P
        {
            attron(COLOR_PAIR(color) | A_BOLD); //re-enable color after move
            mvaddch(PosY, PosX, Character);
            mvaddch(pathToFollow[0][0], pathToFollow[0][1], ' '); //replace 'leftover' T with whitespace
            attroff(COLOR_PAIR(color) | A_BOLD);
            pathToFollow.clear(); //empty pathToFollow to create new path in next turn
        }
    }
}

int Traal::calcPath(int y, int x, Engine &myEngine) //Calculating path using Breadth-First-Search algorithm
{
    bool visited[mapY][mapX] = {false};
    std::vector<std::vector<std::vector<int>>> prev(mapY, std::vector<std::vector<int>>(mapX, std::vector<int>(2, 0))); //holds previous position of each position
    int tempPosY = PosY;
    int tempPosX = PosX;
    std::vector<std::vector<int>> adjacent =
        {{tempPosY, tempPosX - 1}, {tempPosY - 1, tempPosX}, {tempPosY, tempPosX + 1}, {tempPosY + 1, tempPosX}}; //adjacent positions of T
    visited[tempPosY][tempPosX] = true;
    queue.push_back({tempPosY, tempPosX});
    //bfs while loop
    while (!queue.empty())
    {
        tempPosY = queue[0][0];
        tempPosX = queue[0][1];
        adjacent = {{tempPosY, tempPosX - 1}, {tempPosY - 1, tempPosX}, {tempPosY, tempPosX + 1}, {tempPosY + 1, tempPosX}};
        queue.erase(queue.begin());
        if (tempPosY == y && tempPosX == x) //if found P, traceback path
        {
            int i = y;
            int j = x;
            int tempI, tempJ;
            while (i != 0 && j != 0) //this loop creates path from P to T
            {
                if (prev[i][j][0] != 0 && prev[i][j][1] != 0)
                    pathToFollow.push_back({prev[i][j][0], prev[i][j][1]});
                tempI = i;
                tempJ = j;
                i = prev[tempI][tempJ][0];
                j = prev[tempI][tempJ][1];
            }
            std::reverse(pathToFollow.begin(), pathToFollow.end()); //reverse so that it becomes path from T to P
            pathToFollow.push_back({y, x});
            queue.clear(); //clear queue for next turn
            return 1;
        }
        //this loop checks adjacent positions of Traal, if position is not wall (and has not been visited), enqueue to queue
        for (int i = 0; i < adjacent.size(); i++)
        {
            if (!visited[adjacent[i][0]][adjacent[i][1]] && myEngine.collisionCheckMonster(adjacent, i))
            {
                visited[adjacent[i][0]][adjacent[i][1]] = true;
                queue.push_back({adjacent[i][0], adjacent[i][1]});
                prev[adjacent[i][0]][adjacent[i][1]] = {tempPosY, tempPosX}; //store previous position of current position
            }
        }
    }
    return 0;
}