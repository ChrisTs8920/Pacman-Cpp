#ifndef HiScore_h
#define HiScore_h

#include <string>
#include <vector>

struct Score
{
    int score;
    char name[11]; //10 characters + \0
};

class HiScore
{
    //private members
    struct Score array[5];
    int tmpScore;
    char tmpName[11]; //10 characters + \0
    //used to temporarily store score and name to HiScore object

public:
    HiScore();
    //Operator Overloads
    HiScore &operator<<(char name[11]);
    HiScore &operator<<(int points);
    //public methods
    void saveToFile(std::string fileName);
    //getter
    struct Score *getArray();
};

#endif