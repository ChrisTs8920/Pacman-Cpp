#ifndef Moving_h
#define Moving_h

class Moving
{

protected:
    int PosX, PosY;
    short color;
    char Character;

public:
    void placeOnMap(char xCharacter, int mapWidth, int mapHeight);
    //getters
    int getPosX();
    int getPosY();
};

#endif