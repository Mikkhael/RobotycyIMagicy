#ifndef ROBOTACTOR_HPP_INCLUDED
#define ROBOTACTOR_HPP_INCLUDED

#include "Input.hpp"
#include "Actor.hpp"
#include "Animations.hpp"

class RobotActor : public Actor
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
        : Actor(_map, _pos, _size), bodySprite(AnimatedSpritePresets::RobotWalk, AnimationState::Stop)
    {
        headSprite.setTexture(textureManager.get("assets/textures.png"));
        headSprite.setTextureRect({0, 80, 10, 11});
        setValidScaleAndOrigin(headSprite, {10, 11});
        
        setValidScaleAndOrigin(bodySprite, {16, 15});
        
        walkSpeed = 100;
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

#endif // ROBOTACTOR_HPP_INCLUDED
