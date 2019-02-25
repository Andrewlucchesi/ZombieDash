#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


////////////////////////////////////////////////////////////////
//ACTOR
////////////////////////////////////////////////////////////////
Actor::Actor(int ImageID, int x, int y, StudentWorld* world, Direction dir = right, int depth = 0)
	:GraphObject(ImageID, x, y, dir, depth), m_alive(true)
{
	m_world = world;
}

Actor::~Actor()
{ 
	//Nothing yet
}

{
	double deltaX = (getX() - x);
	double deltaY = (getY() - y);
	return(((deltaX*deltaX) + (deltaY * deltaY)) <= 100);
}

bool Actor::insideBoundingBox(int x, int y) //Checks to see if the coordinate x,y is inside the actor's bounding box
{
	int boxX = Actor::getX();
	int boxY = Actor::getY();
	if (boxX <= x && (boxX + SPRITE_WIDTH - 1) >= x && boxY <= y && (boxY + SPRITE_HEIGHT - 1) >= y)
		return true;
	else return false;
}

bool Actor::isAlive() //checks to see if the actor is alive
{
	return m_alive;
}

void Actor::die()
{
	m_alive = false;
}

bool Actor::tryToEscape()
{
	return false;
}

StudentWorld* Actor::world()
{
	return m_world;
}

 void Actor::tryMoving(Direction dir) // By default actors can't move (Only Beings can)
{
	return;
}

//////////////////////////////////////////////////////////////////
//WALL
//////////////////////////////////////////////////////////////////
Wall::Wall(int x, int y, StudentWorld* world)
	:Actor(IID_WALL, x, y, world)
{ 
	
}

void Wall::doSomething()
{
	//will never do anything
}

bool Wall::insideBoundingBox(int x, int y)
{
	return Actor::insideBoundingBox(x, y);
}

Wall::~Wall()
{
}

//////////////////////////////////////////////////////////////////
//Static
//////////////////////////////////////////////////////////////////
StaticActor::StaticActor(int imageID, int x, int y, StudentWorld * world, Direction dir, int depth)
	:Actor(imageID, x, y, world, dir, depth)
{
}

bool StaticActor::insideBoundingBox(int x, int y) //Most Static objects don't have collision boxes
{
	return false;
}

void StaticActor::doSomething() //Static Actors all have same structure for doSomething
{
	checkForOverlapping(); //Check and handle all overlapping situations
	//checkLifespans
}

StaticActor::~StaticActor()
{

}

void StaticActor::checkForOverlapping()
{
	world()->overlaps(this);
}

/*void StaticActor::burnfall()
{
	return;
	//By default, staticactors do not burn/fall. However, all goodies burn and so do LandMines
}
*/

//////////////////////////////////////////////////////////////////
//EXIT
//////////////////////////////////////////////////////////////////
Exit::Exit(int x, int y, StudentWorld* world)
	:StaticActor(IID_EXIT, x, y, world, right, 1)
{

}

Exit::~Exit()
{

}


void Exit::doThisThingWhileOverlapping(Actor* target)
{
	if (target->tryToEscape())
	{
		world()->increaseScore(500);
		target->die();
		world()->playSound(SOUND_CITIZEN_SAVED);
		return;
	}
}

void Exit::doThisThingWhileOverlappingPlayer(Penelope* player)
{
	if (player->tryToEscape())
	{
		world()->levelFinished();
	}
	//inform student world that Penelope has finished the level
	return; 
}


//////////////////////////////////////////////////////////////////
//BEING
//////////////////////////////////////////////////////////////////
Being::Being(int imageID, int x, int y, StudentWorld* world)
	:Actor(imageID, x, y, world)
{
}

Being::~Being()
{
}

void Being::tryMoving(const Direction dir)
{
	int dist = howFarDoIMove();
	if (!(Being::getDirection() == dir))
		Being::setDirection(dir);
	int destx = Being::getX();
	int desty = Being::getY();
	switch (dir)
	{
	case left:
		destx -= dist;
		break;
	case right:
		destx += dist;
		break;
	case up:
		desty += dist;
		break;
	case down:
		desty -= dist;
		break;
	}
	if (!(Being::world()->collision(destx, desty)))
		Being::moveTo(destx, desty);
}

//////////////////////////////////////////////////////////////////
//Human
//////////////////////////////////////////////////////////////////
Human::Human(int imageID, int x, int y, StudentWorld* world)
	:Being(imageID, x, y, world)
{

}

Human::~Human()
{
}

//////////////////////////////////////////////////////////////////
//Pene
//////////////////////////////////////////////////////////////////
Penelope::Penelope(int startX, int startY, StudentWorld *world)
	:Human(IID_PLAYER, startX, startY, world)
{

}

bool Penelope::tryToEscape() //returns true if there are no citizens left
{
	return(world()->noCitizens());
}

Penelope::~Penelope()
{
}

void Penelope::kill() //This function will be called when Penelope is killed
{
	return; //need to edit
}

int Penelope::howFarDoIMove()
{
	return(4); //Penelope moves 4 pixels
}

void Penelope::doSomething()
{
	//check to see if alive
	int destX = Penelope::getX();
	int destY = Penelope::getY();
	int key;
	if (world()->getKey(key)) //add accessor for world
	{
		switch (key)
		{
		case KEY_PRESS_LEFT:
			tryMoving(left);
			break;
		case KEY_PRESS_RIGHT:
			tryMoving(right);
			break;
		case KEY_PRESS_UP:
			tryMoving(up);
			break;
		case KEY_PRESS_DOWN:
			tryMoving(down);
			break;
		}
	}
	//Use studentworld getKey
	return; //Page 27
	//Make penelope move when keys are pressed
}

//////////////////////////////////////////////////////////////////
//Citizen
//////////////////////////////////////////////////////////////////

Citizen::Citizen(int x, int y, StudentWorld * world)
	:Human(IID_CITIZEN, x, y, world)
{
}

void Citizen::doSomething()
{
	return; //for now, do nothing
}

void Citizen::kill()
{
	//kill the citizen
}

int Citizen::howFarDoIMove()
{
	return(2); //Citizens move 2 pixels at a time
}

Citizen::~Citizen()
{

}
