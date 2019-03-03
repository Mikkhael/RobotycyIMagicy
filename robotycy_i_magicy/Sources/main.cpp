#include <iostream>
#include <SFML/Graphics.hpp>
#include "../Headers/Input.hpp"
#include "../Headers/Scene.hpp"
#include "../Headers/HUD.hpp"

int main()
{
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tak");
    
    bool isWindowFocused = true;
    
    Input::bindWindow(window);
    Input::addKeyMapping(Action::left,  		sf::Keyboard::A);
    Input::addKeyMapping(Action::left,  		sf::Keyboard::Left);
    Input::addKeyMapping(Action::right, 		sf::Keyboard::D);
    Input::addKeyMapping(Action::right, 		sf::Keyboard::Right);
    Input::addKeyMapping(Action::up,    		sf::Keyboard::W);
    Input::addKeyMapping(Action::up,    		sf::Keyboard::Up);
    Input::addKeyMapping(Action::down, 		 	sf::Keyboard::S);
    Input::addKeyMapping(Action::down,  		sf::Keyboard::Down);
    Input::addKeyMapping(Action::putCover, 		sf::Mouse::Left);
    Input::addKeyMapping(Action::putDecoy,   	sf::Keyboard::Space);
    Input::addKeyMapping(Action::use,   		sf::Keyboard::E);
    Input::addKeyMapping(Action::restart,   	sf::Keyboard::R);
    
    /**/
    Scene scene1( Vector2d(100, 100),
                 Scene::MapTileData::List{
                     Scene::MapTileData{
                         MapTile::Type::Grass1, MapTile::Type::Metal1, std::vector<sf::IntRect>{sf::IntRect{3, 3, 5, 20}, sf::IntRect{8, 17, 10, 3}}
                     },
                     Scene::MapTileData{
                         MapTile::Type::Path, MapTile::Type::Grass2, std::vector<sf::IntRect>{sf::IntRect{5, 2, 2, 30}}
                     },
                     Scene::MapTileData{
                         MapTile::Type::Metal1, std::vector<sf::IntRect>{sf::IntRect{10, 10, 4, 4}}
                     },
                     Scene::MapTileData{
                         MapTile::Type::Metal2, std::vector<sf::IntRect>{sf::IntRect{12, 12, 1, 1}}
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
                 	Scene::BookData{{17, 19}, 3, BookActor::Type::Decoy}
                 },
                 Scene::PlayerData::List{
                     Scene::PlayerData{ {4, 4}, 0, 0 }
                 }
                 
                 );
    
    scene1.load();
    
    /**/
    
    sf::Event event;
    sf::Clock clock;
    sf::Time elapsedTime, lastTime;
    float deltaTime = 0;
    
    
    sf::View gameView({0,0,800,600});
    gameView.setViewport({0,0,1,1});
    gameView.setCenter(400, 300);
    
    window.setView(gameView);
    
    HUD hud;
    
    while(window.isOpen())
    {
        while(window.pollEvent(event))
        {
            
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if(event.type == sf::Event::GainedFocus)
			{
				isWindowFocused = true;
			}
			else if(event.type == sf::Event::LostFocus)
			{
				isWindowFocused = false;
			}
        }
        
        elapsedTime = clock.getElapsedTime();
        deltaTime = (elapsedTime - lastTime).asSeconds();
        lastTime = elapsedTime;
        
        Input::updateKeyStates();
        
        window.clear(sf::Color::Black);
        
        /**/
        
        
        
        if(isWindowFocused)
		{
			if(Input::isTapped(Action::restart))
			{
				scene1.restart();
			}
			
			gameView.setCenter(scene1.getViewCenter());
			window.setView(gameView);
			
			scene1.update(deltaTime);
		}
        
        window.draw(scene1);
        hud.updateValuesFromScene(scene1);
        window.draw(hud);
        
        /**/
        
        std::cout << '\r' << (1/deltaTime) << " fps";
        //std::cin.get();
        window.display();
        
        
    }
    
    return 0;
}
