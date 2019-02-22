#include <iostream>
#include <SFML/Graphics.hpp>
#include "../Headers/Input.hpp"
#include "../Headers/Map.hpp"
#include "../Headers/MageActor.hpp"

int main()
{
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tak");
    
    Input::bindWindow(window);
    Input::addKeyMapping(Action::left,  sf::Keyboard::A);
    Input::addKeyMapping(Action::left,  sf::Keyboard::Left);
    Input::addKeyMapping(Action::right, sf::Keyboard::D);
    Input::addKeyMapping(Action::right, sf::Keyboard::Right);
    Input::addKeyMapping(Action::up,    sf::Keyboard::W);
    Input::addKeyMapping(Action::up,    sf::Keyboard::Up);
    Input::addKeyMapping(Action::down,  sf::Keyboard::S);
    Input::addKeyMapping(Action::down,  sf::Keyboard::Down);
    Input::addKeyMapping(Action::shoot, sf::Mouse::Left);
    Input::addKeyMapping(Action::use,   sf::Keyboard::Space);
    Input::addKeyMapping(Action::use,   sf::Keyboard::E);
    Input::addKeyMapping(Action::use,   sf::Mouse::Right);
    
    Map map({30, 30}, 32);
    map.setTileRectsWithBorder(MapTile::Type::Grass1, MapTile::Type::Metal1, {{3, 3, 5, 20}, {8, 17, 10, 3}});
    map.setTileRectsWithBorder(MapTile::Type::Path, MapTile::Type::Grass2, {{5, 2, 2, 30}});
    
    map.setTileRect(MapTile::Type::Metal1, {10, 10, 4, 4});
    map.setTile(12, 12, MapTile::Type::Metal2);
    
    
    PlayerMageActor player(map, {100, 100}, {28, 28});
    MageActor x(map, {100, 150});
    x.goToDestination({300, 300});
    
    sf::Event event;
    sf::Clock clock;
    sf::Time elapsedTime, lastTime;
    float deltaTime = 0;
    
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
        
        window.clear(sf::Color::Red);
        
        if(Input::isTapped(Action::use))
        {
            std::cout << Input::getMouseView() << std::endl;
        }
        
        if(Input::isPressed(Action::shoot))
        {
            window.clear(sf::Color::Blue);
        }
        
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
        
        player.update(deltaTime);
        x.update(deltaTime);
        
        
        window.draw(map);
        window.draw(x);
        window.draw(player);
        
        window.draw(tempActor);
        
        window.display();
    }
    
    return 0;
}
