#ifndef ACTOR_HPP_INCLUDED
#define ACTOR_HPP_INCLUDED

#include "Map.hpp"
#include "Collisions.hpp"
#include "LineCaster.hpp"

class Actor : public sf::Drawable, public SimpleTransformable
{

protected:
    Map* map;
    Vector2d size;
    
    void setValidScaleAndOrigin(sf::Transformable& target, const Vector2d& targetSize) const
    {
        target.setOrigin(targetSize / 2);
        target.setScale(size.x / targetSize.x, size.y / targetSize.y);
    }
    
    std::vector<std::pair<Vector2d, double> > patrolPath;
    int pathIndex = 0;
    double sleepCounter = 0;
    bool onPatrolPath = false;
    bool isDistracted = false;
    bool reachedDistraction = false;
    Vector2d tempLastDestination;
    Vector2d tempLastPosition;
    Vector2d destination;
    bool isWalking = false;
    
    bool isAboutToGoToNextNodeOnPath = false;
    bool isAboutToReachDistraction = false;
    void walk(double deltaTime)
    {
    	if(walkSpeed == 0)
		{
			return;
		}
    	if(onPatrolPath)
		{
			if(isDistracted)
			{
				if(sleepCounter > 0)
				{
					sleepCounter -= deltaTime;
					return;
				}
				
				if(isAboutToReachDistraction)
				{
					reachedDistraction = true;
					isAboutToReachDistraction = false;
					goToDestination(tempLastPosition);
				}
				
				Vector2d path = destination - getPosition();
				if(path.magnatudeSquared() <= walkSpeed*walkSpeed * deltaTime*deltaTime)
				{
					setPosition(destination);
					if(reachedDistraction)
					{
						isDistracted = false;
						getUndistracted();
						goToDestination(tempLastDestination);
					}
					else
					{
						isAboutToReachDistraction = true;
						sleepCounter = sleepAfterReachingDistraction;
					}
				}
				else
				{
					move(path.resize(walkSpeed * deltaTime));
				}
			}
			else
			{
				if(sleepCounter > 0)
				{
					sleepCounter -= deltaTime;
					return;
				}
				if(isAboutToGoToNextNodeOnPath)
				{
					pathIndex = (pathIndex+1) % patrolPath.size();
					goToDestination(patrolPath[pathIndex].first);
					isAboutToGoToNextNodeOnPath = false;
				}
				
				Vector2d path = destination - getPosition();
				if(path.magnatudeSquared() <= walkSpeed*walkSpeed * deltaTime*deltaTime)
				{
					setPosition(destination);
					sleepCounter = patrolPath[pathIndex].second;
					isAboutToGoToNextNodeOnPath = true;
				}
				else
				{
					move(path.resize(walkSpeed * deltaTime));
				}
			}
		}
		else
		{
			if(isWalking)
			{
				Vector2d path = destination - getPosition();
				if(path.magnatudeSquared() <= walkSpeed*walkSpeed * deltaTime*deltaTime)
				{
					setPosition(destination);
					isWalking = false;
				}
				else
				{
					move(path.resize(walkSpeed * deltaTime));
				}
			}
		}
    }
    
    bool moveOutOfWalls()
    {
        sf::FloatRect actorRect(getPosition() - size*0.5, size);
        Vector2u actorCoords = map->positionToCoords(getPosition());
        
        if(map->areCoordsGood(actorCoords))
        {
            sf::FloatRect tileRect = map->getTileRect(actorCoords);
            
            if(!map->isTileWalkable(actorCoords))
            {
                Vector2d toMove = Collision::getRectsOverlapMTV(actorRect, tileRect);
                
                Vector2u newCoords = actorCoords + toMove.normalize();
                if(!map->isTileWalkable(newCoords))
                {
                    return false;
                }
                move(toMove);
                actorRect = {getPosition() - size*0.5, size};
                actorCoords = newCoords;
                
            }
            
        }
        
        for(int i=-1; i<=1; i++)
        {
            for(int j=-1; j<=1; j++)
            {
                if(i == 0 && j == 0)
                {
                    continue;
                }
                Vector2u newCoords = actorCoords + Vector2u(j, i);
                if(map->areCoordsGood(newCoords) && !map->isTileWalkable(newCoords) && Collision::checkIfRectsOverlap(actorRect, map->getTileRect(newCoords)))
                {
                    Vector2f toMove = Collision::getRectsOverlapMTV(actorRect, map->getTileRect(newCoords));
                    actorRect = {{actorRect.left + toMove.x, actorRect.top + toMove.y}, size};
                }
            }
        }
        
        Vector2d newPosition(actorRect.left + size.x*0.5, actorRect.top + size.y*0.5);
        if(map->isTileWalkable(map->positionToCoords(newPosition)))
        {
            setPosition(newPosition);
            return true;
        }
        
        return false;
    }
    bool rotateToDestinationWhenWalking = true;
    
    virtual void rotateActorTo(const Vector2d& target)
    {
    }
    
public:
    Actor(Map& _map, const Vector2d& _pos = {0,0}, const Vector2d& _size = {0, 0})
        : map(&_map), size(_size)
    {
        setPosition(_pos);
        if(size.magnatude() == 0)
        {
            size = {map->tileSize, map->tileSize};
        }
    }
    
    double walkSpeed = 200;
    
    void setPatrolPath(const std::vector<std::pair<Vector2d, double> >& _path)
    {
    	pathIndex = 0;
        sleepCounter = 0;
    	patrolPath = _path;
    	isDistracted = false;
    	onPatrolPath = true;
    	goToDestination(patrolPath[0].first);
    }
    
    int sleepBeforeGoingToDistraction = 0;
    int sleepAfterReachingDistraction = 0;
    void goToDistraction(const Vector2d& target)
    {
    	if(walkSpeed <= 0)
		{
			return;
		}
		
    	getDistracted();
    	isDistracted = true;
    	reachedDistraction = false;
    	tempLastDestination = destination;
    	tempLastPosition = getPosition();
    	goToDestination(target);
    	sleepCounter = sleepBeforeGoingToDistraction;
    }
    virtual void getDistracted(){}
    virtual void getUndistracted(){}
    
    
    void goToDestination(const Vector2d& dest, double ws = 0)
    {
        destination = dest;
        if(ws != 0)
            walkSpeed = ws;
        isWalking = true;
        if(rotateToDestinationWhenWalking)
        {
            rotateActorTo(destination);
        }
    }
    
    bool isActorWalking() const
    {
        return isWalking && (sleepCounter <= 0);
    }
    bool isActorDistracted()
    {
    	return isDistracted;
    }
    
    virtual void update(double deltaTime)
    {
        walk(deltaTime);
    }
    
    Scene& getScene()
    {
    	return map->getScene();
    }
    
    Vector2i getCoords()
    {
    	return map->positionToCoords(getPosition());
    }
    
	sf::FloatRect getRect()
	{
		return sf::FloatRect(getPosition() - size/2, size);
	}
	
    virtual ~Actor(){}
};

class EnemyActor : public Actor, public Observer
{
public:
	EnemyActor(Map& _map, const Vector2d& _pos = {0,0}, const Vector2d& _size = {0, 0}, double _closeupRadius = 0, double _viewDistance = 0, double _viewAngle = 0)
		: Actor(_map, _pos, _size), Observer(_closeupRadius, _viewDistance, _viewAngle)
	{
	}
};

#endif // ACTOR_HPP_INCLUDED
