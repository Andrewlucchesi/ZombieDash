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

bool Actor::isOverlapping(int x, int y) //Checks to see if the specified coordinates are overlapping with this actor
{
	return false; //Not implemented yet
}

Actor::~Actor()
{ 
	//Nothing yet
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

StudentWorld* Actor::world()
{
	return m_world;
}

//////////////////////////////////////////////////////////////////
//WALL
//////////////////////////////////////////////////////////////////
Wall::Wall(int x, int y, StudentWorld* world)
	:Static(IID_WALL, x, y, world)
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
Static::Static(int imageID, int x, int y, StudentWorld * world, Direction dir, int depth)
	:Actor(imageID, x, y, world, dir, depth)
{
}

bool Static::insideBoundingBox(int x, int y) //Most Static objects don't have collision boxes
{
	return false;
}

void Static::tryMoving(const Direction dir, int dist = 0)
{
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

void Being::tryMoving(const Direction dir, int dist)
{
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

void Human::tryMoving(const Direction dir, int dist = 2)
{
	Being::tryMoving(dir, dist);
}

//////////////////////////////////////////////////////////////////
//Pene
//////////////////////////////////////////////////////////////////
Penelope::Penelope(int startX, int startY, StudentWorld *world)
	:Human(IID_PLAYER, startX, startY, world)
{

}

Penelope::~Penelope()
{
}

void Penelope::tryMoving(const Direction dir, int dist = 4)
{
	Being::tryMoving(dir, dist);
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


