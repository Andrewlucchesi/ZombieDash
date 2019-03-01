#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


////////////////////////////////////////////////////////////////
//ACTOR
////////////////////////////////////////////////////////////////
Actor::Actor(int ImageID, double x, double y, StudentWorld* world, Direction dir = right, int depth = 0)
	:GraphObject(ImageID, x, y, dir, depth), m_alive(true)
{
	m_world = world;
}

bool Actor::triggersCitizens()
{
	return false;
}

Actor::~Actor()
{ 
	//Nothing yet
}

bool Actor::isOverlapping(double x, double y) //Returns true if the given coordinate is within 10 pixels of the center of this actor
{
	return((world()->calculateDistance(x, y, getX(), getY())) <= 10);
}

bool Actor::insideBoundingBox(double x, double y) //Checks to see if the coordinate x,y is inside the actor's bounding box
{
	double boxX = Actor::getX();
	double boxY = Actor::getY();
	if (boxX <= x && (boxX + SPRITE_WIDTH - 1) >= x && boxY <= y && (boxY + SPRITE_HEIGHT - 1) >= y)
		return true;
	else return false;
}


bool Actor::blocksFlames()
{
	return false;
}

bool Actor::isAlive() //checks to see if the actor is alive
{
	return m_alive;
}

void Actor::tryToInfect()
{
	return;
}

bool Actor::canTriggerMines()
{
	return false;
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

void Actor::doSomething()
{
	if (!isAlive())
		return;
	classSpecificAction();
}

bool Actor::triggersZombieVomit()
{
	return false;
}

bool Actor::threatensCitizens()
{
	return false;
}

 bool Actor::tryMoving(Direction dir) // By default actors can't move (Only Beings can)
{
	return false;
}

//////////////////////////////////////////////////////////////////
//WALL
//////////////////////////////////////////////////////////////////
Wall::Wall(double x, double y, StudentWorld* world)
	:Actor(IID_WALL, x, y, world)
{ 
	
}

void Wall::classSpecificAction()
{
	//will never do anything
}

bool Wall::insideBoundingBox(double x, double y)
{
	return Actor::insideBoundingBox(x, y);
}

void Wall::burnfall()
{
	return;
}

bool Wall::blocksFlames()
{
	return true;
}

Wall::~Wall()
{
}

//////////////////////////////////////////////////////////////////
//Static
//////////////////////////////////////////////////////////////////
StaticActor::StaticActor(int imageID, double x, double y, StudentWorld * world, Direction dir, int depth)
	:Actor(imageID, x, y, world, dir, depth)
{
}

bool StaticActor::insideBoundingBox(double x, double y) //Static objects don't have collision boxes
{
	return false;
}

 void StaticActor::classSpecificAction() //Static Actors all have same structure for doSomething
{
	world()->overlaps(this); //Check and handle all overlapping situations
}



 void StaticActor::burnfall()
 {
	 return;
 }

 StaticActor::~StaticActor()
{

}


//EXITS
Exit::Exit(double x, double y, StudentWorld* world)
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

bool Exit::blocksFlames()
{
	return true;
}

//PITS
Pit::Pit(double x, double y, StudentWorld* world)
	:StaticActor(IID_PIT, x, y, world, right, 0)
{}

void Pit::doThisThingWhileOverlapping(Actor * target)
{
	target->burnfall();
}

void Pit::doThisThingWhileOverlappingPlayer(Penelope * player)
{
	doThisThingWhileOverlapping(player);
}

Pit::~Pit()
{
}


//////////////////////////////////////////////////////////////////
//Goodies
//////////////////////////////////////////////////////////////////
Goodie::Goodie(int imageID, double x, double y, StudentWorld* world)
	:StaticActor(imageID, x, y, world, right, 1)
{
}

void Goodie::doThisThingWhileOverlapping(Actor* target)
{
	return;
}

void Goodie::doThisThingWhileOverlappingPlayer(Penelope* player)
{
	world()->increaseScore(50);
	die();
	giveGoodie(player);
	world()->playSound(SOUND_GOT_GOODIE);
}

void Goodie::burnfall()
{
	die();
}

Goodie::~Goodie()
{
}




//VaccineGoodies
VaccineGoodie::VaccineGoodie(double x, double y, StudentWorld* world)
	:Goodie(IID_VACCINE_GOODIE, x, y, world)
{

}

VaccineGoodie::~VaccineGoodie()
{
}

void VaccineGoodie::giveGoodie(Penelope* player)
{
	player->addVaccines(1);
}



//Mine Gooides
LandmineGoodie::LandmineGoodie(double x, double y, StudentWorld* world)
	:Goodie(IID_LANDMINE_GOODIE, x, y, world)
{}

LandmineGoodie::~LandmineGoodie()
{}

void LandmineGoodie::giveGoodie(Penelope* player)
{
	player->addMines(2);
}

//Gas Goodies
GasGoodie::GasGoodie(double x, double y, StudentWorld* world)
	:Goodie(IID_GAS_CAN_GOODIE, x, y, world)
{}
GasGoodie::~GasGoodie()
{}

void GasGoodie::giveGoodie(Penelope* player)
{
	player->addFlames(5);
}

//////////////////////////////////////////////////////////////////
//Temp StaticActors
//////////////////////////////////////////////////////////////////
tempStaticActor::tempStaticActor(int imageID, double x, double y, StudentWorld* world, Direction dir, int depth)
: StaticActor(imageID, x, y, world, dir, depth) , m_Age(0)
{}

void tempStaticActor::classSpecificAction()
{
	m_Age++;
	if (withinLifespan())
		world()->overlaps(this);
	return;
}

tempStaticActor::~tempStaticActor()
{}

void tempStaticActor::doThisThingWhileOverlappingPlayer(Penelope * player)
{
	doThisThingWhileOverlapping(player);
}

bool tempStaticActor::withinLifespan() //By default, temp static actors like flames and vomit die after being alive for 2 ticks
{
	if (getAge() < 2) //If the actors Age is less than two ticks, return false proceeds with its Action
	{
		return true;
	}
	StaticActor::die(); //If actor is older than two ticks, it is marked as dead and removed fromt he game.
	return false;
}

int tempStaticActor::getAge()
{
	return m_Age;
}

//Flames
Flame::Flame(double x, double y, StudentWorld* world, Direction dir)
	:tempStaticActor(IID_FLAME, x, y, world, dir, 0)
{}

void Flame::doThisThingWhileOverlapping(Actor* target)
{
	target->burnfall();
}

bool Flame::canTriggerMines()
{
	return true;
}

Flame::~Flame()
{
}

//Vommit
Vomit::Vomit(double x, double y, StudentWorld* world, Direction dir)
	:tempStaticActor(IID_VOMIT, x, y, world, dir, 0)
{}

void Vomit::doThisThingWhileOverlapping(Actor * target)
{
	target->tryToInfect();
}

Vomit::~Vomit()
{
}



//Landmines
Landmine::Landmine(double x, double y, StudentWorld * world)
	:tempStaticActor(IID_LANDMINE, x, y, world, right, 1)
{}

void Landmine::doThisThingWhileOverlapping(Actor * target)
{
	if (target->canTriggerMines()) //if triggered, explodes
	{
		die();
		world()->playSound(SOUND_LANDMINE_EXPLODE);
		double x = getX(); double y = getY();
		world()->addActor(new Pit(x, y, world()));  //Creates a pit at mines location
		for (double i = (x - SPRITE_WIDTH); i <= x + SPRITE_WIDTH; i += SPRITE_WIDTH)	//Add flames in an 3x3 square around mine
		{
			for (double j = (y - SPRITE_HEIGHT); j <= y + SPRITE_HEIGHT; j += SPRITE_HEIGHT)
			{
				if ((world()->isFlammable(i, j)))
					world()->addActor(new Flame(i, j, world(), up));
			}
		}
	
	}
}

bool Landmine::withinLifespan() //Lifespan of landmine is inverted. Landmine is only active once the age is greater than 30, and will never die from age
{
	if (getAge() > 30)
		return true;
	else
		return false;
	
}

//////////////////////////////////////////////////////////////////
//BEING
//////////////////////////////////////////////////////////////////
Being::Being(int imageID, double x, double y, StudentWorld* world)
	:Actor(imageID, x, y, world)
{
}

void Being::burnfall()
{
	kill();
}

Being::~Being()
{
}

bool Being::isParalyzed()
{
	bool cur = m_paralyzed; 
	m_paralyzed = !m_paralyzed;
	return cur;
}

bool Being::tryMovingTorwardsPoint(double x, double y)
{
	double deltaX = x - getX();
	double deltaY = y - getY();
	if (x == getX()) //on same row as target
	{
		if (deltaY > 0  && tryMoving(up)) //target is above current point
		{
			setDirection(up);
			return true;
		}
		else if(deltaY < 0 && tryMoving(down))
		{
			setDirection(down);
			return true;
		}
	}
	else if (y == getY()) //on same collumn as target
	{
		if (deltaX > 0 && tryMoving(right))
		{
			setDirection(right);
			return true;
		}
		else if(deltaX < 0 && tryMoving(left))
		{
			setDirection(left);
			return true;
		}
	}
	else
	{
		int whichDir = randInt(0, 1); //Randomly try either to move vertically (0) or horizantally (1) torwards target

		if (whichDir == 0)
		{
			if (deltaY > 0 && tryMoving(up))
			{
				setDirection(up);
				return true;
			}
			else if (deltaY < 0 && tryMoving(down))
			{
				setDirection(down);
				return true;
			}
			else if (deltaX > 0 && tryMoving(right))
			{
				setDirection(right);
				return true;
			}
			else if (deltaX < 0 && tryMoving(left))
			{
				setDirection(left);
				return true;
			}
		}
		else if (whichDir == 1)
		{
			if (deltaX > 0 && tryMoving(right))
			{
				setDirection(right);
				return true;
			}
			else if (deltaX < 0 && tryMoving(left))
			{
				setDirection(left);
				return true;
			}
			else if (deltaY > 0 && tryMoving(up))
			{
				setDirection(up);
				return true;
			}
			else if (deltaY < 0 && tryMoving(down))
			{
				setDirection(down);
				return true;
			}
		}
		return false;
	}
}



bool Being::canTriggerMines()
{
	return true;
}

bool Being::tryMoving(const Direction dir)
{
	double dist = howFarDoIMove();
	if (!(Being::getDirection() == dir))
		Being::setDirection(dir);
	double destx = Being::getX();
	double desty = Being::getY();
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
	if (!(Being::world()->collision(destx, desty, this)))
	{
		Being::moveTo(destx, desty);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////
//Human
//////////////////////////////////////////////////////////////////
Human::Human(int imageID, double x, double y, StudentWorld* world)
	:Being(imageID, x, y, world), m_infectCount(0)
{

}

Human::~Human()
{
}

int Human::getInfect()
{
	return m_infectCount;
}

void Human::tryToInfect()
{
	m_isInfected = true;
}

bool Human::triggersZombieVomit()
{
	return true;
}

void Human::cure()
{
	m_isInfected = false;
	m_infectCount = 0;
}

bool Human::checkForCriticalInfection()
{
	if (m_isInfected == true)
	{
		m_infectCount++;
		if (m_infectCount >= 500)
		{
			kill();
			return true;
		}

	}
	return false;
}

void Human::classSpecificAction()
{
	if (checkForCriticalInfection() || isParalyzed())
		return;
	humanSpecificAction();

}



//////////////////////////////////////////////////////////////////
//Pene
//////////////////////////////////////////////////////////////////
Penelope::Penelope(int startX, int startY, StudentWorld *world)
	:Human(IID_PLAYER, startX, startY, world), m_mines(0), m_flames(0), m_vaccines(0)
{

}

bool Penelope::tryToEscape() //returns true if there are no citizens left
{
	if (world()->noCitizens())
	{
		world()->playSound(SOUND_LEVEL_FINISHED); 
		return true;
	}
	return false;
}

Penelope::~Penelope()
{
}

void Penelope::kill() //This function will be called when Penelope is killed
{
	if (isAlive())
	{
		die();
		world()->playSound(SOUND_PLAYER_DIE);
		return;
	}
}

bool Penelope::isParalyzed()
{
	return false;
}

double Penelope::howFarDoIMove()
{
	return(4); //Penelope moves 4 pixels
}

int Penelope::getMines()
{
	return m_mines;
}

void Penelope::addMines(int amt)
{
	m_mines += amt;
}

int Penelope::getFlames()
{
	return m_flames;
}

void Penelope::addFlames(int amt)
{
	m_flames += amt;
}

int Penelope::getVaccines()
{
	return m_vaccines;
}

void Penelope::addVaccines(int amt)
{
	m_vaccines += amt;
}

bool Penelope::triggersCitizens()
{
	return true;
}

void Penelope::humanSpecificAction()
{
	//check to see if alive
	double destX = Penelope::getX();
	double destY = Penelope::getY();
	int key;
	if (world()->getKey(key))
	{
		switch (key)  //Handle all possible inputs and make appropiate actions for each input
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

		case KEY_PRESS_TAB:  //Place landmine
			if (m_mines > 0)
			{
				world()->addActor(new Landmine(getX(), getY(), world()));
				m_mines--; 
			}
			break;
		case KEY_PRESS_ENTER: //Use Vaccine
		{
			if (m_vaccines > 0)
			{
				m_vaccines--;
				
			}
		}
		case KEY_PRESS_SPACE: //Fire flamethrower
			if (m_flames > 0)
			{
				m_flames--;
				world()->playSound(SOUND_PLAYER_FIRE);

				Direction dir = getDirection();
				bool blocked = false;
				for (double i = 1; i <= 3; i++) //Figure out which direction and how far apart to place flames
				{
					double destx = getX();
					double desty = getY();
					switch (dir)
					{

					case up: 
						desty += (SPRITE_HEIGHT*i);
						break;
					case down:
						desty -= (SPRITE_HEIGHT*i);
						break;
					case right:
						destx += (SPRITE_WIDTH*i);
						break;
					case left:
						destx -= (SPRITE_WIDTH*i);
						break;
					}

					if ((world()->isFlammable(destx, desty))) //If the next space is flammable, add a flame
					{
						world()->addActor(new Flame(destx, desty, world(), dir));
					}
					else //If not, do nothing and do not introduce any more flames
					{
						break;
					}
				}
			}
		}
	}
	
	return; 
	
}

//////////////////////////////////////////////////////////////////
//Citizen
//////////////////////////////////////////////////////////////////

Citizen::Citizen(double x, double y, StudentWorld * world)
	:Human(IID_CITIZEN, x, y, world)
{
}

void Citizen::humanSpecificAction()
{  //First find distance of nearest threat (if any) and the player
	double x_p, y_p, dist_p, x_z, y_z, dist_z;
	world()->locateNearestCitizenThreat(getX(), getY(), x_z, y_z, dist_z);
	world()->locatePlayer(getX(), getY(), x_p, y_p, dist_p);

	if ((dist_z == -1 || (dist_p < dist_z)) && dist_p <= 80) //P is close, so citizen will try to move torwards her
	{
		if (tryMovingTorwardsPoint(x_p, y_p))
		{
			return;
		}
	}
	if (dist_z <= 80 && dist_z >0) //There is a zombie nearby that the citizen wants to move away from
	{
		double bestX = 0, bestY = 0, bestDist = 0, newX_z, newY_z, newDist_z;
		Direction bestDir;
		if (!(world()->collision(getX() + 2, getY(), this))) //Check to see if can move in the right direction
		{
			world()->locateNearestCitizenThreat(getX() + 2, getY(), newX_z, newY_z, newDist_z);
			if (newDist_z > bestDist && newDist_z > dist_z)
			{
				bestDist = newDist_z, bestX = getX() + 2, bestY = getY();
				bestDir = right;
			}	//Check to see if this spot will give a better distance from zombies
		}
		if (!(world()->collision(getX() - 2, getY(), this))) //Check for left
		{
			world()->locateNearestCitizenThreat(getX() - 2, getY(), newX_z, newY_z, newDist_z);
			if (newDist_z > bestDist && newDist_z > dist_z)
			{
				bestDist = newDist_z, bestX = getX() - 2, bestY = getY();
				bestDir = left;
			}
		}
		if (!(world()->collision(getX(), getY() + 2, this))) //check for up
		{
			world()->locateNearestCitizenThreat(getX(), getY() + 2, newX_z, newY_z, newDist_z);
			if (newDist_z > bestDist && newDist_z > dist_z)
			{
				bestDist = newDist_z, bestX = getX(), bestY = getY() + 2;
				bestDir = up;
			}
		}
		if (!(world()->collision(getX(), getY() - 2, this))) //check for down
		{
			world()->locateNearestCitizenThreat(getX(), getY() - 2, newX_z, newY_z, newDist_z);
			if (newDist_z > bestDist && newDist_z > dist_z)
			{
				bestDist = newDist_z, bestX = getX(), bestY = getY() - 2;
				bestDir = down;
			}
		}

		//After making all checks, if there is a new safer distance, the citizen will move in that direction
		if (bestDist != 0)
		{
			setDirection(bestDir);
			moveTo(bestX, bestY);
		}
	}


}

bool Citizen::checkForCriticalInfection()
{
	if (Human::checkForCriticalInfection())
	{
		world()->playSound(SOUND_ZOMBIE_BORN);
		int chance = randInt(1, 100);
		if (chance <= 70)
		{
			world()->addActor(new DumbZombie(getX(), getY(), world()));
			//introduce a dumb zombie
		}
		else
		{
			world()->addActor(new SmartZombie(getX(), getY(), world()));
			//introduce a smart zombie
		}
		return true;
	}
	return false;
}

void Citizen::kill()
{
	if (isAlive()) {
		if (!(getInfect() == 500))
			world()->playSound(SOUND_CITIZEN_DIE);
		world()->increaseScore(-1000);
		die();
		world()->citizenGone();
	}
}

bool Citizen::tryToEscape()
{
	if (isAlive())
	{
		world()->increaseScore(500);
		world()->playSound(SOUND_CITIZEN_SAVED);
		die();
		world()->citizenGone();
	}
	return true;
}

void Citizen::tryToInfect()
{
	world()->playSound(SOUND_CITIZEN_INFECTED);
	Human::tryToInfect();
}

double Citizen::howFarDoIMove()
{
	return(2); //Citizens move 2 pixels at a time
}

Citizen::~Citizen()
{

}



//////////////////////////////////////////////////////////////////
//Zombies
//////////////////////////////////////////////////////////////////

Zombie::Zombie(double x, double y, StudentWorld* world)
	:Being(IID_ZOMBIE, x, y, world), m_movementPlanDistance(0)
{

}
double Zombie::howFarDoIMove()
{
	return 1;
}

bool Zombie::threatensCitizens()
{
	return true;
}

bool Zombie::triggersCitizens()
{
	return true;
}

Zombie::~Zombie()
{
}

void Zombie::setMovementPlanDistance(int amt)
{
	m_movementPlanDistance = amt;
}

void Zombie::decrementMovementPlan()
{
	m_movementPlanDistance--;
}

int Zombie::getMovementPlanDistance()
{
	return m_movementPlanDistance;
}

void Zombie::classSpecificAction()
{
	if (isParalyzed())
		return;
	
	//Try to vomit in front of itself
	int chance = randInt(1, 3);  
	if (chance == 1) //1/3 chance to be able to vomit
	{
		Direction Dir = getDirection();
	


		switch (Dir)
		{
		case up:
			if (world()->isZombieVomitTriggerAt(getX(), getY() + SPRITE_HEIGHT))
			{
				world()->addActor(new Vomit(getX(), getY() + SPRITE_HEIGHT, world(), Dir));
				world()->playSound(SOUND_ZOMBIE_VOMIT);
				return;
			}
			break;
		case down:
			if (world()->isZombieVomitTriggerAt(getX(), getY() - SPRITE_HEIGHT))
			{
				world()->addActor(new Vomit(getX(), getY() - SPRITE_HEIGHT, world(), Dir));
				world()->playSound(SOUND_ZOMBIE_VOMIT);
				return;
			}
			break;
		case right:
			if (world()->isZombieVomitTriggerAt(getX() + SPRITE_WIDTH, getY()))
			{
				world()->addActor(new Vomit(getX() + SPRITE_WIDTH, getY(), world(), Dir));
					world()->playSound(SOUND_ZOMBIE_VOMIT);
					return;
			}
			break;

		case left:
			if (world()->isZombieVomitTriggerAt(getX() - SPRITE_WIDTH, getY()))
			{
				world()->addActor(new Vomit(getX() - SPRITE_WIDTH, getY(), world(), Dir));
					world()->playSound(SOUND_ZOMBIE_VOMIT);
					return;
			}
			break;


		}
	}

	//Make vomit check

	zombieSpecificAction();
}


//Dumb Zombies
DumbZombie::DumbZombie(double x, double y, StudentWorld * world)
	:Zombie(x, y, world)
{
}

DumbZombie::~DumbZombie()
{
}

void DumbZombie::kill()
{
	if (isAlive())
	{
		die();
		world()->increaseScore(1000);
		world()->playSound(SOUND_ZOMBIE_DIE);
		
		if (randInt(1, 10) == 7) //if penelope is lucky lucky rubber ducky, dumb zombie drops a vaccine
		{
			Direction dir = randInt(0, 3) * 90;
			switch (dir)
			{
			case up:
				if (world()->anyOverlaps(getX(), getY() + SPRITE_HEIGHT))
					world()->addActor(new VaccineGoodie(getX(), getY() + SPRITE_HEIGHT, world()));
				break;
			case down:
				if (world()->anyOverlaps(getX(), getY() - SPRITE_HEIGHT))
					world()->addActor(new VaccineGoodie(getX(), getY() - SPRITE_HEIGHT, world()));
				break;
			case left:
				if (world()->anyOverlaps(getX() - SPRITE_WIDTH, getY()))
					world()->addActor(new VaccineGoodie(getX() - SPRITE_WIDTH, getY(), world()));
				break;
			case right:
				if (world()->anyOverlaps(getX() + SPRITE_WIDTH, getY()))
					world()->addActor(new VaccineGoodie(getX() - SPRITE_WIDTH, getY(), world()));
				break;
			}
		}
	}
}

void DumbZombie::zombieSpecificAction()
{
	//Dumb zombie will check to see if it needs to make a new movement plan
	if (getMovementPlanDistance() <= 0)
	{
		setMovementPlanDistance(randInt(3, 10));
		setDirection(randInt(0, 3) * 90);
	}
	if (tryMoving(getDirection()))
	{
		decrementMovementPlan();
	}
	else
	{
		setMovementPlanDistance(0);
	}

}




//Smart Zombies
SmartZombie::SmartZombie(double x, double y, StudentWorld * world)
	:Zombie(x, y, world)
{
}

SmartZombie::~SmartZombie()
{
}

void SmartZombie::kill()
{
	if (isAlive())
	{
		kill();
		world()->increaseScore(2000);
		world()->playSound(SOUND_ZOMBIE_DIE);
	}
}

void SmartZombie::zombieSpecificAction()
{
	double bestx, besty, bestdist;

	//if a new movement plan is needed, make one
	if (getMovementPlanDistance() <= 0)
	{
		setMovementPlanDistance(randInt(3, 10));
		world()->locateNearestVomitTrigger(getX(), getY(), bestx, besty, bestdist); //CHecks to see if a human is nearby
		if (bestdist > 80)
		{
			setDirection(randInt(0, 3) * 90);
		}
		else if (bestdist <= 80)
		{
			double deltaX = bestx - getX(), deltaY = besty - getY();
			if (deltaX == 0)
			{
				if (deltaY > 0)
					setDirection(up);
				else if (deltaY < 0)
					setDirection(down);
			}
			else if (deltaY == 0)
			{
				if (deltaX > 0)
					setDirection(right);
				else if (deltaY < 0)
					setDirection(left);
			}
			else
			{
				int chance = randInt(0, 1); //0 is vertical, 1 is horizantal
				if (chance == 0)
				{
					if (deltaY > 0)
						setDirection(up);
					else if (deltaY < 0)
						setDirection(down);
				}
				else if (chance == 1)
				{
					if (deltaX > 0)
						setDirection(right);
					else if (deltaX < 0)
						setDirection(left);
				}
			}


		}
	}


	if (!tryMoving(getDirection()))
	{
		setMovementPlanDistance(0);
	}
	else decrementMovementPlan();

}
