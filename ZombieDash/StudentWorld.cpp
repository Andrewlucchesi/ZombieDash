#include "StudentWorld.h"
#include "GameConstants.h"
#include "actor.h"
#include "level.h"
#include <iostream>
#include <sstream>
#include <string>
#include <list>
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
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			Level::MazeEntry i = lev.getContentsOf(x, y);
			switch (i)
			{
			case Level::empty: break;

			case Level::player: 
				m_player = new Penelope(x*SPRITE_WIDTH, y*SPRITE_HEIGHT, this);
				break;

			case Level::wall: 
				Actor* wall = new Wall(x*SPRITE_WIDTH, y*SPRITE_HEIGHT, this);
				m_actors.push_back(wall);
				break;
			}
		}
	}

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	list <Actor*> ::iterator It;
	for (It = m_actors.begin(); It != m_actors.end(); It++)
	{
		(*It)->doSomething();
	}
	m_player->doSomething();
	if (m_player->isAlive())
	{
		return GWSTATUS_CONTINUE_GAME;
	}
	//else deal with death later

}

bool StudentWorld::collision(int destX, int destY)
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

void StudentWorld::cleanUp()
{
	m_actors.clear();
	delete m_player;
	
}

StudentWorld::~StudentWorld()
{
	StudentWorld::cleanUp();
}

