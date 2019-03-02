#ifndef ROBOTACTOR_HPP_INCLUDED
#define ROBOTACTOR_HPP_INCLUDED

#include "Input.hpp"
#include "Actor.hpp"
#include "Animations.hpp"

class RobotActor : public EnemyActor
{
protected:
    AnimatedSprite bodySprite;
    sf::Sprite headSprite;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform = states.transform.combine(getTransform());
        target.draw(bodySprite, states);
        target.draw(headSprite, states);
    }
    
    virtual void rotateActorTo(const Vector2d& target)
    {
        double newRotation = toDegrees((target - getPosition()).getRotationAngle()) - 90;
        headSprite.setRotation(newRotation);
        bodySprite.setRotation(newRotation);
    }
public:
    
    RobotActor(Map& _map, const Vector2d& _pos = {0,0}, const Vector2d& _size = {32, 32})
        : EnemyActor(_map, _pos, _size, 16, 320, 35), bodySprite(AnimatedSpritePresets::RobotWalk, AnimationState::Stop)
    {
        headSprite.setTexture(TextureManager::get("assets/textures.png"));
        headSprite.setTextureRect({0, 80, 10, 11});
        setValidScaleAndOrigin(headSprite, {10, 11});
        
        setValidScaleAndOrigin(bodySprite, {16, 15});
        
        walkSpeed = 100;
        sleepBeforeGoingToDistraction = 1.5;
        sleepAfterReachingDistraction = 1;
    }
    
	virtual Vector2d getForewardVector() const
	{
		return Vectors::foreward.rotate(headSprite.getRotation() + 90);
	}
	
	virtual Vector2d getCastBeginPosition() const
	{
		return getPosition();
	}
	
	virtual void getDistracted()
	{
		headSprite.setTextureRect({0, 64, 10, 11});
	}
	virtual void getUndistracted()
	{
		headSprite.setTextureRect({0, 80, 10, 11});
	}
	
    virtual void update(double deltaTime)
    {
        Actor::update(deltaTime);
        if(isActorWalking())
        {
            if(bodySprite.state != AnimationState::Loop)
            {
                bodySprite.reset(AnimationState::Loop);
            }
            bodySprite.updateFrame(deltaTime);
        }
        else
        {
            bodySprite.reset(AnimationState::Stop);
        }
    }
    
    virtual ~RobotActor(){};
};

class RobotCircleActor : public EnemyActor
{
    
protected:
    sf::Sprite headSprite;
    sf::Sprite bodySprite;
    
    double rotationSpeed = 20;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform = states.transform.combine(getTransform());
        target.draw(bodySprite, states);
        target.draw(headSprite, states);
    }
    
public:
    
    RobotCircleActor(Map& _map, const Vector2d& _pos = {0,0}, const Vector2d& _size = {64, 64})
        : EnemyActor(_map, _pos, _size, 16, 320, 180)
    {
        bodySprite.setTexture(TextureManager::get("assets/textures.png"));
        bodySprite.setTextureRect({0, 96, 30, 30});
        headSprite.setTexture(TextureManager::get("assets/textures.png"));
        headSprite.setTextureRect({0, 128, 26, 26});
        setValidScaleAndOrigin(bodySprite, {30, 30});
        setValidScaleAndOrigin(headSprite, {26, 26});
        walkSpeed = 0;
    }
    
	virtual Vector2d getForewardVector() const
	{
		return Vectors::foreward;
	}
	
	virtual Vector2d getCastBeginPosition() const
	{
		return getPosition();
	}
	
    virtual void update(double deltaTime)
    {
        Actor::update(deltaTime);
        headSprite.rotate(rotationSpeed * deltaTime);
    }
    
    virtual ~RobotCircleActor  (){};
};

#endif // ROBOTACTOR_HPP_INCLUDED
