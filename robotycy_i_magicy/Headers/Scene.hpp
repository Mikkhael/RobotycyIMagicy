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
        for(auto& actor : others)
        {
            target.draw(*actor, states);
        }
        for(auto& actor : enemies)
        {
            target.draw(*actor, states);
        }
        for(auto& actor : enemies)
        {
			actor->drawViewField(target, states);
        }
        target.draw(*player, states);
        
        //std::cout << "DREW " << std::endl;
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
        Vector2u position;
        std::vector<std::pair<Vector2u, double> > path;
        bool alternate = false;
    public:
        
        using ListR = const std::vector<RobotData>&;
        using List = std::vector<RobotData>;
        
        RobotData(bool _isCircle, const Vector2u& _position, const std::vector<std::pair<Vector2u, double> >& _path, bool _alternate)
            : isCircle(_isCircle), position(_position), path(_path), alternate(_alternate)
        {}
            
        void addToScene(Scene& scene) const
        {
            if(isCircle)
            {
                scene.enemies.push_back(new RobotCircleActor(scene.map, scene.map.getTileCenterPosition(position)));
            }
            else
            {
                RobotActor* r = new RobotActor(scene.map, scene.map.getTileCenterPosition(position));
                if(path.size() == 1)
                {
                    r->goToDestination(scene.map.getTileCenterPosition(path[0].first));
                }
                else if(path.size() > 1)
				{
					std::vector<std::pair<Vector2d, double> > goodPath;
					for(int i=0; i<path.size(); i++)
					{
						goodPath.push_back(std::make_pair(scene.map.getTileCenterPosition(path[i].first), path[i].second));
					}
					if(alternate)
					{
						for(int i=path.size()-2; i>0; i--)
						{
							goodPath.push_back(std::make_pair(scene.map.getTileCenterPosition(path[i].first), path[i].second));
						}
					}
					r->setPatrolPath(goodPath);
				}
                scene.enemies.push_back(r);
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
            if(!scene.player)
			{
				PlayerMageActor* p = new PlayerMageActor(scene.map, scene.map.getTileCenterPosition(position), {28, 28});
				scene.player = p;
			}
        }
    };

    Map map;
    PlayerMageActor* player = nullptr;
    std::vector<EnemyActor*> enemies;
    std::vector<Actor*> others;
    
    const MapTileData::List mapTileDatas;
    const RobotData::List robotDatas;
    const PlayerData::List playerDatas;
    
    Vector2d getViewCenter()
    {
    	return player->getPosition() + (Input::getMouseView() - player->getPosition()) * 0.3;
    }
    
    bool checkIfEnemyCanSeePlayer(EnemyActor& enemy)
    {
    	return enemy.canDetectOverMapAndRect(player->getPosition(), map, {0,0,0,0});
    }
    
    void update(double deltaTime)
    {
    	bool isPlayerSeen = false;
        for(auto& actor : others)
        {
            actor->update(deltaTime);
        }
        for(auto& actor : enemies)
        {
            actor->update(deltaTime);
            if(!isPlayerSeen && checkIfEnemyCanSeePlayer(*actor))
			{
				isPlayerSeen = true;
			}
        }
        player->update(deltaTime);
        if(isPlayerSeen)
		{
			std::cout << "TAK\n";
		}
        //std::cout << "UPDATED " << std::endl;
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
        //std::cout << "LOADED" << std::endl;
    }
    
    Scene(const Vector2u& mapSize, MapTileData::ListR _mapTileDatas, RobotData::ListR _robotDatas, PlayerData::ListR _playerDatas)
        : 
            map(*this, mapSize, 32),
            mapTileDatas(_mapTileDatas),
            robotDatas(_robotDatas),
            playerDatas(_playerDatas)
    {
    }
    
    ~Scene()
    {
        for(auto it = others.begin(); it != others.end(); it++)
        {
            delete *it;
        }
        for(auto it = enemies.begin(); it != enemies.end(); it++)
        {
            delete *it;
        }
        delete player;
        player = nullptr;
    }    
};

#endif // SCENE_HPP_INCLUDED
