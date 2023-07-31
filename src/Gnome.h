#ifndef Gnome_h
#define Gnome_h

class Gnome : public Moving
{
    int mapX, mapY;
    //private members
    std::vector<std::vector<int>> queue;        //queue for bfs
    std::vector<std::vector<int>> pathToFollow; //path to follow once bfs found P
    //private methods
    int calcPath(int y, int x, Engine &myEngine);

public:
    //constructor
    Gnome(int mapHeight, int mapWidth);
    //public method
    void moveEntity(int y, int x, Engine *myEngine);
};

#endif