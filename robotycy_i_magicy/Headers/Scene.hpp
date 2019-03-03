#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include "Map.hpp"
#include "MageActor.hpp"
#include "RobotActor.hpp"
#include "Cover.hpp"
#include "DecoyActor.hpp"
#include "BookActor.hpp"

class Scene : public sf::Drawable
{	
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(map, states);
        performDrawRoutine(target, states, others);
        performDrawRoutine(target, states, books);
        performDrawRoutine(target, states, decoys);
        performDrawRoutine(target, states, enemies);
        performDrawRoutine(target, states, covers);
        
        for(auto& actor : enemies)
        {
			actor->drawViewField(target, states);
        }
        target.draw(*player, states);
        
        //std::cout << "DREW " << std::endl;
    }
    
    template<typename T>
    void performCleanRoutine(T& list)
    {
    	for(auto it = list.begin(); it != list.end(); it++)
        {
            delete *it;
        }
        list.clear();
    }
    
    template<typename T>
    void performSetupRoutine(T& datas)
    {
    	for(auto& data : datas)
        {
            data.addToScene(*this);
        }
    }
    
    template<typename T>
    void performUpdateRoutine(double deltaTime, T& actors)
    {
		for(auto& actor : actors)
		{
			actor->update(deltaTime);
		}
    }
    
    template<typename T>
    void performDrawRoutine(sf::RenderTarget& target, sf::RenderStates states, T& actors) const
    {
		for(auto& actor : actors)
        {
            target.draw(*actor, states);
        }
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
					for(unsigned int i=0; i<path.size(); i++)
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
    
    class BookData
    {
    	Vector2u position;
    	int charge;
    	BookActor::Type type;
    	
	public:
		
        using ListR = const std::vector<BookData>&;
        using List = std::vector<BookData>;
        
		BookData(const Vector2u& _position, int _charge, BookActor::Type _type)
            : position(_position), charge(_charge), type(_type)
        {}
        
        void addToScene(Scene& scene) const
        {
            scene.books.push_back(new BookActor(type, charge, scene.map, scene.map.getTileCenterPosition(position)));
        }
		
    };

    Map map;
    PlayerMageActor* player = nullptr;
    std::vector<EnemyActor*> enemies;
    std::vector<CoverActor*> covers;
    std::vector<DecoyActor*> decoys;
    std::vector<BookActor*> books;
    std::vector<Actor*> others;
    
    const MapTileData::List mapTileDatas;
    const RobotData::List robotDatas;
    const PlayerData::List playerDatas;
    const BookData::List bookDatas;
    
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
    	if(enemy.canSeeOverMap(point, map))
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
    	for(unsigned int i=0; i<decoys.size(); i++)
		{
			if(decoys[i]->isActive && checkIfEnemyCanSeePoint(enemy, decoys[i]->getPosition()))
			{
				return decoys[i];
			}
		}
		return nullptr;
    }
    
    bool isGameLost = false;
    void update(double deltaTime)
    {
    	if(isGameLost)
		{
			
		}
		else
		{
			bool isPlayerSeen = false;
			performUpdateRoutine(deltaTime, others);
			performUpdateRoutine(deltaTime, books);
			performUpdateRoutine(deltaTime, covers);
			performUpdateRoutine(deltaTime, decoys);
			performUpdateRoutine(deltaTime, enemies);
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
				isGameLost = true;
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
			for(auto& book : books)
			{
				if(book->isTaken())
				{
					continue;
				}
				if(map.positionToCoords(book->getPosition()) == map.positionToCoords(player->getPosition()))
				{
					book->pickUp(*player);
				}
			}
			
			if(Input::isTapped(Action::putCover) && player->coversToPlace > 0)
			{
				Vector2i coords = map.positionToCoords(Input::getMouseView());
				Vector2d position = map.getTileCenterPosition(coords);
				//std::cout << position << "  " << (player->coversToPlace) << "   " << covers.size() << "   " << map.isTileWalkable(coords) << std::endl;
				bool canPlace = map.isTileWalkable(coords);
				for(unsigned int i=0; i<covers.size() && canPlace; i++)
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
		}
		
    	
		
        //std::cout << "UPDATED " << std::endl;
    }
    
    bool isLoaded = false;
    bool isSetup = false;
    void load()
    {
    	if(!isLoaded)
		{
			for(auto& data : mapTileDatas)
			{
				data.addToScene(*this);
			}
			setup();
			isLoaded = true;
		}
        //std::cout << "LOADED" << std::endl;
    }
    
    void unload()
    {
    	if(isLoaded)
		{
			clean();
			isLoaded = false;
		}
    }
    
    void setup()
    {
    	if(!isSetup)
		{
			isGameLost = false;
			performSetupRoutine(robotDatas);
			performSetupRoutine(playerDatas);
			performSetupRoutine(bookDatas);
			isSetup = true;
		}
		//std::cout << "SET UP" << std::endl;
    }
    
    
    
    void clean()
    {
    	if(isSetup)
		{
			performCleanRoutine(others);
			performCleanRoutine(enemies);
			performCleanRoutine(covers);
			performCleanRoutine(decoys);
			performCleanRoutine(books);
			if(player)
			{
				delete player;
				player = nullptr;
			}
			isSetup = false;
		}
		//std::cout << "CLEANED" << std::endl;
    }
    
    void restart()
    {
    	clean();
    	setup();
    }
    
    Scene(const Vector2u& mapSize, MapTileData::ListR _mapTileDatas, RobotData::ListR _robotDatas, BookData::ListR _bookDatas, PlayerData::ListR _playerDatas)
        : 
            map(*this, mapSize, 32),
            mapTileDatas(_mapTileDatas),
            robotDatas(_robotDatas),
            playerDatas(_playerDatas),
            bookDatas(_bookDatas)
    {
    }
    
    ~Scene()
    {
        unload();
    }    
};

#endif // SCENE_HPP_INCLUDED
