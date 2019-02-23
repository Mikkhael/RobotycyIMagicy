#ifndef MAGEACTOR_HPP_INCLUDED
#define MAGEACTOR_HPP_INCLUDED

#include "Input.hpp"
#include "Actor.hpp"
#include "Animations.hpp"
#include "LineCaster.hpp"

class MageActor : public Actor
{
protected:
    AnimatedSprite bodySprite;
    sf::Sprite hatSprite;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform = states.transform.combine(getTransform());
        if(isActorWalking())
        {
            target.draw(bodySprite, states);
        }
        target.draw(hatSprite, states);
    }
    
    virtual void rotateActorTo(const Vector2d& target)
    {
        double newRotation = toDegrees((target - getPosition()).getRotationAngle()) - 90;
        hatSprite.setRotation(newRotation);
        bodySprite.setRotation(newRotation);
    }
public:
    
    MageActor(Map& _map, const Vector2d& _pos = {0,0}, const Vector2d& _size = {32, 32})
        : Actor(_map, _pos, _size), bodySprite(AnimatedSpritePresets::MageWalk, AnimationState::Stop)
    {
        hatSprite.setTexture(textureManager.get("assets/textures.png"));
        hatSprite.setTextureRect({0, 48, 15, 15});
        setValidScaleAndOrigin(hatSprite, {15, 15});
        
        setValidScaleAndOrigin(bodySprite, {7, 21});
    }
    
    virtual void update(double deltaTime)
    {
        Actor::update(deltaTime);
        if(isActorWalking())
        {
            if(bodySprite.state != AnimationState::Loop)
            {
                bodySprite.setFrame(0, AnimationState::Loop);
            }
            bodySprite.updateFrame(deltaTime);
        }
        else
        {
            bodySprite.setFrame(1, AnimationState::Stop);
        }
    }
    
    virtual ~MageActor(){};
};

class PlayerMageActor : public MageActor
{
protected:
    Vector2d lineCastEnd;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        MageActor::draw(target, states);
        //states.transform = states.transform.combine(getTransform());
        
        sf::VertexArray va(sf::Lines, 2);
        va[0].position = getPosition();
        va[1].position = lineCastEnd;
        
        va[0].color = sf::Color::Red;
        va[1].color = sf::Color::Red;
        
        target.draw(va, states);
    }
    
public:
    PlayerMageActor(Map& _map, const Vector2d& _pos = {0,0}, const Vector2d& _size = {32, 32})
        : MageActor(_map, _pos, _size)
    {        
        walkSpeed = 200;
        rotateToDestinationWhenWalking = false;
    }
    
    void update(double deltaTime)
    {
        MageActor::update(deltaTime);
        Vector2d destination = Input::getDirectionAxis() * 100000;
        goToDestination(getPosition() + destination);
        rotateActorTo(Input::getMouseView());
        
        moveOutOfWalls();
        
        lineCastEnd = LineCast::castOverMap(getPosition(), Input::getMouseView(), *map).contactPoint;
    }
    
    virtual ~PlayerMageActor(){};
};

#endif // MAGEACTOR_HPP_INCLUDED
