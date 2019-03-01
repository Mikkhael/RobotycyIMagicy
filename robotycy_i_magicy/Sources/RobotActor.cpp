#include <SFML/Graphics.hpp>
#include "../Headers/RobotActor.hpp"


RobotCircleActor::RobotCircleActor(Map& _map, const Vector2d& _pos, const Vector2d& _size)
	: Actor(_map, _pos, _size)
{
	bodySprite.setTexture(TextureManager::get("assets/textures.png"));
	bodySprite.setTextureRect({0, 96, 30, 30});
	headSprite.setTexture(TextureManager::get("assets/textures.png"));
	headSprite.setTextureRect({0, 128, 26, 26});
	setValidScaleAndOrigin(bodySprite, {30, 30});
	setValidScaleAndOrigin(headSprite, {26, 26});
	std::cout << getScene().getPlayer().getPosition() << std::endl;
}
