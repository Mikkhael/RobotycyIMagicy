#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include "Map.hpp"
#include "MageActor.hpp"
#include "RobotActor.hpp"
#include "Cover.hpp"
#include "DecoyActor.hpp"

class Scene : public sf::Drawable
{
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(map, states);
        for(auto& actor : others)
        {
            target.draw(*actor, states);
        }
        for(auto& actor : decoys)
        {
            target.draw(*actor, states);
        }
        for(auto& actor : enemies)
        {
            target.draw(*actor, states);
        }
        for(auto& actor : covers)
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
        int coversToPlace;
        int decoysToPlace;
        
    public:
        
        using ListR = const std::vector<PlayerData>&;
        using List = std::vector<PlayerData>;
        
        
        PlayerData(const Vector2u& _position, int _coversToPlace, int _decoysToPlace)
            : position(_position), coversToPlace(_coversToPlace), decoysToPlace(_decoysToPlace)
        {}
        
        void addToScene(Scene& scene) const
        {
            if(!scene.player)
			{
				PlayerMageActor* p = new PlayerMageActor(scene.map, scene.map.getTileCenterPosition(position), {28, 28});
				p->coversToPlace = coversToPlace;
				p->decoysToPlace = decoysToPlace;
				scene.player = p;
			}
        }
    };

    Map map;
    PlayerMageActor* player = nullptr;
    std::vector<EnemyActor*> enemies;
    std::vector<CoverActor*> covers;
    std::vector<DecoyActor*> decoys;
    std::vector<Actor*> others;
    
    const MapTileData::List mapTileDatas;
    const RobotData::List robotDatas;
    const PlayerData::List playerDatas;
    
    Vector2d getViewCenter()
    {
    	return player->getPosition() + (Input::getMouseView() - player->getPosition()) * 0.3;
    }
    
    
    bool checkIfEnemyCanSeePoint(EnemyActor& enemy, const Vector2d& point)
    {
    	if(enemy.isInCloseup(point))
		{
			return true;
		}
		if(!enemy.isInView(point))
		{
			return false;
		}
    	if(enemy.canSeeOverMap(player->getPosition(), map))
		{
			for(auto& cover : covers)
			{
				if(!enemy.canSeeOverRect(point, cover->getRect()))
				{
					return false;
				}
			}
			return true;
		}
		return false;
    }
    bool checkIfEnemyCanSeePlayer(EnemyActor& enemy)
    {
    	return checkIfEnemyCanSeePoint(enemy, player->getPosition());
    }
    
    DecoyActor* checkIfEnemySeesDecoy(EnemyActor& enemy)
    {
    	for(int i=0; i<decoys.size(); i++)
		{
			if(decoys[i]->isActive && checkIfEnemyCanSeePoint(enemy, decoys[i]->getPosition()))
			{
				return decoys[i];
			}
		}
		return nullptr;
    }
    
    void update(double deltaTime)
    {
    	bool isPlayerSeen = false;
        for(auto& actor : others)
        {
            actor->update(deltaTime);
        }
        for(auto& actor : covers)
        {
            actor->update(deltaTime);
        }
        for(auto& actor : decoys)
        {
            actor->update(deltaTime);
        }
        for(auto& actor : enemies)
        {
            actor->update(deltaTime);
        }
        player->update(deltaTime);
        
        
        for(auto& actor : enemies)
        {
            if(!isPlayerSeen && checkIfEnemyCanSeePlayer(*actor))
			{
				isPlayerSeen = true;
				break;
			}
        }
        if(isPlayerSeen)
		{
			//std::cout << "Seen" << std::endl;
		}
		
        for(auto& enemy : enemies)
        {
        	if(enemy->isActorDistracted())
			{
				continue;
			}
            DecoyActor* decoy = checkIfEnemySeesDecoy(*enemy);
            if(decoy)
			{
				decoy->setInactive();
				enemy->goToDistraction(decoy->getPosition());
			}
        }
		
        if(Input::isTapped(Action::putCover) && player->coversToPlace > 0)
		{
			Vector2i coords = map.positionToCoords(Input::getMouseView());
			Vector2d position = map.getTileCenterPosition(coords);
			//std::cout << position << "  " << (player->coversToPlace) << "   " << covers.size() << "   " << map.isTileWalkable(coords) << std::endl;
			bool canPlace = map.isTileWalkable(coords);
			for(int i=0; i<covers.size() && canPlace; i++)
			{
				if(covers[i]->getPosition() == position)
				{
					canPlace = false;
				}
			}
			if(canPlace)
			{
				player->coversToPlace--;
				covers.push_back(new CoverActor(map, position));
			}
		}
		
		if(Input::isTapped(Action::putDecoy) && player->decoysToPlace > 0)
		{
			player->decoysToPlace--;
			decoys.push_back(new DecoyActor(map, player->getPosition()));
		}
		
        //std::cout << "UPDATED " << std::endl;
    }
    
    void load()
    {
        setup();
        //std::cout << "LOADED" << std::endl;
    }
    
    void setup()
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
    
    void clean()
    {
    	for(auto it = others.begin(); it != others.end(); it++)
        {
            delete *it;
        }
        for(auto it = enemies.begin(); it != enemies.end(); it++)
        {
            delete *it;
        }
        for(auto it = covers.begin(); it != covers.end(); it++)
        {
            delete *it;
        }
        for(auto it = decoys.begin(); it != decoys.end(); it++)
        {
            delete *it;
        }
        if(player)
		{
			delete player;
			player = nullptr;
		}
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
        clean();
    }    
};

#endif // SCENE_HPP_INCLUDED
