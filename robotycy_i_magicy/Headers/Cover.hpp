#ifndef COVER_HPP_INCLUDED
#define COVER_HPP_INCLUDED

#include "Actor.hpp"
#include "Animations.hpp"

class CoverActor : public Actor
{
	AnimatedSprite sprite;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform = states.transform.combine(getTransform());
		target.draw(sprite, states);
    }
    
	bool active = true;
	
public:
	
	
	bool isActive() const
	{
		return active;
	}
	
	void setInactive()
	{
		active = false;
		sprite.noResetWheOnceCompleted = true;
		sprite.setPreset(AnimatedSpritePresets::CoverFade);
		sprite.reset(AnimationState::Once);
	}
	
	CoverActor(Map& _map, const Vector2d& _pos = {0,0}, const Vector2d& _size = {32, 32})
		: Actor(_map, _pos, _size), sprite(AnimatedSpritePresets::CoverBlink)
	{
		setValidScaleAndOrigin(sprite, {16, 16});
	}
	
	void update(double deltaTime)
	{
		sprite.updateFrame(deltaTime);
	}
	
	
};

#endif // COVER_HPP_INCLUDED
