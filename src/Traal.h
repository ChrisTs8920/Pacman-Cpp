#ifndef Traal_h
#define Traal_h

class Traal : public Moving
{
    int mapX, mapY;
    //private members
    std::vector<std::vector<int>> queue;        //queue for bfs
    std::vector<std::vector<int>> pathToFollow; //path to follow once bfs found P
    //private methods
    int calcPath(int y, int x, Engine &myEngine);

public:
    //constructor
    Traal(int mapHeight, int mapWidth);
    //public method
    void moveEntity(int y, int x, Engine *myEngine);
};

#endif