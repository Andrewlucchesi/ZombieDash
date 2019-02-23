#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int imageID, int x, int y, StudentWorld* world, Direction dir, int depth); //direction needed for projectiles, obv for everything else
	virtual bool insideBoundingBox(int x, int y); //checks to see if the specified coordinate is within the Actor's bounding box
	bool isOverlapping(int x, int y);
	virtual void tryMoving(int dir, int dist) = 0;
	bool isAlive();
	//virtual void burn() = 0;
	//virtual void infect() = 0;
	StudentWorld* world();
	virtual void doSomething() = 0;
	virtual ~Actor();
private:
	bool m_alive;
	StudentWorld* m_world;
};

class Static :public Actor
{
public:
	Static(int imageID, int x, int y, StudentWorld* world, Direction dir = right, int depth = 0);
	virtual bool insideBoundingBox(int x, int y); //Redefine so bounding box checks always return false
	virtual void tryMoving( Direction dir, int dist);
private:
};

//class vomit
//class pit
//class mine

//class goodie
//class vaccine goodie
//class mine goodie
//class gas goodie

class Wall : public Static
{
public:
	Wall(int x, int y, StudentWorld* world);
	virtual void doSomething();
	virtual bool insideBoundingBox(int x, int y); //Wall will need to use default definition
	virtual ~Wall();
private:
};

class Being : public Actor
{
public: Being(int imageID, int x, int y, StudentWorld* world);
	//	virtual void kill();
		virtual void tryMoving( Direction dir, int dist);
		virtual ~Being();
private:
};

class Human : public Being
{
public:Human(int imageID, int x, int y, StudentWorld* world);
	   virtual ~Human();
	   void tryMoving( Direction dir, int dist);
	   //escape function
	   //Become infected
private:
	//infection variable
};


class Penelope : public Human
{
public:
	Penelope(int startX, int startY, StudentWorld *world);
	void tryMoving( Direction dir, int dist);
	virtual void doSomething();
	virtual ~Penelope();
private:
	//Will have mines, flames, vaccines later
};

//class citizen


//class zombie
//class dumb zombie
//class smart zombie


#endif // ACTOR_H_
