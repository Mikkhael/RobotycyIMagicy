#ifndef MAPTILE_HPP_INCLUDED
#define MAPTILE_HPP_INCLUDED

#include "Vectors.hpp"
#include "TextureManager.hpp"
#include <map>

class MapTile : public sf::RectangleShape
{
public:
    enum Type {Grass1, Grass2, Path, Metal1, Metal2, Black};
private:
    
    constexpr static int Size = 32;
    
    const static std::map<Type, sf::IntRect> typeToTextureRect;
    
    Type type;
    Vector2i position;
    bool walkable;
    
public:
    
    MapTile(Type _type, const Vector2i& _position, bool _walkable = false)
        : type(_type), position(_position), walkable(_walkable)
    {
        setSize({Size, Size});
        setTexture(&textureManager.get("assets/textures.png"));
        setTextureRect(typeToTextureRect.at(type));
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

#endif // MAPTILE_HPP_INCLUDED
