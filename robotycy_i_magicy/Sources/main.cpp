#include <iostream>
#include <SFML/Graphics.hpp>
#include "../Headers/Input.hpp"
#include "../Headers/Scene.hpp"
#include "../Headers/HUD.hpp"

int main()
{
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tak");
    
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
    /*
    Map map({100, 100}, 32);
    map.setTileRectsWithBorder(MapTile::Type::Grass1, MapTile::Type::Metal1, {{3, 3, 5, 20}, {8, 17, 10, 3}});
    map.setTileRectsWithBorder(MapTile::Type::Path, MapTile::Type::Grass2, {{5, 2, 2, 30}});
    
    map.setTileRect(MapTile::Type::Metal1, {10, 10, 4, 4});
    map.setTile(12, 12, MapTile::Type::Metal2);
    
    
    PlayerMageActor player(map, {100, 100}, {28, 28});
    MageActor x(map, {100, 150});
    x.goToDestination({300, 300});
    
    RobotActor r1(map, {200, 200}, {32, 32});
    r1.goToDestination({250, 550});
    
    RobotCircleActor r2(map, {200, 400});
    
    /**/
    
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
        }
        
        elapsedTime = clock.getElapsedTime();
        deltaTime = (elapsedTime - lastTime).asSeconds();
        lastTime = elapsedTime;
        
        Input::updateKeyStates();
        
        window.clear(sf::Color::Black);
        /*
        constexpr float speed = 3000;
        if(Input::isPressed(Action::use))
        {
            if(Input::isPressed(Action::down))
            {
                map.move({0, -speed * deltaTime});
            }
            if(Input::isPressed(Action::up))
            {
                map.move({0, speed * deltaTime});
            }
            if(Input::isPressed(Action::left))
            {
                map.move({speed * deltaTime, 0});
            }
            if(Input::isPressed(Action::right))
            {
                map.move({-speed * deltaTime, 0});
            }
        }
        
        r1.update(deltaTime);
        r2.update(deltaTime);
        player.update(deltaTime);
        x.update(deltaTime);
        
        
        window.draw(map);
        
        window.draw(r1);
        window.draw(r2);
        
        window.draw(x);
        window.draw(player);
        /**/
        
        /**/
        
        
        if(Input::isTapped(Action::restart))
		{
			scene1.restart();
		}
        
        
        gameView.setCenter(scene1.getViewCenter());
		window.setView(gameView);
		
        scene1.update(deltaTime);
        
        
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
