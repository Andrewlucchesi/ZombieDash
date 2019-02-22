#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Penelope;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	bool collision(int x, int y);
	virtual ~StudentWorld();

private:
	std::list<Actor*> m_actors;
	Penelope* m_player;
	
};

#endif // STUDENTWORLD_H_
//list <Actor*> :: iterator IT