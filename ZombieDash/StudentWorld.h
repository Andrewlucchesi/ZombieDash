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
	bool collision(double x, double y, Actor* Caller);
	void citizenGone();
	bool noCitizens(); //true if there are no citizens
	void levelFinished(); //set level to finished

	 // Is there something at the indicated location that might cause a
	  // zombie to vomit (i.e., a human)?
	bool isZombieVomitTriggerAt(double x, double y);

	// Return true if there is a living human, otherwise false.  If true,
	// otherX, otherY, and distance will be set to the location and distance
	// of the human nearest to (x,y).
	///bool locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance);

	// Return true if there is a living zombie, false otherwise.  If true,
	// otherX, otherY and distance will be set to the location and distance
	// of the one nearest to (x,y).
	bool locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance);
	bool locatePlayer(double x, double y, double& otherX, double& otherY, double& distance);
	virtual double calculateDistance(double x1, double y1, double x2, double y2);
	virtual ~StudentWorld();

private:
	std::list<Actor*> m_actors;
	int m_citizenCount; //keeps track of the number of citizens
	bool m_isLevelFinished;
	Penelope* m_player;
	
};



#endif // STUDENTWORLD_H_
//list <Actor*> :: iterator IT