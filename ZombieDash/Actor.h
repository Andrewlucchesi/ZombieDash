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
	//virtual void infect(); //Default for infect function will be to do nothing. Only humans can be infected
	//virtual void die();

	//virtual void escape(); //will be called whenever an actor is overlapping with the exit. Only humans effected
	StudentWorld* world();
	virtual void doSomething() = 0;
	virtual ~Actor();
private:
	bool m_alive;
	StudentWorld* m_world;
};

class StaticActor :public Actor
{
public:
	StaticActor(int imageID, int x, int y, StudentWorld* world, Direction dir = right, int depth = 0);
	virtual bool insideBoundingBox(int x, int y); //Redefine so bounding box checks always return false
	virtual void tryMoving( Direction dir, int dist);
	//virtual void burn(); //The default for Static is to not burn(flames, vomit, pits dont burn)
	virtual ~StaticActor();
private:
};

class Flame : public StaticActor
{
public:
private:
};

class Vomit:public StaticActor
{
public:
private:
};

class pit :public StaticActor
{
public:
private:
};

class Landmine: public StaticActor
{
public:
private:
};

class Goodie : public StaticActor
{
public:
private:
};

class VaccineGoodie :public StaticActor
{
public:
private:
};

class LandmineGoodie :public StaticActor
{
public:
private:
};

class GasGoodie :public StaticActor
{
public:
private:
};

class Wall : public StaticActor
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
	//  virtual void burn(); // all beings are burned the same way, call the kill function
		virtual void tryMoving( Direction dir, int dist);
		virtual ~Being();
private:
};

class Human : public Being
{
public:Human(int imageID, int x, int y, StudentWorld* world);
	   virtual ~Human();
	   void tryMoving( Direction dir, int dist);
	   //virtual void escape();
	   //virtual void infect(); //Allow for humans to be infected. Will set infection count to one, which will begin counting
	   
private:
	//infection variable
};


class Penelope : public Human
{
public:
	Penelope(int startX, int startY, StudentWorld *world);
	void tryMoving( Direction dir, int dist);
	virtual void kill();
	virtual void doSomething();
	virtual ~Penelope();
private:
	//Will have mines, flames, vaccines later
};

class Citizen : public Human
{
public:
Citizen(int x, int y, StudentWorld* world);
virtual void doSomething();
virtual void tryMoving(Direction dir, int dist);
virtual void kill();
virtual	~Citizen();

private:
};

class Zombie : public Being
{
public:
private:
};

class DumbZombie : public Zombie
{
public:
private:
};

class SmartZombie : public Zombie
{
public:
private:
};

#endif // ACTOR_H_
