#include "StudentWorld.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

//gameworld.cpp

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

void StudentWorld::setDisplayText()
{
    int Level = getLevel();
    int Lives = getLives();
    int Health = m_IceMan->getHealth();
    int squirts = m_IceMan->getWater();
    int gold = m_IceMan->getGold();
    int barrelLeft = getOil();
    int sonar = m_IceMan->getSonar();
    int score = getScore();
    string s = FunctionFormat( Level, Lives,  Health, squirts, gold, barrelLeft, sonar, score);
    
    GameWorld::setGameStatText(s);
}
string StudentWorld::FunctionFormat(int Level, int Lives, int Hlth, int Wtr, int Gld, int Oil, int Sonar, int Scr)
{
    string text = " Level: " + to_string(Level) + "  Lives : " + to_string(Lives) + "  Hlth : " + to_string(Hlth) + "%  Wtr : " + to_string(m_IceMan->getWater() ) +
    " Gld: " + to_string(Gld) + "  Oil Left: "+ to_string(Oil)+"  Sonar: " + to_string(Sonar) + "  Scr: " + to_string(Scr);
    return text;
}
int StudentWorld::init()
{
    m_IceMan = new IceMan(this);
    
    srand(time(NULL));
    
    int randX = rand() % MAXSIZE_X;
    int randY = rand() % MAX_BOULDER_Y + Y_OFFSET;
    
    int barrelX = rand() % MAXSIZE_X;
    int barrelY = rand() % MAXSIZE_Y + Y_OFFSET;
    
    int pX = 60;
    int pY = 60;
    
    int gX = rand() % MAXSIZE_X;
    int gY = rand() % MAXSIZE_Y - Y_OFFSET;
    
    int Pnum = min( 15.0, 2 + current_level_number * 1.5);
    
    for (int i = 0; i < Pnum; i++)
    {
        protester.push_back(new Protester(this, pX, pY));
    }
    
    L = min(2 + current_level_number, 21); //BARREL NUMBER SPAWN
    int G = max(5- current_level_number/2, 2);
    int B = min(current_level_number / 2 + 2, 9); // boulder
    
    //ice initialization
    for (int i = 0; i < MAXSIZE_X; i++) //VERTICAL AXIS (X-Axis)
    {
        for (int j = 0; j < (MAXSIZE_Y); j++) //HORIZONTAL AXIS (Y-Axis)
        {
            m_actor[i][j] = 0; //Initializing all actors to 0 in order to check if there is an actual actor there
        }
    }
    
    
    for (int x = 0; x < MAXSIZE_X; x++) //VERTICAL AXIS (X-Axis)
    {
        for (int y = 0; y < (MAXSIZE_Y); y++) //HORIZONTAL AXIS (Y-Axis)
        {
            m_Ice[x][y] = new Ice(this, x, y);
            
            if (x > MINESHAFT_START_LEFT && x < MINESHAFT_STOP_RIGHT) //If below the mineshaft
            {
                if (y > MINESHAFT_BOTTOM)
                {
                    m_Ice[x][y]->setVisible(false);
                }
                
            }
            if (y > MAXSIZE_Y - 5)
            {
                m_Ice[x][y]->setVisible(false);
            }
            
        }
        
    }
    
    //BARREL HANDLER
    for (int i = 0; i < L; i++)
    {
        if (barrelX < (MAXSIZE_X - X_BOUND_RIGHT) && barrelY < (MAXSIZE_Y - Y_BOUND_TOP) && ItemDoesNotExist(barrelX, barrelY))
        {
            m_actor[barrelX][barrelY] = new Barrel(this, barrelX, barrelY);
            barrelX = rand() % MAXSIZE_X;
            barrelY = rand() % MAXSIZE_Y + Y_BOUND_TOP;
        }
        else
        {
            barrelX = rand() % MAXSIZE_X;
            barrelY = rand() % MAXSIZE_Y + Y_BOUND_TOP;
            i--;
        }
        
    }
    
    //BOULDER HANDLER
    for (int i = 0; i < B; i++)
    {
        if (randX < (MAXSIZE_X - X_BOUND_RIGHT) && randY < (MAXSIZE_Y - Y_BOUND_TOP) && ItemDoesNotExist(randX, randY) && distanceBtwObj(randX, randY))
        {
            m_actor[randX][randY] = new Boulder(this, randX, randY);
            
            deleteIce(randX, randY);
            
            randX = rand() % MAXSIZE_X;
            randY = rand() % MAX_BOULDER_Y + Y_OFFSET;
        }
        else
        {
            randX = rand() % MAXSIZE_X;
            randY = rand() % MAX_BOULDER_Y + Y_OFFSET;
            i--;
        }
    }
    
    //GOLD NUGGET HANDLER
    for (int i = 0; i < L; i++)
    {
        if (gX < (MAXSIZE_X - X_BOUND_RIGHT) && gY < (MAXSIZE_Y - Y_BOUND_TOP) && ItemDoesNotExist(gX, gY))
        {
            m_actor[gX][gY] = new GoldNugget(this, gX, gY);
            gX = rand() % MAXSIZE_X;
            gY = rand() % MAXSIZE_Y + Y_BOUND_TOP;
        }
        else
        {
            gX = rand() % MAXSIZE_X;
            gY = rand() % MAXSIZE_Y + Y_BOUND_TOP;
            i--;
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::ItemDoesNotExist(int itemX, int itemY)
{
    bool objectDoesNotExist = false;
    
    for (int x = itemX; x < (itemX + 4); x++)
    {
        for (int y = itemY; y < (itemY + 4); y++)
        {
            if (m_Ice[x][y]->isVisible())
            {
                objectDoesNotExist = true;
            }
            else
            {
                objectDoesNotExist = false;
                return objectDoesNotExist;
            }
        }
    }
    return objectDoesNotExist;
}


void StudentWorld::deleteIce(int xPassed, int yPassed) //DOESNT ACTUALLY DELETE JUST SETS VISIBLE //WILL CLEAR LATER IN THE CLEAR ALL FUNCTION - Joseph
{
    bool dirtDeleted = false;
    
    for (int x = xPassed; x < xPassed + 4; x++)
    {
        for (int y = yPassed; y < yPassed + 4; y++)
        {
            if (m_Ice[y][x] != nullptr)
            {
                if ((x < MAXSIZE_Y) && (y < MAXSIZE_X))
                {
                    m_Ice[x][y]->setVisible(false);
                    
                    dirtDeleted = true;
                   
                }
            }
            else
            {
                return;
            }
        }
        
    }
    
    if (dirtDeleted == true)
    {
        playSound(SOUND_DIG);
    }
}


bool StudentWorld::checkIceBelow(int xPassed, int yPassed)
{
    bool IceFound= true;
    for (int xToCheck = xPassed; xToCheck < xPassed + 4; xToCheck++)
    {
        if (m_Ice[xToCheck][yPassed - 1]->isVisible())
        {
            IceFound= true;
            break;
        }
        else
        {
            IceFound= false;
        }
    }
    
    return IceFound;
}

bool StudentWorld::checkIce(int xPassed, int yPassed)
{
    bool IceFound= true;
    if (xPassed > 0 && xPassed < MAXSIZE_X && yPassed > 0 && yPassed < MAXSIZE_Y)
    {
        if (m_actor[xPassed][yPassed] != 0 || m_Ice[xPassed][yPassed]->isVisible())
        {
            IceFound= true;
        }
        else
        {
            IceFound= false;
        }
    }
    else
    {
        IceFound= true;
    }
    
    return IceFound;
}


bool StudentWorld::checkBoulderBelow(int xPassed, int yPassed)
{
    bool boulderFound = false;
    
    for (int xToCheck = xPassed; xToCheck < xPassed + 4; xToCheck++)
    {
        if (m_actor[xToCheck][yPassed - 4] != 0)
        {
            if (m_actor[xToCheck][yPassed - 4]->isVisible())
            {
                boulderFound = true;
                
                return true;
            }
        }
        else
        {
            continue;
        }
    }
    return false;
}



bool StudentWorld::checkIceManBelow(int xPassed, int yPassed)
{
    bool IceManFound = false;
    
    //Check Above
    for (int i = 1; i < 5; i++)
    {
        if (xPassed + i == m_IceMan->getX() && yPassed == m_IceMan->getY())
            return true;
        
        else if (xPassed - i == m_IceMan->getX() && yPassed == m_IceMan->getY())
            return true;
        
        else if (xPassed == m_IceMan->getX() && yPassed + i == m_IceMan->getY())
            return true;
        
        else if (xPassed == m_IceMan->getX() && yPassed - i == m_IceMan->getY())
            return true;
    }
    return IceManFound;
}


void StudentWorld::setIceManHP(int hitPoints)
{
    m_IceMan->setHitpoints(hitPoints);
    
    cout << "\tIceMan is dead\n";
}

void StudentWorld::removeDeadActors()
{
    for (int i = 0; i < MAXSIZE_X; i++)
    {
        for (int j = 0; j < MAXSIZE_Y; j++)
        {
            if (m_actor[i][j] != 0 && !m_actor[i][j]->isAlive())
            {
                m_actor[i][j]->setVisible(false);
                
                m_actor[i][j] = 0;
                
                cout << "\tDeleted actor at " << i << " | " << j << endl;
                
                delete m_actor[i][j];
            }
            else
            {
                continue;
            }
        }
    }
    
    for (int h = 0; h < protester.size(); h++)
    {
        if (protester[h] != nullptr && protester[h]->getState() == dead)
        {
            cout << "\tDeleted protester at at " << protester[h]->getX() << " | " << protester[h]->getY() << endl;
            protester[h]->setVisible(false);
            
            playSound(SOUND_PROTESTER_GIVE_UP);
            
            delete protester[h];
            
            protester[h] = nullptr;
        }
        else
        {
            continue;
        }
    }
    
    if (!m_IceMan->isAlive())
    {
        m_IceMan->setVisible(false);
        //m_IceMan = 0;
        
        cout << "\tDeleted IceMan at " << m_IceMan->getX() << " | " << m_IceMan->getY() << endl;
        delete m_IceMan;
    }
    
}



int StudentWorld::move()
{
    cout << m_actor[0][60] << endl;
    if (m_lives == 0)
    {
        exit(0);
    }
    ticks++;
    
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    StudentWorld::setDisplayText();
    m_IceMan->doSomething(); //IceMan doSomething
    
    int G = current_level_number * 25 + 300; //1 in G CHANCE THAT A WATERPOOL WILL SPAWN BASED ON TICKS
    
    if (ticks%G == 0) //IF TICKS == IS G CHANCE THEN SPAWN WATERPOOL OR SONAR KIT
    {
        int x = rand() % MAXSIZE_X;
        int y = rand() % MAXSIZE_Y - Y_BOUND_TOP;
        
        
        int s = rand() % 5; //5 RANDOM NUMBERS (SONAR KIT)
        
        if (s == 4) //IF 1/5 THEN SPAWN SONAR KIT
        {
            m_actor[0][60] = new SonarKit(this, 0, 60);
        }
        else //IF NOT THEN SPAWN A WATER POOL (4/5 CHANCE)
        {
            if (!m_Ice[x][y]->isVisible())
            {
                m_actor[x][y] = new WaterPool(this, x, y);
            }
        }
    }
    
    bool itemFound = checkIce(m_IceMan->getX(), m_IceMan->getY());
    
    if (itemFound)
    {
        cout << "THERE IS AN ITEM FOUND ON IceMan's POSITION" << endl;
    }
    
    for (int i = 0; i < MAXSIZE_X; i++)
    {
        for (int j = 0; j < MAXSIZE_Y; j++)
        {
            if (m_actor[i][j] != 0)
            {
                ticks++;
                m_actor[i][j]->doSomething(); //Call doSomething for all actors
            }
            else
            {
                continue;
            }
        }
    }
    
    for (size_t h = 0; h < protester.size(); h++)
    {
        if (protester[h] != nullptr)
        {
            ticks++;
            protester[h]->doSomething();
        }
        else
        {
            continue;
        }
    }
    
    if (!m_IceMan->isAlive())
    {
        m_lives--;
        ticks++;
        m_IceMan->setHitpoints(1);
        m_IceMan->setVisible(true);
        
        return GWSTATUS_PLAYER_DIED;
    }
    
    removeDeadActors(); //Checks every tick to remove the actors that are dead
    
    if (m_oilCollected == L)
    {
        playSound(SOUND_FINISHED_LEVEL);
        
        current_level_number++;
        ticks++;
        cleanUp();
        
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::goldTab(int xPassed, int yPassed)
{
    m_actor[xPassed][yPassed] = new GoldNugget(this, xPassed, yPassed);
    m_actor[xPassed][yPassed] -> setVisible(true);
    m_IceMan->decreaseGold();
    
}

void StudentWorld::squirt(int xPassed, int yPassed, IceMan::Direction dir)
{
    cout << "Water: " << m_IceMan->getWater() << endl;
    
    if (xPassed > -1 && xPassed < MAXSIZE_X && yPassed > -1 && yPassed < MAXSIZE_Y)
    {
        if (m_IceMan->getWater() > 0)
        {
            switch (dir)
            {
                case IceMan::up:
                    m_actor[xPassed][yPassed] = new Squirt(this, xPassed, yPassed + 4, IceMan::up);
                    
                    m_actor[xPassed][yPassed]->setDirection(IceMan::up);
                    playSound(SOUND_PLAYER_SQUIRT);
                    m_IceMan->reduceWater();
                    return;
                    
                case IceMan::down:
                    m_actor[xPassed][yPassed] = new Squirt(this, xPassed, yPassed - 4, IceMan::down);
                    m_actor[xPassed][yPassed]->setDirection(IceMan::down);
                    playSound(SOUND_PLAYER_SQUIRT);
                    m_IceMan->reduceWater();
                    return;
                    
                case IceMan::right:
                    m_actor[xPassed][yPassed] = new Squirt(this, xPassed + 4, yPassed, IceMan::right);
                    m_actor[xPassed][yPassed]->setDirection(IceMan::right);
                    playSound(SOUND_PLAYER_SQUIRT);
                    m_IceMan->reduceWater();
                    return;
                    
                case IceMan::left:
                    m_actor[xPassed][yPassed] = new Squirt(this, xPassed - 4, yPassed, IceMan::left);
                    m_actor[xPassed][yPassed]->setDirection(IceMan::left);
                    playSound(SOUND_PLAYER_SQUIRT);
                    m_IceMan->reduceWater();
                    return;
                    
                default:
                    m_actor[xPassed][yPassed]->setHitpoints(0);
                    return;
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
    
}


bool StudentWorld::checkIceMan(int xPassed, int yPassed, IceMan::Direction dir)
{
    bool IceManFound = false;
    
    //Check Above
    for (int i = 1; i < 4; i++)
    {
        if (xPassed + i == m_IceMan->getX() && yPassed == m_IceMan->getY())
        {return true;
            
        }
        
        else if (xPassed - i == m_IceMan->getX() && yPassed == m_IceMan->getY())
        {return true;
            
        }
        
        else if (xPassed == m_IceMan->getX() && yPassed + i == m_IceMan->getY())
        {return true;
        }
        else if (xPassed == m_IceMan->getX() && yPassed - i == m_IceMan->getY())
        {   return true;
            
        }
    }
    
    return IceManFound;
}

bool StudentWorld::barrelVisible(int xPassed, int yPassed)
{
    bool IceManFound = false;
    //Check Above
    
    for (int i = 1; i < 5; i++)
    {
        if (xPassed + i == m_IceMan->getX() && yPassed == m_IceMan->getY())
            return true;
        
        else if (xPassed - i == m_IceMan->getX() && yPassed == m_IceMan->getY())
            return true;
        
        else if (xPassed == m_IceMan->getX() && yPassed + i == m_IceMan->getY())
            return true;
        
        else if (xPassed == m_IceMan->getX() && yPassed - i == m_IceMan->getY())
            return true;
        
    }
    return IceManFound;
}

bool StudentWorld::checkProtester(int xPassed, int yPassed, Protester::Direction dir)
{
    bool protesterFound = false;
    //Check Above
    
    for (int i = 0; i < protester.size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (protester[i] != 0)
            {
                
                if (xPassed + j == protester[i]->getX() && yPassed == protester[i]->getY())
                {
                    setIndex(i);
                    
                    protesterFound = true;
                }
                
                if (xPassed - j == protester[i]->getX() && yPassed == protester[i]->getY())
                {
                    setIndex(i);
                    
                    protesterFound = true;
                }
                
                if (xPassed == protester[i]->getX() && yPassed + j == protester[i]->getY())
                {
                    setIndex(i);
                    
                    protesterFound = true;
                }
                
                if (xPassed == protester[i]->getX() && yPassed - j == protester[i]->getY())
                {
                    setIndex(i);
                    
                    protesterFound = true;
                }
                
                if (xPassed == protester[i]->getX() && yPassed == protester[i]->getY())
                {
                    setIndex(i);
                    
                    protesterFound = true;
                }
            }
            
            else
            {
                continue;
            }
            
        }
        
    }
    
    return protesterFound;
}

bool StudentWorld::protesterCheckIceMan(int xPassed, int yPassed)
{
    bool IceManFound = false;
    
    //Check Above
    for (int i = 1; i < 8; i++) //Checking if IceMan is 4 units away
    {
        if (xPassed + i == m_IceMan->getX() && yPassed == m_IceMan->getY())
            return true;
        
        else if (xPassed - i == m_IceMan->getX() && yPassed == m_IceMan->getY())
            return true;
        
        else if (xPassed == m_IceMan->getX() && yPassed + i == m_IceMan->getY())
            return true;
        
        else if (xPassed == m_IceMan->getX() && yPassed - i == m_IceMan->getY())
            return true;
        
    }
    
    return IceManFound;
}



bool StudentWorld::protesterFacingIceMan(int xPassed, int yPassed, Protester::Direction dir)
{
    //Protester is looking to the
    switch (dir)
    {
        case Protester::up:
            return yPassed < m_IceMan->getY();
            
        case Protester::down:
            return yPassed > m_IceMan->getY();
            
        case Protester::left:
            return xPassed > m_IceMan->getX();
            
        case Protester::right:
            return xPassed < m_IceMan->getX();
            
        default:
            
            return false;
    }
    
}

void StudentWorld::checkItems(int xPassed, int yPassed)
{
    for (int x = 0; x < MAXSIZE_X; x++)
    {
        for (int y = 0; y < MAXSIZE_Y; y++)
        {
            
            for (int i = 1; i < 13; i++)
            {
                
                if (m_actor[x][y] != 0)
                {
                    
                    if (xPassed + i == m_actor[x][y]->getX() && yPassed == m_actor[x][y]->getY())
                        m_actor[x][y]->setVisible(true);
                    
                    else if (xPassed - i == m_actor[x][y]->getX() && yPassed == m_actor[x][y]->getY())
                        m_actor[x][y]->setVisible(true);
                    
                    else if (xPassed == m_actor[x][y]->getX() && yPassed + i == m_actor[x][y]->getY())
                        m_actor[x][y]->setVisible(true);
                    
                    else if (xPassed == m_actor[x][y]->getX() && yPassed - i == m_actor[x][y]->getY())
                        m_actor[x][y]->setVisible(true);
                }
                else
                {
                    continue;
                    
                }
                
            }
            
        }
    }
}


bool StudentWorld::distanceBtwObj(int randX, int randY)
{
    int objX, objY;
    
    double distance = 0;
    
    for (size_t x = 0; x < MAXSIZE_X; x++)
    {
        for (size_t y = 0; y < MAXSIZE_Y; y++)
        {
            if (m_actor[x][y] != 0)
            {
                objX = m_actor[x][y]->getX();
                objY = m_actor[x][y]->getY();
                distance = sqrt(pow((randX - objX), 2) + pow((randY - objY), 2));
                
                if (distance <= 6)
                {
                    return false;
                }
                
            }
            
        }
        
    }
    return true;
}

void StudentWorld::cleanUp()
{
    for (int i = 0; i < MAXSIZE_X; i++)
    {
        for (int j = 0; j < MAXSIZE_Y; j++)
        {
            if (m_actor[i][j] != 0)
            {
                m_actor[i][j]->setVisible(false);
                
                m_actor[i][j] = 0;
                
                cout << "\tDeleted actor at " << i << " | " << j << endl;
                
                delete m_actor[i][j];
            }
            else
            {
                continue;
            }
            
        }
        
    }
    
    for (int i = 0; i < MAXSIZE_X; i++)
    {
        for (int j = 0; j < MAXSIZE_Y; j++)
        {
            if (m_Ice[i][j] != 0)
            {
                
                m_Ice[i][j]->setVisible(false);
                
                m_Ice[i][j] = 0;
                
                delete m_Ice[i][j];
            }
            else
            {
                continue;
            }
            
        }
        
    }
    
    for (int h = 0; h < protester.size(); h++)
    {
        if (protester[h] != nullptr)
        {
            protester[h]->setVisible(false);
            cout << "\tDeleted protester at at " << protester[h]->getX() << " | " << protester[h]->getY() << endl;
            
            delete protester[h];
            
            protester[h] = nullptr;
        }
        else
        {
            continue;
        }
        
    }
    if (m_IceMan != 0)
    {
        m_IceMan->setVisible(false);
        
        m_IceMan = 0;
        
        delete m_IceMan;
    }
    else
    {
        return;
    }
    
}
