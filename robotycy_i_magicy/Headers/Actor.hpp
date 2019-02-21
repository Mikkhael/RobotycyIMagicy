#ifndef ACTOR_HPP_INCLUDED
#define ACTOR_HPP_INCLUDED

#include "Map.hpp"

class Actor : public sf::Drawable, public SimpleTransformable
{
    Map* map;

protected:
    Vector2d size;
    
    Vector2d positionToCoords() const
    {
        return (getPosition() - map->getPosition()) / 32;
    }
    Vector2d coordsToPosition(const Vector2d& coords) const
    {
        return coords * 32 + map->getPosition();
    }
    
   void setValidScaleAndOrigin(sf::Transformable& target, const Vector2d& targetSize) const
    {
        target.setOrigin(targetSize / 2);
        target.setScale(size.x / targetSize.x, size.y / targetSize.y);
    }
    
    Vector2d destination;
    bool isWalking = false;
    
    void walk(double deltaTime)
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
    
    
public:
    Actor(Map& _map, const Vector2d& _pos = {0,0}, const Vector2d& _size = {32, 32})
        : map(&_map), size(_size)
    {
        setPosition(_pos);
    }
    
    double walkSpeed = 200;
    
    void goToDestination(const Vector2d& dest, double ws = 0)
    {
        destination = dest;
        if(ws != 0)
            walkSpeed = ws;
        isWalking = true;
    }
    
    void stopWalking()
    {
        isWalking = false;
    }
    
    bool isActorWalking() const
    {
        return isWalking;
    }
    
    virtual void update(double deltaTime)
    {
        walk(deltaTime);
    }
    
    virtual ~Actor(){}
};

#endif // ACTOR_HPP_INCLUDED
