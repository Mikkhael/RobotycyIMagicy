#include <iostream>
#include <SFML/Graphics.hpp>
#include "../Headers/Input.hpp"
#include "../Headers/Map.hpp"

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
    
    Map map({30, 30});
    map.getTile(2, 4).set(MapTile::Type::Grass1);
    map.getTile(2, 5).set(MapTile::Type::Metal1);
    map.getTile(3, 4).set(MapTile::Type::Grass2);
    
    sf::Event event;
    while(window.isOpen())
    {
        while(window.pollEvent(event))
        {
            
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        
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
        
        window.draw(map);
        
        window.display();
    }
    
    return 0;
}
