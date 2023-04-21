#ifndef Potter_h
#define Potter_h

class Potter : public Moving
{
public:
    //constructor
    Potter(int mapHeight, int mapWidth);
    //public method
    bool moveEntity(int ch, Engine *myEngine);
};

#endif