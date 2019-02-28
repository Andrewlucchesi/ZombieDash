#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Penelope;

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int imageID, double x, double y, StudentWorld* world, Direction dir, int depth); //direction needed for projectiles, obv for everything else
	virtual bool insideBoundingBox(double x, double y); //checks to see if the specified coordinate is within the Actor's bounding box
	virtual bool blocksFlames();
	bool isOverlapping(double x, double y); //Checks to see if the specified coordinate is overlapping with actor

	virtual void tryMoving(Direction dir);
	bool isAlive();
	virtual void burnfall() = 0;
	virtual void tryToInfect(); //Default for infect function will be to do nothing. Only humans can be infected
	virtual bool canTriggerMines();
	void die(); //Sets the actor's status to dead
	virtual bool tryToEscape(); //will be called whenever an actor is overlapping with the exit. Only humans effected
	StudentWorld* world();
	void doSomething();
	virtual ~Actor();
private:
	bool m_alive;
	StudentWorld* m_world;
	virtual void classSpecificAction() = 0;
}; 

class StaticActor :public Actor
{
public:
	StaticActor(int imageID, double x, double y, StudentWorld* world, Direction dir = right, int depth = 0);
	virtual bool insideBoundingBox(double x, double y); //Redefine so bounding box checks always return false
	virtual void doThisThingWhileOverlapping(Actor* target)=0;
	virtual void doThisThingWhileOverlappingPlayer(Penelope* player) = 0;
	virtual void burnfall(); //The default for Static is to not burn(flames, vomit, pits dont burn), although goodies will
	virtual ~StaticActor();
private:
	virtual void classSpecificAction(); 
};

class Exit : public StaticActor
{
public:
	Exit(double x, double y, StudentWorld* world);
	virtual void doThisThingWhileOverlapping(Actor* target);
	virtual void doThisThingWhileOverlappingPlayer(Penelope* player);
	virtual bool blocksFlames();
	virtual ~Exit(); //Needs to block flames

private:

};

class Pit :public StaticActor
{
public:
	Pit(double x, double y, StudentWorld* world);
	virtual void doThisThingWhileOverlapping(Actor * target);
	virtual void doThisThingWhileOverlappingPlayer(Penelope* player);
	virtual ~Pit();

private:
};

class tempStaticActor : public StaticActor
{
public:
tempStaticActor(int imageID, double x, double y, StudentWorld* world, Direction dir, int depth = 0);
virtual ~tempStaticActor();
virtual void doThisThingWhileOverlappingPlayer(Penelope* player);
protected:
int getAge();
private:
	virtual void classSpecificAction();
	virtual bool withinLifespan();  //Does same thing for Flame and vommit. For mines, sets the mine to active after certain number of ticks
	int m_Age;
};

class Flame : public tempStaticActor
{
public:
	Flame(double x, double y, StudentWorld* world, Direction dir);
	virtual void doThisThingWhileOverlapping(Actor* target);
	virtual bool canTriggerMines();
	virtual ~Flame();


private:
};

class Vomit : public tempStaticActor
{
public:
	Vomit(double x, double y, StudentWorld* world, Direction dir);
	virtual void doThisThingWhileOverlapping(Actor* target);
	~Vomit();
private:
};



class Landmine: public tempStaticActor
{
public:
	Landmine(double x, double y, StudentWorld* world);
	virtual void doThisThingWhileOverlapping(Actor* target); //will need a bool member function to check if that object triggers landmines

private:
	virtual bool withinLifespan();
};

class Goodie : public StaticActor
{
public: 
	Goodie(int imageID, double x, double y, StudentWorld* world);
	virtual void doThisThingWhileOverlapping(Actor* target);
	virtual void doThisThingWhileOverlappingPlayer(Penelope* player);
	void burnfall(); //all goodies will be destroyed by flames
	virtual ~Goodie();
private:
	virtual void giveGoodie(Penelope* player) = 0;

};

class VaccineGoodie :public Goodie
{
public:
	VaccineGoodie(double x, double y, StudentWorld* world);

	virtual ~VaccineGoodie();
private:
	virtual void giveGoodie(Penelope* player);
};

class LandmineGoodie :public Goodie
{
public:
	LandmineGoodie(double x, double y, StudentWorld* world);

	virtual ~LandmineGoodie();
private:
	virtual void giveGoodie(Penelope* player);
};

class GasGoodie :public Goodie
{
public:
	GasGoodie(double x, double y, StudentWorld* world);

	virtual ~GasGoodie();
private:
	virtual void giveGoodie(Penelope* player);
};

class Wall : public Actor
{
public:
	Wall(double x, double y, StudentWorld* world);
	virtual bool insideBoundingBox(double x, double y); //Wall will need to use default definition
	virtual void burnfall();
	virtual bool blocksFlames();
	virtual ~Wall();
private:
	virtual void classSpecificAction();
};

class Being : public Actor
{
public: Being(int imageID, double x, double y, StudentWorld* world);
		
	    virtual void burnfall(); // all beings are burned the same way, call the kill function
		virtual double howFarDoIMove() = 0;
		virtual bool canTriggerMines();
		void tryMoving( Direction dir);
		virtual ~Being();
private:
	virtual void kill() = 0;
	bool m_resting;
};

class Human : public Being
{
public:
	Human(int imageID, double x, double y, StudentWorld* world);
	   virtual ~Human();
	   int getInfect();
	   virtual void tryToInfect(); //Allow for humans to be infected. Will set infection count to one, which will begin counting
protected:
	void cure();
private:
	int m_infectCount;
	bool m_isInfected;
};


class Penelope : public Human
{
public:
	Penelope(int startX, int startY, StudentWorld *world);
	
	virtual double howFarDoIMove();
	virtual bool tryToEscape(); 
	virtual ~Penelope();
	int getMines();
	void addMines(int amt);
	int getFlames();
	void addFlames(int amt);
	int getVaccines();
	void addVaccines(int amt);
private:
	virtual void classSpecificAction(); 
	virtual void kill();
	int m_mines;
	int m_flames;
	int m_vaccines;

};

class Citizen : public Human
{
public:
Citizen(double x, double y, StudentWorld* world);
virtual double howFarDoIMove();
virtual bool tryToEscape();
virtual	~Citizen();

private:
	virtual void classSpecificAction();
	virtual void kill();
};

class Zombie : public Being
{
public:
	virtual double howFarDoIMove();
private:
};

class DumbZombie : public Zombie
{
public:
private:
	//virtual void kill();
};

class SmartZombie : public Zombie
{
public:
private:
	//virtual void kill();
};

#endif // ACTOR_H_
