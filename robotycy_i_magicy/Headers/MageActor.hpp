#ifndef MAGEACTOR_HPP_INCLUDED
#define MAGEACTOR_HPP_INCLUDED

#include "Input.hpp"
#include "Actor.hpp"
#include "Animations.hpp"

class MageActor : public Actor
{
protected:
    sf::Sprite hatSprite;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform = states.transform.combine(getTransform());
        target.draw(hatSprite, states);
    }
    
public:
    
    MageActor(Map& _map, const Vector2d& _pos = {0,0}, const Vector2d& _size = {32, 32})
        : Actor(_map, _pos, _size)
    {
        hatSprite.setTexture(textureManager.get("assets/textures.png"));
        hatSprite.setTextureRect({0, 48, 15, 15});
        setValidScaleAndOrigin(hatSprite, {15, 15});
    }
    
    virtual ~MageActor(){};
};

class PlayerMageActor : public MageActor
{
public:
    PlayerMageActor(Map& _map, const Vector2d& _pos = {0,0}, const Vector2d& _size = {32, 32})
        : MageActor(_map, _pos, _size)
    {
        hatSprite.setTexture(textureManager.get("assets/textures.png"));
        hatSprite.setTextureRect({0, 48, 15, 15});
        setValidScaleAndOrigin(hatSprite, {15, 15});
        
        walkSpeed = 200;
    }
    
    void update(double deltaTime)
    {
        Vector2d destination = Input::getDirectionAxis() * 10000;
        goToDestination(getPosition() + destination);
        
        walk(deltaTime);
        hatSprite.setRotation(toDegrees((Input::getMouseView() - getPosition()).getRotationAngle()) - 90);
        
    }
    
    virtual ~PlayerMageActor(){};
};

#endif // MAGEACTOR_HPP_INCLUDED
