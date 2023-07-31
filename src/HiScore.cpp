#include <fstream>
#include <curses.h>
#include <string.h>
#include <vector>
#include "HiScore.h"

HiScore::HiScore()
{
    for (int i = 0; i < 5; i++)
    {
        strcpy(array[i].name, "");
        array[i].score = 0;
    }
}

HiScore &HiScore::operator<<(char name[10])
{
    strcpy(tmpName, name);
    return *this;
}

HiScore &HiScore::operator<<(int points)
{
    tmpScore = points;
    return *this;
}

void HiScore::saveToFile(std::string fileName)
{
    try
    {
        //find min of array
        int min = INT_MAX;
        int pos = -1;
        for (int i = 0; i < 5; i++)
        {
            if (min >= array[i].score)
            {
                min = array[i].score;
                pos = i;
            }
        }
        
        //save current score to array
        if (tmpScore > min)
        {
            strcpy(array[pos].name, tmpName);
            array[pos].score = tmpScore;
        }
        
        //bubble sort for array
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4 - i; j++)
            {
                if (array[j].score < array[j + 1].score)
                {
                    //swap scores
                    int temp = array[j].score;
                    array[j].score = array[j + 1].score;
                    array[j + 1].score = temp;
                    //swap names
                    char tempN[10];
                    strcpy(tempN, array[j].name);
                    strcpy(array[j].name, array[j + 1].name);
                    strcpy(array[j + 1].name, tempN);
                }
            }
        }

        //write to binary file
        std::ofstream out("HiScore.dat", std::ios::binary);
        if (!out)
            throw -1;
        out.write((char *) &array, sizeof(struct Score[5]));
        out.close();
    }
    catch (int e)
    {
        clear();
        printw("Scores file not found, press any key to exit..");
        getch();
        exit(1);
    }
}

struct Score *HiScore::getArray()
{
    return array;
}