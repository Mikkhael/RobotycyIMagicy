#ifndef MAPTILE_HPP_INCLUDED
#define MAPTILE_HPP_INCLUDED

#include "Vectors.hpp"
#include "TextureManager.hpp"
#include <map>
#include <unordered_set>

class MapTile : public sf::Drawable
{
    sf::RectangleShape sprite;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(type != Type::Black)
        {
            target.draw(sprite, states);
        }
    }
    
public:
    enum class Type {Grass1, Grass2, Path, Metal1, Metal2, Black};
    
private:
    
    const float size = 32;
    
    const static std::map<Type, sf::IntRect> typeToTextureRect;
    const static std::unordered_set<int>    nonWalkableTypes;
    
    Type type;
    Vector2u position;
    
public:
    
    void setType(Type _type)
    {
        type = _type;
        sprite.setTextureRect(typeToTextureRect.at(type));
    }
    
    bool isWalkable() const
    {
        return nonWalkableTypes.count(int(type)) <= 0;
    }
    
    MapTile(int _size = 32, const Vector2u& _position = {0,0}, Type _type = Type::Black)
        : size(_size), position(_position)
    {
        sprite.setSize({size, size});
        sprite.setPosition(position * size);
        sprite.setTexture(&TextureManager::get("assets/textures.png"));
        setType(_type);
    }
    
};

constexpr static int spriteSize = 16;
const std::map<MapTile::Type, sf::IntRect> MapTile::typeToTextureRect =
{
    {MapTile::Type::Grass1, sf::IntRect{0, 0, spriteSize, spriteSize}},
    {MapTile::Type::Grass2, sf::IntRect{spriteSize, 0, spriteSize, spriteSize}},
    {MapTile::Type::Path,   sf::IntRect{spriteSize*2, 0, spriteSize, spriteSize}},
    {MapTile::Type::Metal1, sf::IntRect{0, spriteSize, spriteSize, spriteSize}},
    {MapTile::Type::Metal2, sf::IntRect{spriteSize, spriteSize, spriteSize, spriteSize}},
    {MapTile::Type::Black,  sf::IntRect{spriteSize*2, spriteSize, spriteSize, spriteSize}},
};
const std::unordered_set<int> MapTile::nonWalkableTypes = 
{
    int(MapTile::Type::Black)
};

#endif // MAPTILE_HPP_INCLUDED
