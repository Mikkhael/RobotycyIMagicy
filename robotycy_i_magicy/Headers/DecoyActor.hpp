#ifndef DECOYACTOR_HPP_INCLUDED
#define DECOYACTOR_HPP_INCLUDED

#include "Actor.hpp"
#include "Animations.hpp"

class DecoyActor : public Actor
{
	AnimatedSprite sprite;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform = states.transform.combine(getTransform());
		target.draw(sprite, states);
    }
	
public:
	
	bool isActive = true;
	
	void setInactive()
	{
		isActive = false;
		sprite.setColor(sf::Color(255,255,255,50));
	}
	
	DecoyActor(Map& _map, const Vector2d& _pos = {0,0}, const Vector2d& _size = {28, 20})
		: Actor(_map, _pos, _size), sprite(AnimatedSpritePresets::DecoyBlink)
	{
		setValidScaleAndOrigin(sprite, {14, 10});
	}
	
	void update(double deltaTime)
	{
		sprite.updateFrame(deltaTime);
	}
	
	
};

#endif // DECOYACTOR_HPP_INCLUDED
