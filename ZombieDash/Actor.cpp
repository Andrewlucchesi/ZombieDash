#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


////////////////////////////////////////////////////////////////
//ACTOR
////////////////////////////////////////////////////////////////
Actor::Actor(int ImageID, int x, int y, Direction dir = right, int depth = 0)
	:GraphObject(ImageID, x, y, dir, depth)
{
	
}

Actor::~Actor()
{
	//Nothing yet
}

bool Actor::insideBoundingBox(int x, int y)
{
	int boxX = Actor::getX();
	int boxY = Actor::getY();
	if (boxX <= x && (boxX + SPRITE_WIDTH - 1) >= x && boxY <= y && (boxY + SPRITE_HEIGHT - 1) >= y)
		return true;
	else return false;
}
//////////////////////////////////////////////////////////////////
//WALL
//////////////////////////////////////////////////////////////////
Wall::Wall(int x, int y)
	:Actor(IID_WALL, x, y, right, 0)
{
	//Pass stuff on to actor
}

void Wall::doSomething()
{
	//will probably never do anything
}


//////////////////////////////////////////////////////////////////
//BEING
//////////////////////////////////////////////////////////////////
Being::Being(int imageID, int x, int y, Direction dir = right, int depth = 0)
	:Actor(imageID, x, y, dir, depth)
{
	m_alive = true;
}

bool Being::isAlive()
{
	return m_alive;
}

//////////////////////////////////////////////////////////////////
//Pene
//////////////////////////////////////////////////////////////////
Penelope::Penelope(int startX, int startY, StudentWorld *world)
	:Being(IID_PLAYER, startX, startY), m_world(world)
{

}

Penelope::~Penelope()
{
	delete m_world;
}

void Penelope::doSomething()
{
	//check to see if alive
	int destX = Penelope::getX();
	int destY = Penelope::getY();
	int key;
	if (m_world->getKey(key))
	{
		switch (key)
		{
		case KEY_PRESS_LEFT:
			if (!(Penelope::getDirection() == left))
				Penelope::setDirection(left);
			destX -= 4;
			if (!m_world->collision(destX, destY))
				Penelope::moveTo(destX, destY);
			break;
		case KEY_PRESS_RIGHT:
			if (!(Penelope::getDirection() == right))
				Penelope::setDirection(right);
			destX += 4;
			if (!m_world->collision(destX, destY))
				Penelope::moveTo(destX, destY);
			break;
		case KEY_PRESS_UP:
			if (!(Penelope::getDirection() == up))
				Penelope::setDirection(up);
			destY += 4;
			if (!m_world->collision(destX, destY))
				Penelope::moveTo(destX, destY);
			break;
		case KEY_PRESS_DOWN:
			if (!(Penelope::getDirection() == down))
				Penelope::setDirection(down);
			destY -= 4; 
			if (!m_world->collision(destX, destY))
				Penelope::moveTo(destX, destY);
			break;
		}
	}
	//Use studentworld getKey
	return; //Page 27
	//Make penelope move when keys are pressed
}

