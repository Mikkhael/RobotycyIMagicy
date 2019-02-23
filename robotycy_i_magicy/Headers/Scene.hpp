#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include "Map.hpp"
#include "MageActor.hpp"
#include "RobotActor.hpp"

class Scene : public sf::Drawable
{
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(map, states);
        for(auto& actor : actors)
        {
            target.draw(*actor, states);
        }
        
        //std::cout << "DREW " << actors.size() << " actors" << std::endl;
    }
public:
    
    class MapTileData
    {
        MapTile::Type type;
        MapTile::Type border;
        bool isBorder;
        std::vector<sf::IntRect> rects;
        
    public:
        
        using ListR = const std::vector<MapTileData>&;
        using List = std::vector<MapTileData>;
        
        MapTileData(MapTile::Type _type, const std::vector<sf::IntRect>& _rects)
            : type(_type), isBorder(false), rects(_rects)
        {
        }
        
        MapTileData(MapTile::Type _type, MapTile::Type _border, const std::vector<sf::IntRect>& _rects)
            : type(_type), border(_border), isBorder(true), rects(_rects)
        {
        }
        
        void addToScene(Scene& scene) const
        {
            if(isBorder)
            {
                scene.map.setTileRectsWithBorder(type, border, rects);
            }
            else
            {
                scene.map.setTileRects(type, rects);
            }
        }
    };

    class RobotData
    {
        bool isCircle;
        Vector2d position;
        std::vector<Vector2u> path;
        bool alternate = false;
    public:
        
        using ListR = const std::vector<RobotData>&;
        using List = std::vector<RobotData>;
        
        RobotData(bool _isCircle, const Vector2u& _position, const std::vector<Vector2u>& _path, bool _alternate)
            : isCircle(_isCircle), position(_position), path(_path), alternate(_alternate)
        {}
            
        void addToScene(Scene& scene) const
        {
            if(isCircle)
            {
                scene.actors.push_back(new RobotCircleActor(scene.map, position));
            }
            else
            {
                RobotActor* r = new RobotActor(scene.map, position);
                if(path.size() > 0)
                {
                    r->goToDestination(scene.map.coordsToPosition(path[0]));
                }
                scene.actors.push_back(r);
            }
        }
    };
    
    class PlayerData
    {
        Vector2u position;
        
    public:
        
        using ListR = const std::vector<PlayerData>&;
        using List = std::vector<PlayerData>;
        
        
        PlayerData(const Vector2u& _position)
            : position(_position)
        {}
        
        void addToScene(Scene& scene) const
        {
            PlayerMageActor* p = new PlayerMageActor(scene.map, position, {28, 28});
            scene.actors.push_back(p);
        }
    };

    Map map;
    std::vector<Actor*> actors;
    
    
    const MapTileData::List mapTileDatas;
    const RobotData::List robotDatas;
    const PlayerData::List playerDatas;
    
    
    void update(double deltaTime)
    {
        for(auto& actor : actors)
        {
            actor->update(deltaTime);
        }
        //std::cout << "UPDATED " << actors.size() << " actors" << std::endl;
    }
    
    void load()
    {
        for(auto& mapTileData : mapTileDatas)
        {
            mapTileData.addToScene(*this);
        }
        for(auto& robotData : robotDatas)
        {
            robotData.addToScene(*this);
        }
        for(auto& playerData : playerDatas)
        {
            playerData.addToScene(*this);
        }
    }
    
    Scene(const Vector2u& mapSize, MapTileData::ListR _mapTileDatas, RobotData::ListR _robotDatas, PlayerData::ListR _playerDatas)
        : 
            map(mapSize, 32),
            mapTileDatas(_mapTileDatas),
            robotDatas(_robotDatas),
            playerDatas(_playerDatas)
    {
    }
    
    ~Scene()
    {
        for(auto it = actors.begin(); it != actors.end(); it++)
        {
            delete *it;
        }
    }    
};

#endif // SCENE_HPP_INCLUDED
