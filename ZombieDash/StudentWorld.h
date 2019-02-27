#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class StaticActor;
class Penelope;
enum goodietype { vaccine, mine, gas }; //used to tell studentworld what goodie to give Penelope

class StudentWorld : public GameWorld
{//added comment
public:
    StudentWorld(std::string assetPath);
     int init();
     int move();
    void cleanUp();
	void overlaps(StaticActor* checker);
	bool collision(int x, int y);
	bool noCitizens(); //true if there are no citizens
	void levelFinished(); //set level to finished

	void giveGoodies(goodietype goods);
	virtual ~StudentWorld();

private:
	std::list<Actor*> m_actors;
	int m_citizenCount; //keeps track of the number of citizens
	bool m_isLevelFinished;
	Penelope* m_player;
	
};

#endif // STUDENTWORLD_H_
//list <Actor*> :: iterator IT