#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class StaticActor;
class Penelope;

class StudentWorld : public GameWorld
{//added comment
public:
    StudentWorld(std::string assetPath);
	void addActor(Actor* newActor);
	bool isFlammable(double x, double y);
     int init();
     int move();
    void cleanUp();
	void overlaps(StaticActor* checker);
	bool collision(double x, double y);
	void citizenGone();
	bool noCitizens(); //true if there are no citizens
	void levelFinished(); //set level to finished

	virtual ~StudentWorld();

private:
	std::list<Actor*> m_actors;
	int m_citizenCount; //keeps track of the number of citizens
	bool m_isLevelFinished;
	Penelope* m_player;
	
};

#endif // STUDENTWORLD_H_
//list <Actor*> :: iterator IT