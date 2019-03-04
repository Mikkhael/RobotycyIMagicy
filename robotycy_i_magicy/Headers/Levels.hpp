#ifndef LEVELS_HPP_INCLUDED
#define LEVELS_HPP_INCLUDED

#include "Scene.hpp"

class LevelManager
{
	std::vector<Scene> scenes;
	
	Scene* currentScene = nullptr;
	int currentLevelId = -1;
	
public:
	
	const int levelsCount = 2;
	
	LevelManager()
	{
		scenes.reserve(levelsCount);
		loadLevelsScenes();
	}
	
	void unload()
	{
		if(currentScene)
		{
			currentScene->unload();
		}
		currentScene = nullptr;
		currentLevelId = -1;
	}
	
	bool isLoaded()
	{
		return currentScene != nullptr;
	}
	
	void loadLevel(unsigned int id = 0)
	{
		if(currentScene)
		{
			currentScene->unload();
		}
		currentLevelId = id;
		currentScene = &scenes[id];
		currentScene->load();
	}
	
	void loadNextLevel()
	{
		currentLevelId++;
		if(currentLevelId >= levelsCount)
		{
			currentLevelId = 0;
		}
		loadLevel(currentLevelId);
	}
	
	Scene& getScene()
	{
		if(!currentScene)
		{
			throw 123;
		}
		return *currentScene;
	}
	
	
private:
	void loadLevelsScenes()
	{
		if(scenes.size() > 0)
		{
			scenes.clear();
		}
		
		scenes.reserve(levelsCount);
		
		scenes.emplace_back( // LEVEL 1
		
			 Vector2d(100, 100),
                 Scene::MapTileData::List{
                     Scene::MapTileData{
                         MapTile::Type::Grass1,
                         MapTile::Type::Metal1,
                         std::vector<sf::IntRect>{
                         	sf::IntRect{3, 3, 5, 20},
                         	sf::IntRect{8, 17, 10, 3}
						 }
                     },
                     Scene::MapTileData{
                         MapTile::Type::Path,
                         MapTile::Type::Grass2,
                         std::vector<sf::IntRect>{
                         	sf::IntRect{5, 2, 2, 30}
						 }
                     },
                     Scene::MapTileData{
                         MapTile::Type::Metal1,
                         std::vector<sf::IntRect>{
                         	sf::IntRect{10, 10, 4, 4}
						 }
                     },
                     Scene::MapTileData{
                         MapTile::Type::Metal2,
                         std::vector<sf::IntRect>{
                         	sf::IntRect{12, 12, 1, 1}
						 }
                     }
                 },
                 Scene::RobotData::List{
                     Scene::RobotData{
                         false, {3, 3}, {
                         	{{3, 3}, 1},
                         	{{4, 6}, 2},
                         	{{7, 6}, 0},
                         	}, false
                     }/*,
                     Scene::RobotData{
                         true, {6, 12}, {}, false
                     }*/
                 },
                 Scene::BookData::List{
                 	Scene::BookData{{17, 17}, 3, BookActor::Type::Cover},
                 	Scene::BookData{{17, 19}, 3, BookActor::Type::Decoy},
                 	Scene::BookData{{5, 29}}
                 },
                 Scene::PlayerData::List{
                     Scene::PlayerData{ {4, 2}, 0, 0 }
                 },
                 Vector2d{18, 18},
                 "controlsBasics"
                 
		);
		
		scenes.emplace_back( // LEVEL 2
		
			 Vector2d(100, 100),
                 Scene::MapTileData::List{
                     Scene::MapTileData{
                         MapTile::Type::Path,
                         MapTile::Type::Metal1,
                         std::vector<sf::IntRect>{
                         	sf::IntRect{3, 3, 5, 20},
                         	sf::IntRect{8, 17, 10, 3}
						 }
                     },
                     Scene::MapTileData{
                         MapTile::Type::Metal2,
                         MapTile::Type::Metal1,
                         std::vector<sf::IntRect>{
                         	sf::IntRect{5, 2, 2, 30}
						 }
                     },
                     Scene::MapTileData{
                         MapTile::Type::Metal1,
                         std::vector<sf::IntRect>{
                         	sf::IntRect{10, 10, 4, 4}
						 }
                     },
                     Scene::MapTileData{
                         MapTile::Type::Metal2,
                         std::vector<sf::IntRect>{
                         	sf::IntRect{12, 12, 1, 1}
						 }
                     }
                 },
                 Scene::RobotData::List{
                     Scene::RobotData{
                         false, {4, 3}, {
                         	{{4, 3}, 1},
                         	{{4, 18}, 1},
                         	{{10, 18}, 1},
                         	}, true
                     },
                     Scene::RobotData{
                         true, {6, 12}, {}, false
                     }
                 },
                 Scene::BookData::List{
                 	Scene::BookData{{17, 17}, 300, BookActor::Type::Decoy},
                 	Scene::BookData{{17, 19}, 300, BookActor::Type::Cover}
                 },
                 Scene::PlayerData::List{
                     Scene::PlayerData{ {15, 18}, 2, 1 }
                 },
                 Vector2d{5, 3}
                 
		);
		
	}
};

#endif // LEVELS_HPP_INCLUDED
