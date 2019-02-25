#include "StudentWorld.h"
#include "GameConstants.h"
#include "actor.h"
#include "level.h"
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{

}

int StudentWorld::init()
{
	//must create Pene according to first level file
	Level lev(StudentWorld::assetPath());
	int levNum = StudentWorld::getLevel();
	ostringstream oss;
	oss.setf(ios::fixed);
	oss << "level";
	if(levNum < 10 )
	{
		oss << "0";
	}
	oss << levNum << ".txt";
	string levText = oss.str();
	Level::LoadResult result = lev.loadLevel(levText);
	if (levNum > 99 || result == Level::load_fail_file_not_found)
	{
		cerr << "Cannot find file " << levText << endl;
		return GWSTATUS_PLAYER_WON;
	}

	else if (result == Level::load_fail_bad_format)
	{
		cerr << "Improper Format " << levText << endl;
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level " << levText << endl;
	}
	
	//Initialization step
	m_citizenCount = 0;
	m_isLevelFinished = false;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			Level::MazeEntry i = lev.getContentsOf(x, y);
			Actor* entry;
			switch (i)
			{
			case Level::empty: break;

			case Level::player: 
				m_player = new Penelope(x*SPRITE_WIDTH, y*SPRITE_HEIGHT, this);
				break;

			case Level::wall: 
				entry = new Wall(x*SPRITE_WIDTH, y*SPRITE_HEIGHT, this);
				m_actors.push_back(entry);
				break;

			case Level::exit:
				entry = new Exit(x*SPRITE_WIDTH, y*SPRITE_HEIGHT, this);
				m_actors.push_back(entry);
				break;
			}
		}
	}

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
     // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	list <Actor*> ::iterator It;
	for (It = m_actors.begin(); It != m_actors.end(); It++)
	{
		(*It)->doSomething();
	}
	m_player->doSomething();
	


	//Update GameStatText
	ostringstream oss;
	oss.setf(ios::fixed);
	oss << "Score: ";
	oss << setfill('0') << setw(6) << getScore();
	oss << "  Level: ";
	oss << setfill('0') << setw(2) << getLevel();
	oss << "  Lives: " << getLives();
	oss << "  Vaccines: " << m_player->getVaccines() <<"  Flames: " << m_player->getFlames() << "  Mines: " << m_player->getMines() << "  Infected: " << m_player->getInfect();
	
	setGameStatText(oss.str());

//Check for exit conditions
	if (m_isLevelFinished && m_player->isAlive()) //If player has exited, the code will move on to next level 
	{
		return GWSTATUS_FINISHED_LEVEL;
	}
	else if (m_player->isAlive())
	{
		return GWSTATUS_CONTINUE_GAME;
	}
	else
	{
		decLives(); //Do we have to check for lives or does framework do it for us?
		return GWSTATUS_PLAYER_DIED;
	}
	//else deal with death later
}

bool StudentWorld::collision(int destX, int destY) //Check to see if the given point is inside of any Actors
{
	list<Actor*> ::iterator It;
	for (It = m_actors.begin(); It != m_actors.end() ; It++)
	{
		if ((*It)->insideBoundingBox(destX, destY) || (*It)->insideBoundingBox(destX + SPRITE_WIDTH - 1, destY)
			|| (*It)->insideBoundingBox(destX, destY + SPRITE_HEIGHT - 1) || (*It)->insideBoundingBox(destX + SPRITE_HEIGHT - 1, destY + SPRITE_HEIGHT - 1) ) //Check to see if corners of moving box will be inside of a boundingbox
		{
			return true;
		}
	}
}

bool StudentWorld::noCitizens()
{
	return(m_citizenCount == 0);
}

void StudentWorld::levelFinished()
{
	m_isLevelFinished = true;
}

void StudentWorld::overlaps(StaticActor* checker) //Looks to see if there are any overlaps at the given coordinate point

{
	list<Actor*> ::iterator It;
	for (It = m_actors.begin(); It != m_actors.end(); It++)
	{
		if((*It)->isOverlapping(checker->getX(), checker->getY())) //Individually checking each actor for coordinates overlapping with the checkers
		{
			checker->doThisThingWhileOverlapping(*It); //Calls the checker's unique overlap action, with a pointer to the target actor
		}
	}
	if (m_player->isOverlapping(checker->getX(), checker->getY()))
	{
		checker->doThisThingWhileOverlappingPlayer(m_player);
	}

}

void StudentWorld::cleanUp()
{
	list<Actor*> ::iterator It;
	for (It = m_actors.begin(); It != m_actors.end(); It++)
		delete *It;
	m_actors.clear();
	delete m_player;
	
}

StudentWorld::~StudentWorld()
{
	StudentWorld::cleanUp();
}

