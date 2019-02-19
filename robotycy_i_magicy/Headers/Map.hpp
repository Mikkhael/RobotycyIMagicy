#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "Vectors.hpp"
#include "MapTile.hpp"

class Map : public sf::Drawable, public SimpleTransformable
{
    
    Vector2u size;
    std::vector<std::vector< MapTile > > grid;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform = states.transform.combine(getTransform());
        for(auto i = 0u; i<grid.size(); i++)
        {
            for(auto j = 0u; j<grid[0].size(); j++)
            {
                target.draw(grid[i][j], states);
            }
        }
    }
    
public:
    
    MapTile& getTile(unsigned int x, unsigned int y)
    {
        return grid[y][x];
    }
    MapTile& getTile(const Vector2u& pos)
    {
        return grid[pos.y][pos.x];
    }
    
    Vector2u getSize()
    {
        return size;
    }
    
    Map(const Vector2u& _size)
        : size(_size), grid(size.y, std::vector<MapTile>(size.x))
    {
        for(unsigned int i=0u; i<size.y; i++)
        {
            for(unsigned int j=0u; j<size.x; j++)
            {
                grid[i][j] = MapTile({j, i});
            }
        }
    }
    
};

#endif // MAP_HPP_INCLUDED
