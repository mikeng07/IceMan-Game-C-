#include <iostream>
#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
#include <iomanip>

//actor.cpp

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
void Actor::doSomething()
{}
void IceMan::doSomething()
{
    //cout << "X: " << getX() << "| Y: " << getY() << endl;
    //    cout << "DIRECTION: " << getDirection() << endl;
    int numValue = 0;
    if (waitTime < 3)
    {
        waitTime++;
        return;
    }
    
    if (getWorld()->getKey(numValue))
    {
        switch(numValue){
        case KEY_PRESS_LEFT:
        {
            if (getDirection() != left)
            {
                setDirection(left);
            }
            else if (getX() >= 1 && getDirection() == left)
            {
                moveTo(getX() - 1, getY());
                getWorld()->deleteIce(getX(), getY());
                setDirection(left);
            }
            break;
        }
        case KEY_PRESS_UP:
        {
            if (getDirection() != up)
            {
                setDirection(up);
            }
            else if (getY() <= MAXSIZE_Y - 5 && getDirection() == up)
            {
                moveTo(getX(), getY() + 1);
                getWorld()->deleteIce(getX(), getY());
                setDirection(up);
            }
             break;
        }
        case KEY_PRESS_RIGHT:
        {
            if (getDirection() != right)
            {
                setDirection(right);
            }
            else if (getX() <= MAXSIZE_X - 5 && getDirection() == right)
            {
                moveTo(getX() + 1, getY());
                getWorld()->deleteIce(getX(), getY());
                setDirection(right);
            }
             break;
        }
        
        case KEY_PRESS_DOWN:
        {
            if (getDirection() != down)
            {
                setDirection(down);
            }
            else if (getY() >= 1 && getDirection() == down)
            {
                moveTo(getX(), getY() - 1);
                getWorld()->deleteIce(getX(), getY());
                setDirection(down);
            }
             break;
        }
        case KEY_PRESS_ESCAPE:
        {
            setHitpoints(0);
            break;
        }
        case KEY_PRESS_TAB:
        {
            //GoldNugget * gold = getWorld() -> getIceMan();
            
            if(getGold()>0)
            {
              //  gold->getGoldState();
                //GoldNugget * ga; // object
                //GoldNuggetState * g; // enum
               // gold_state = sleep; // state variable
                if (waitTime == 3) //MAKES SURE THEY CANT SPAM SPACEBAR
                {
                    waitTime=0;
                getWorld()->goldTab(getX(), getY());
                    
                }
                
            }
        break;
        }
        case KEY_PRESS_SPACE:
        {
            if(getWater()>0)
            {
            if (waitTime == 3 ) //MAKES SURE THEY CANT SPAM SPACEBAR
            {
                
                getWorld()->squirt(getX(), getY(), getDirection());
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                waitTime = 0;
            }
            }
            break;
        }
        case 'z':
        {
            if(getSonar()>0)
            {
            decreaseSonar();
            getWorld()->playSound(SOUND_SONAR);
            for (int i = 1; i < 13; i++)
            {
                getWorld()->checkItems(getX() + i, getY());
                getWorld()->checkItems(getX() - i, getY());
            }
                
            }
        break;
        }
        default: cout << "break" << endl;
        break;
        }
     }
}

bool Boulder::isStable()
{
    return getWorld()->checkIceBelow(getX(), getY());
}

void Boulder::doSomething()
{
    if (!isAlive())
    {
        //        cout << "\tBoulder at " << getX() << "|" << getY() << " is dead\n";
        return;
    }
    
    if (boulder_state != waiting && boulder_state != falling) //If boulder is in normal state
    {
        if (isStable())
        {
            boulder_state = stable;
            
            //            cout << "\tBoulder at " << getX() << "|" << getY() << " is stable\n";
        }
        else
        {
            boulder_state = waiting;
            //            cout << "\tBoulder at " << getX() << "|" << getY() << " is waiting\n";
        }
        //        cout << "-----------------------------------------\n";
    }
    else if (waitTime < 30 && boulder_state == waiting)
    {
        waitTime++;
        //        cout << "\tBoulder at " << getX() << "|" << getY() << " is waiting for " << waitTime << " ticks\n";
        return;
    }
    else if (waitTime >= 30 && boulder_state == waiting)
    {
        waitTime = 0;
        boulder_state = falling;
        //        cout << "\tBoulder at " << getX() << "|" << getY() << " is moving to a falling state\n";
        getWorld()->playSound(SOUND_FALLING_ROCK);
    }
    
    else if (boulder_state == falling)
    {
        //Move down one square each tick until it hits the bottom of the field
        
        //Runs on top of another boulder
        
        //Runs into dirt (By moving down a square the boulder would overlap
        
        //Then must set state to dead so it can be removed from game
        
        //        cout << "\tBoulder at " << getX() << "|" << getY() << " is falling\n";
        
        if (getWorld()->checkIceBelow(getX(), getY()) || getY() < 1)
        {
            boulder_state = stable;
            setHitpoints(0);
            //            cout << "\tBoulder at " << getX() << "|" << getY() << " is dead at the bottom\n";
            
            //Boulder is now stable at the bottom and waits to get cleared at the end of the current tick
        }
        
        else if (getWorld()->checkBoulderBelow(getX(), getY()))
        {
            boulder_state = stable;
            
            setHitpoints(0);
            //            cout << "\tBoulder at " << getX() << "|" << getY() << " hit another boulder\n";
        }
        
        else if (getWorld()->checkIceManBelow(getX(), getY())) //TODO: Fix radius of IceMan check
        {
            boulder_state = stable;
            
            getWorld()->setIceManHP(0);
            setHitpoints(0);
            //            cout << "\t Boulder hit IceMan\n";
        }
        
        else
        {
            moveTo(getX(), (getY() - 1)); //If there isn't any dirt below it and not at the bottom, then keep falling
        }
    }
}

void WaterPool::doSomething()
{
    IceMan* IceMan = getWorld()->getIceMan();
    int level = getWorld()->getLevel();
    //IMPLEMENTED 5/25/17 1:06 AM JOSEPH TICKS TO MAKE WATER POOL DISAPPEAR
    
    int t = max(100, 300 - 10 * level);
    cout << "GAME TICKS: " << ticks << endl;
    cout << "WATERPOOL TICKS TO GO AWAY: " << t << endl;
    if (ticks == t) {
        setHitpoints(0);
        return;
    }
    
    if (getWorld()->checkIceMan(getX(), getY(), getDirection()))
    {
        setHitpoints(0);
       getWorld()->playSound(SOUND_GOT_GOODIE);
        IceMan->addWater(5);
        getWorld()->increaseScore(100);
    }
    
    ticks++;
}

void Barrel::doSomething()
{
    if (!isAlive())
    {
        return;
    }
    
    if (getWorld()->barrelVisible(this->getX(), this->getY()))
    {
        setVisible(true);
    }
    
    
    if (this->isVisible() && getWorld()->checkIceMan(this->getX(), this->getY(), this->getDirection()))
    {
        cout << "GOT OIL" << endl;
        
        setHitpoints(0);
        
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->decreaseOil();
    }
    
}



//PROTESTER //IMPLEMENT NEXT

void Protester::doSomething()
{
    if (!isAlive() && protester_state != leaveOilField)
    {
        setState(leaveOilField);
        return;
    }
    
    if (waitingTime < tickToWaitBetweenMoves)
    {
        waitingTime++;
        cout << "Waiting for " << waitingTime << " ticks \n";
        return;
    }
    
    tickToWaitBetweenMoves = std::max(0, (3 - (1 / 4)));
    
    //LEAVING OIL FIELD HANDLER////
    if (protester_state == leaveOilField)
    {
        if (getY() <= 60 - 1)
        {
            moveTo(getX(), getY() + 1);
            
            setDirection(up);
            return;
        }
        
        if (getX() <= 60 - 1)
        {
            moveTo(getX() + 1, getY());
            setDirection(right);
            return;
        }
        if (getX() == 60 && getY() == 60)
        {
            setHitpoints(0);
            
            setState(dead);
        }
        
    }
    
    if (ticks < 7)
    {
        ticks++;
        return;
    }
    else if (waitingTime >= tickToWaitBetweenMoves && getWorld()->checkIceMan(getX(), getY(), down) && nonRestingTicks >= 15 && getWorld()->protesterFacingIceMan(getX(), getY(), getDirection()))
        //TODO: Check if 4 units from IceMan
        //Might want to change the checkIceMan function depending on what unit actually means
    {
        waitingTime = 0;
        
        nonRestingTicks = 0;
        
        getWorld()->annoyIceMan(2); //Cause 2 points of annoyance to IceMan
        
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        
        cout << "Protester yelled at IceMan\n";
        
        return;
    }
    
    int counterX = getX();
    int counterY = getY();
    
    for (int i = 0; i <= 4; i++)
    {
        if (getWorld()->getIceMan()->getX() == this->getX() || (getWorld()->getIceMan()->getY() == this->getY() && !getWorld()->checkIceMan(counterX + i, counterY, down))) //Check to see if IceMan is in a straight line from protester
        {
            
            cout << "IceMan in line of shight 4 \n";
        }
    }
    
    nonRestingTicks++;
    
    //PROTESTER GETTING A RANDOM DIRECTION
    int done = false;
    
    cout << "TICKS: " << ticks << endl;
    cout << "TICKS TO WAIT: " << tickToWaitBetweenMoves << endl;
    
    while (!done)
    {
        int randomNum = rand() % 4;
        switch (randomNum)
        {
            case 0:
                setDirection(up);
                done = true;
                
                ticks = 0;
                break;
                
            case 1:
                setDirection(down);
                done = true;
                
                ticks = 0;
                break;
                
            case 2:
                setDirection(left);
                done = true;
                
                ticks = 0;
                break;
                
            case 3:
                setDirection(right);
                done = true;
                
                ticks = 0;
                break;
                
            default:
                break;
        }
    }
    //PROTESTER MOVING
    if (ticks == 0)
    {
        if (getX() == MAXSIZE_X - 5)
        {
            moveTo(getX() + 1, getY());
            setDirection(left);
        }
        if (getX() == 0)
        {
            moveTo(getX() + 1, getY());
            setDirection(right);
        }
        if (getX() == 60)
        {
            moveTo(getX() - 1, getY());
            setDirection(left);
        }
        if (getY() == 60)
        {
            moveTo(getX(), getY() - 1);
            
            setDirection(down);
        }
        if (getY() == 0)
        {
            moveTo(getX(), getY() + 1);
            
            setDirection(up);
        }
        
        //MOVING
        if (getDirection() == right && getX() <= MAXSIZE_X - 5 && getWorld()->checkIce(getX(), getY())== false)
        {
            moveTo(getX() + 1, getY());
            
            step = false;
            return;
        }
        else
        {
            ticks++;
            step = true;
        }
        
        if (getDirection() == down && getY() >= 1 && getWorld()->checkIce(getX(), getY())==false)
        {
            moveTo(getX(), getY() - 1);
            step = false;
            
            return;
        }
        else
        {
            ticks++;
            step = true;
        }
        
        if (getDirection() == up && getY() <= MAXSIZE_Y - 5 && getWorld()->checkIce(getX(), getY())==false)
        {
            moveTo(getX(), getY() + 1);
            step = false;
            return;
        }
        else
        {
            ticks++;
            step = true;
        }
        
        if (getDirection() == left && getX() >= 1 && getWorld()->checkIce(getX(), getY())==false)
        {
            moveTo(getX() - 1, getY());
            step = false;
            return;
        }
        else
        {
            ticks++;
            step = true;
        }
    }
    
    cout << "X: " << getX() << "Y: " << getY() << endl;
    ticks++;
}

//SQUIRT // GOTTA IMPLEMENT THE REST WHEN PROTESTER IS IMPLEMENTED

void Squirt::doSomething() //BOTTOM OF MAP ERROR FIX LATER
{
    if (!isAlive())
    {
        return;
    }
    
    if (distanceTraveled == 3)
    {
        setHitpoints(0);
    }
    
    if (getWorld()->checkProtester(getX(), getY(), right))
    {
        getWorld()->annoyProtester(1);
        setHitpoints(0);
    }
    //SQUIRT MOVEMENT
    
    switch (getDirection())
    {
        case up:
            if (getWorld()->checkIce(getX(), getY() + 1) == false)
            {
                moveTo(getX(), getY() + 1);
                distanceTraveled++;
            }
            else
            {
                setHitpoints(0);
            }
            return;
            
        case down:
            if (getWorld()->checkIce(getX(), getY() - 1) == false)
            {
                moveTo(getX(), getY() - 1);
                
                distanceTraveled++;
            }
            else
            {
                setHitpoints(0);
            }
            return;
            
        case left:
            
            if (getWorld()->checkIce(getX() - 1, getY()) == false)
            {
                moveTo(getX() - 1, getY());
                distanceTraveled++;
            }
            else
            {
                setHitpoints(0);
            }
            
            return;
            
            
        case right:
            if (getWorld()->checkIce(getX() + 1, getY()) == false)
            {
                moveTo(getX() + 1, getY());
                
                distanceTraveled++;
            }
            else
            {
                setHitpoints(0);
            }
            return;
            
        default: setHitpoints(0);
            return;
    }
    
}

void SonarKit::doSomething()
{
    IceMan* IceMan = getWorld()->getIceMan();
    
    if (!isAlive())
        return;
    
    int t = max(100, 300 - 10 * getWorld()->getLevel());
    
    
    if (ticks == t) {
        setHitpoints(0);
        return;
    }
    
    cout << "GAME TICKS: " << ticks << endl;
    cout << "SONAR TICKS TO GO AWAY: " << t << endl;
    
    if (this->isVisible() && getWorld()->checkIceMan(this->getX(), this->getY(), this->getDirection()))
    {
        
        cout << "GOT SONAR KIT" << endl;
        
        setHitpoints(0);
        
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(75);
        IceMan->increaseSonar();
    }
    
    ticks++;
}
void GoldNugget::getGoldState()
{
    IceMan* IceMan = getWorld()->getIceMan();
    
    int j =KEY_PRESS_TAB;
    if(getWorld()->getKey(j) && IceMan -> getGold()>0  )
    {
        gold_state = temporary;
        getWorld()->increaseScore(0);
    }
    else
    gold_state = permanent;
        
    
}
void GoldNugget::doSomething()
    {
        IceMan* IceMan = getWorld()->getIceMan();
        
        Protester* protester = getWorld()->getProtester();
        
        if (!isAlive())
            return;
        
        if (!isVisible() && getWorld()->barrelVisible(this->getX(), this->getY()))
            setVisible(true);
        
        
        
        if (this->isVisible() && getWorld()->checkIceMan(this->getX(), this->getY(), this->getDirection()) && gold_state == permanent)
        {
            cout << "GOT GOLD NUGGET" << endl;
            
            setHitpoints(0);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            
            int j =KEY_PRESS_TAB;
            if(getWorld()->getKey(j) && IceMan -> getGold()>0  )
            {
                gold_state = temporary;
            }
            else{
                gold_state = permanent;
                getWorld()->increaseScore(10);
                IceMan->increaseGold();}
        }
        if(gold_state == temporary)
        {
            getWorld()->increaseScore(0);
            //getWorld()->checkIceMan(this->getX(), this->getY(), this->getDirection()) = false;
        }
        if (getWorld()->checkProtester(this->getX(), this->getY(), right))
        {
            cout << "PROTESTER GOT GOLD" << endl;
            
            setHitpoints(0);
            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
            getWorld()->increaseScore(25);
            
            protester->setState(rest);
        }
    }
