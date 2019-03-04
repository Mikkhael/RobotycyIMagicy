#ifndef NEXTLEVELACTOR_HPP_INCLUDED
#define NEXTLEVELACTOR_HPP_INCLUDED

#include "Actor.hpp"

class NextLevelActor : public Actor
{
	sf::Sprite sprite;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform = states.transform.combine(getTransform());
        if(isActive())
		{
			target.draw(sprite, states);
		}
    }
    
	bool active = false;
	
public:
	
	
	bool isActive() const
	{
		return active;
	}
	
	void setInactive()
	{
		active = false;
	}
	void setActive()
	{
		active = true;
	}
	
	NextLevelActor(Map& _map, const Vector2d& _pos = {0,0}, const Vector2d& _size = {18, 18})
		: Actor(_map, _pos, _size)
	{
		sprite.setTexture(TextureManager::get("assets/textures.png"));
		sprite.setTextureRect({48, 64, 16, 16});
		setValidScaleAndOrigin(sprite, {16, 16});
	}
	
	~NextLevelActor(){};
};

#endif // NEXTLEVELACTOR_HPP_INCLUDED
