#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include "GameWorld.h"
#include <algorithm>

//actor.h

const int DM_START_X = 30;
const int DM_START_Y = 60;

class StudentWorld; //Forward declaration

enum BoulderState
{
    stable, falling, waiting
};

enum GoldNuggetState
{
    permanent, temporary
};


enum ProtesterState
{
    rest, leaveOilField, dead, start
};

enum SonarState
{
};

class Actor : public GraphObject
{
private:
    int m_hitpoints;
    StudentWorld * m_world;
    
public:
    Actor(StudentWorld * world, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    :GraphObject(imageID, startX, startY, right, size, depth)
    {
        m_hitpoints = 10;
        m_world = world;
    }
    void setHitpoints(int newHitpoints)
    {
        this->m_hitpoints = newHitpoints;
    }
    int getHitPoint()
    {
        return m_hitpoints;
    }
    
    virtual bool isAlive()
    {
        return m_hitpoints > 0;
    }
    virtual StudentWorld* getWorld()
    {
        return m_world;
    }
    
    virtual void doSomething();
};


class IceMan : public Actor
{
private:
    int m_water;
    int m_sonarCharges;
    int m_goldNuggets;
    unsigned int waitTime = 0;
    
public:
    IceMan(StudentWorld * world)
    : Actor(world, IID_PLAYER, DM_START_X, DM_START_Y)
    {
        setVisible(true);
        setHitpoints(10);
        
        m_water = 5;
        m_goldNuggets = 0;
        m_sonarCharges = 1;
    }
    
    virtual void doSomething();
    
    int getGold()
    {
        return m_goldNuggets;
    }
    void decreaseGold()
    {
        m_goldNuggets--;
    }
   
    void increaseGold()
    {
        m_goldNuggets++;
    }
    int getWater()
    {
        return m_water;
    }
    int getHealth()
    {
        return getHitPoint()*10;
    }
    void setWater(int water)
    {
        m_water = water;
    }
    void addWater(int water)
    {
        m_water += water;
    }
    int getSonar()
    {
        return m_sonarCharges;
    }
    void increaseSonar()
    {
        m_sonarCharges++;
    }
    void decreaseSonar()
    {
        m_sonarCharges--;
    }
    void reduceWater()
    {
        m_water--;
    }
    inline IceMan* getIceMan()
    {
        return this;
    }
    virtual StudentWorld* getWorld()
    {
        return Actor::getWorld();
    }
};


class Squirt : public Actor
{
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction dir)
    : Actor(world, IID_WATER_SPURT, startX, startY, dir, 1.0, 1)
    {
        setVisible(true);
        distanceTraveled = 0;
    }
    virtual void doSomething();
private:
    int distanceTraveled;
    unsigned int waitTime = 0;
};

class Ice : public Actor
{
public:
    Ice(StudentWorld * world, int startX, int startY)
    : Actor(world, IID_ICE, startX, startY, right, 0.25, 3)
    {
        setVisible(true);
    }
private:
};

class Barrel : public Actor
{
public:
    Barrel(StudentWorld * world, int startX, int startY)
    : Actor(world, IID_BARREL, startX, startY, right, 1.0, 2)
    {
        setVisible(false); //Barrels should start hidden and only be discovered when walked over
    }
    virtual void doSomething();
};


class GoldNugget : public Actor
{
public:
    GoldNugget(StudentWorld * world, int startX, int startY)
    : Actor(world, IID_GOLD, startX, startY, right, 1.0, 2)
    {
        this->gold_state = permanent;
        setVisible(false);
    }
    
    void doSomething();
    void getGoldState();
private:
    GoldNuggetState gold_state;
    //    unsigned int nearBy =
};


class Boulder : public Actor
{
    //Must start in a stable state, must add code later
public:
    Boulder(StudentWorld * world, int startX, int startY)
    :Actor(world, IID_BOULDER, startX, startY, down, 1.0, 1)
    {
        this->boulder_state = stable;
        setVisible(true);
    }
    void doSomething();
    bool isStable();
    BoulderState getState()
    {
        return boulder_state;
    }
    
private:
    BoulderState boulder_state;
    unsigned int waitTime = 0;
};


class Protester : public Actor
{
public:
    Protester(StudentWorld * world, int startX, int startY)
    :Actor(world, IID_PROTESTER, 60, 60, left, 1.0, 0)
    {
        setVisible(true);
        setHitpoints(5);
        
        protester_state = start;
        tickToWaitBetweenMoves = 0;
        ticks = 0;
        step = false;
    }
    
    void doSomething();
    
    ProtesterState getState() { return protester_state; }
    
    void setState(ProtesterState state)
    {
        protester_state = state;
    }
private:
    unsigned int tickToWaitBetweenMoves;
    unsigned int waitingTime = 0;
    
    unsigned int nonRestingTicks = 0;
    
    unsigned int ticks;
    
    bool step;
    ProtesterState protester_state;
};

class WaterPool : public Actor
{
private:
    int ticks;
public:
    WaterPool(StudentWorld* world, int startX, int startY)
    :Actor(world, IID_WATER_POOL, startX, startY, right, 1.0, 2)
    {
        setVisible(true);
        
        ticks = 0;
    }
    
    virtual void doSomething();
};

class SonarKit : public Actor
{
private:
    int ticks;
public:
    SonarKit(StudentWorld* world, int startX, int startY)
    :Actor(world, IID_SONAR, startX, startY, right, 1.0, 2)
    {
        setVisible(true);
        
        ticks = 0;
    }
    void doSomething();
};

class HardcoreProtester : public Protester

{};



#endif

