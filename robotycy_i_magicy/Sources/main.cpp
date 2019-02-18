#include <iostream>
#include <SFML\Graphics.hpp>

int main()
{
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tak");
    
    
    std::cout << "Tak" << std::endl;
    
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
        
        window.clear(sf::Color::Red);
        window.display();
    }
    
    return 0;
}
