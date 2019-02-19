#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "Vectors.hpp"
#include "MapTile.hpp"

class Map : public sf::Drawable, public SimpleTransformable
{
    std::vector<MapTile*> mapTiles;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform = states.transform.combine(getTransform());
        for(auto& mapTile : mapTiles)
        {
            target.draw(*mapTile, states);
        }
    }
    
public:
    
    void addNewMapTile(MapTile* mapTile)
    {
        mapTiles.push_back(mapTile);
    }
    
    ~Map()
    {
        for(auto i = mapTiles.begin(); i<mapTiles.end(); i++)
        {
            delete *i;
        }
    }
    
};

#endif // MAP_HPP_INCLUDED
