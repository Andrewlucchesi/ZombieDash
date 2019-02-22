#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int imageID, int x, int y, Direction dir, int depth); //direction needed for projectiles, obv for everything else
	virtual bool insideBoundingBox(int x, int y); //checks to see if the specified coordinate is within the Actor's bounding box
	//bool isOverlapping(int x, int y);
	virtual void doSomething() = 0;
	virtual ~Actor();
private:

};

class Wall : public Actor
{
public:
	Wall(int x, int y);
	virtual void doSomething();
private:
};

class Being : public Actor
{
public: Being(int imageID, int x, int y, Direction dir, int depth );
	//	virtual void escape() = 0;
		bool isAlive();
	//	virtual void kill();
private:
bool m_alive;

};
class Penelope : public Being
{
public:
	Penelope(int startX, int startY, StudentWorld *world);
	virtual void doSomething();
	~Penelope();
private:
	StudentWorld* m_world;
	//Will have mines, flames, vaccines later
};


#endif // ACTOR_H_
