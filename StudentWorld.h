#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <vector>
using namespace std;

//gameworld.h

const int MAXSIZE_X = 64;
const int MAXSIZE_Y = 64;

const int MINESHAFT_START_LEFT = 29;
const int MINESHAFT_STOP_RIGHT = 34;
const int MINESHAFT_BOTTOM = 5;
const int OBJECT_EXIST = 1;

const int BOULDER_EXIST = 2;

const int Y_OFFSET = 20;

const int Y_BOUND_TOP = 8;
const int X_BOUND_RIGHT = 4;

const int MAX_BOULDER_Y = 38;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class IceMan; //FORWARD DECLARATION

class StudentWorld : public GameWorld
{
private:
    Actor * m_actor[MAXSIZE_X][MAXSIZE_Y];
    Ice * m_Ice[MAXSIZE_X][MAXSIZE_Y];
    IceMan* m_IceMan;
    
    vector<Protester*> protester;
    
    int ticks;
    int protesterIndex;
    
    int current_level_number;
    int m_lives;
    int L;  // oil
    int m_oilCollected;
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir)
    {
        ticks = 0;
        current_level_number= 0;
        m_lives = 3;
        protesterIndex = 0;
        m_oilCollected = 0;
    }
    
    void setDisplayText();
    string FunctionFormat(int Level, int Lives, int Hlth, int Wtr, int Gld, int Oil, int Sonar, int Scr);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    void removeDeadActors();
    
    void deleteIce(int xPassed, int yPassed);
    //bool checkActorBelow(int xPassed, int yPassed, int IMID);
    
    bool checkBoulderBelow(int xPassed, int yPassed);
    
    bool checkIceMan(int xPassed, int yPassed, Actor::Direction dir);
    
    bool barrelVisible(int xPassed, int yPassed);
    
    bool checkIceManBelow(int xPassed, int yPassed);
    
    bool checkIce(int xPassed, int yPassed);
    
    bool checkIceBelow(int xPassed, int yPassed);
    
    bool checkProtester(int xPassed, int yPassed, Protester::Direction dir);
    
    void checkItems(int xPassed, int yPassed);
    
    bool protesterCheckIceMan(int xPassed, int yPassed);
    
    bool protesterFacingIceMan(int xPassed, int yPassed, Protester::Direction dir);
    
    void setIceManHP(int hitPoints);
    
    inline void annoyIceMan(int hitPoints)
    {
        int tempHP = m_IceMan->getHitPoint();
        
        tempHP = tempHP - hitPoints;
        
        m_IceMan->setHitpoints(tempHP);
        if(tempHP == 0)
        playSound(SOUND_PLAYER_GIVE_UP);
    }
    
    inline void annoyProtester(int hitPoints)
    {
        int index = getIndex();
        
        int tempHP = protester[index]->getHitPoint();
        
        tempHP = tempHP - hitPoints;
        
        protester[index]->setHitpoints(tempHP);
        
        playSound(SOUND_PROTESTER_ANNOYED);
        
        cout << "P LIFE" << protester[index]->getHitPoint() << endl;
    }
    
    bool ItemDoesNotExist(int itemX, int itemY);
    void goldTab(int xPassed, int yPassed);
    void squirt(int xPassed, int yPassed, Actor::Direction dir);
    
    bool distanceBtwObj(int itemX, int itemY);
    
    
    inline IceMan* getIceMan()
    {
        return m_IceMan;
    }
    inline int getLives() const
    {
        return m_lives;
    }
    inline int getLevel()
    {
        return current_level_number;
    }
    
    inline int getTicks()
    {
        return ticks;
    }
    inline void setIndex(int index)
    {
        this->protesterIndex = index;
    }
    inline int getIndex()
    {
        return protesterIndex;
    }
    inline Protester* getProtester()
    {
        return protester[protesterIndex];
    }
    inline int getOil()
    {
        return L;
    }
    inline void decreaseOil()
    {
        L--;
    }
    
    inline void increaseOilCollected()
    {
        m_oilCollected++;
    }
    
    
};

#endif 
