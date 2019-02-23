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
    
    const float tileSize;
    
    Vector2d positionToCoords(const Vector2d& position) const
    {
        return (position - getPosition()) / tileSize;
    }
    Vector2d coordsToPosition(const Vector2d& coords) const
    {
        return coords * tileSize + getPosition();
    }
    
    
    void setTile(unsigned int x, unsigned int y, MapTile::Type mapTile)
    {
        if(x < size.x && y < size.y)
        {
            grid[y][x].setType(mapTile);
        }
    }
    void setTile(const Vector2u& pos, MapTile::Type mapTile)
    {
        if(pos.x < size.x && pos.y < size.y)
        {
            grid[pos.y][pos.x].setType(mapTile);
        }
    }
    
    bool areCoordsGood(const Vector2u& coords)
    {
        return coords.x < size.x && coords.y < size.y;
    }
    
    bool isTileWalkable(const Vector2u& coords)
    {
        return areCoordsGood(coords) ? getTile(coords).isWalkable() : false;
    }
    
    bool isTileOpaque(const Vector2u& coords)
    {
        return !isTileWalkable(coords);
    }
    
    MapTile& getTile(unsigned int x, unsigned int y)
    {
        return grid[y][x];
    }
    MapTile& getTile(const Vector2u& pos)
    {
        return grid[pos.y][pos.x];
    }
    
    sf::FloatRect getTileRect(const Vector2u& coords)
    {
        return sf::FloatRect( getPosition().x + coords.x * tileSize, getPosition().y + coords.y * tileSize, tileSize * getScale().x, tileSize * getScale().y);
    }
    
    void setTileRect(MapTile::Type mapTile, const sf::IntRect& rect)
    {
        for(int i=0; i<rect.height; i++)
        {
            for(int j=0; j<rect.width; j++)
            {
                setTile(j+rect.left, i+rect.top, mapTile);
            }
        }
    }
    
    void setTileRects(MapTile::Type mapTileFill, const std::vector< sf::IntRect >& rects)
    {
        for(auto& rect : rects)
        {
            setTileRect(mapTileFill, rect);
        }
    }
    void setTileRectsWithBorder(MapTile::Type mapTileFill, MapTile::Type border, const std::vector< sf::IntRect >& rects)
    {
        for(auto& rect : rects)
        {
            for(int i = -1; i <= rect.width ; i++)
            {
                setTile(rect.left + i, rect.top - 1, border);
                setTile(rect.left + i, rect.top + rect.height, border);
            }
            for(int i = 0; i <= rect.height; i++)
            {
                setTile(rect.left - 1, rect.top + i, border);
                setTile(rect.left + rect.width, rect.top + i, border);
            }
        }
        
        for(auto& rect : rects)
        {
            setTileRect(mapTileFill, rect);
        }
    }
    
    Vector2u getSize()
    {
        return size;
    }
    
    Map(const Vector2u& _size, int _tileSize = 32)
        : size(_size), tileSize(_tileSize)
    {
        grid.reserve(size.y);
        for(unsigned int i=0u; i<size.y; i++)
        {
            grid.push_back({});
            grid[i].reserve(size.x);
            for(unsigned int j=0u; j<size.x; j++)
            {
                grid[i].emplace_back(tileSize, Vector2d(j, i));
            }
        }
    }
    
};

#endif // MAP_HPP_INCLUDED
