#ifndef BOOKACTOR_HPP_INCLUDED
#define BOOKACTOR_HPP_INCLUDED

#include "Actor.hpp"
#include "MageActor.hpp"

class BookActor : public Actor
{
	sf::Sprite sprite;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
    	if(!isTaken())
		{
			states.transform = states.transform.combine(getTransform());
			target.draw(sprite, states);
		}
    }
	
	bool taken = false;
	
public:
	
	enum class Type{Decoy, Cover, Empty} type;
	int charge = 0;
	
	bool isTaken() const
	{
		return taken;
	}
	
	void pickUp(PlayerMageActor& player)
	{
		taken = true;
		switch(type)
		{
		case Type::Decoy:
			{
				player.decoysToPlace += charge;
				break;
			}
		case Type::Cover:
			{
				player.coversToPlace += charge;
				break;
			}
		default:
			{
				break;
			}
		}
	}
	
	BookActor(Type type_, int charge_, Map& _map, const Vector2d& _pos = {0,0})
		: Actor(_map, _pos, {20, 27}), type(type_), charge(charge_)
	{
		sprite.setTexture(TextureManager::get("assets/textures.png"));
		switch(type)
		{
		case Type::Decoy:
			{
				sprite.setTextureRect({48, 48, 12, 16});
				break;
			}
		case Type::Cover:
			{
				sprite.setTextureRect({64, 48, 12, 16});
				break;
			}
		case Type::Empty:
			{
				sprite.setTextureRect({80, 48, 12, 16});
				break;
			}
		}
		setValidScaleAndOrigin(sprite, {12, 16});
	}
	
	~BookActor(){}
};

#endif // BOOKACTOR_HPP_INCLUDED
