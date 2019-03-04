#include <iostream>
#include <SFML/Graphics.hpp>
#include "../Headers/Input.hpp"
#include "../Headers/SplashScreen.hpp"
#include "../Headers/Scene.hpp"
#include "../Headers/HUD.hpp"
#include "../Headers/Levels.hpp"

int main()
{
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Robotycy&Magicy");
    
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
    Input::addKeyMapping(Action::nextLevel,   	sf::Keyboard::Space);
    Input::addKeyMapping(Action::use,   		sf::Keyboard::E);
    Input::addKeyMapping(Action::restart,   	sf::Keyboard::R);
    Input::addKeyMapping(Action::escape,  	 	sf::Keyboard::Escape);
    Input::addKeyMapping(Action::menuStart, 	sf::Keyboard::Space);
    
    LevelManager levelManager;
    SplashScreen::load();
    
    sf::Event event;
    sf::Clock clock;
    sf::Time elapsedTime, lastTime;
    float deltaTime = 0;
    
    
    sf::View gameView({0,0,800,600});
    gameView.setViewport({0,0,1,1});
    gameView.setCenter(400, 300);
    
    window.setView(gameView);
    
    HUD hud;
    
    hud.showMenu = true;
    
    constexpr double frameRefreshLimiter = 0.016;
    double displayCounter = 0;
    
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
        
        /**/
        
        
        
        if(isWindowFocused)
		{			
			if(hud.isSplashScreen())
			{
				if(Input::isTapped(Action::escape))
				{
					levelManager.unload();
					hud.showMenu = true;
					hud.clearSplashScrean();
					Input::updateKeyStates();
					continue;
				}
				
				if(Input::isTapped(Action::nextLevel))
				{
					hud.nextSplashScreen();
					Input::updateKeyStates();
				}
			}
			else if(hud.showMenu)
			{
				if(Input::isTapped(Action::escape))
				{
					window.close();
					continue;
				}
				
				if(Input::isTapped(Action::menuStart))
				{
					levelManager.loadLevel(1);
					Input::updateKeyStates();
					//hud.setSplashScreen("story");
					hud.showMenu = false;
				}
			}
			else if(levelManager.isLoaded())
			{
				hud.updateValuesFromScene(levelManager.getScene());
				
				if(Input::isTapped(Action::escape))
				{
					levelManager.unload();
					hud.showMenu = true;
					continue;
				}
				
				if(levelManager.getScene().isGameWon)
				{
					if(Input::isTapped(Action::nextLevel))
					{
						levelManager.loadNextLevel();
						Input::updateKeyStates();
						if(levelManager.areLevelsCompleted())
						{
							levelManager.unload();
							hud.showMenu = true;
							hud.setSplashScreen("gameWon");
							continue;
						}
					}
				}
				else
				{
					if(Input::isTapped(Action::restart))
					{
						levelManager.getScene().restart();
						Input::updateKeyStates();
					}
				}
				
				
				gameView.setCenter(levelManager.getScene().getViewCenter());
				window.setView(gameView);
				
				double tempDeltaTime = deltaTime;
				double subDeltaTime = 0;
				constexpr double maxSubDeltaTime = 0.016;
				
				while(tempDeltaTime > 0)
				{
					if(tempDeltaTime >= maxSubDeltaTime)
					{
						subDeltaTime = maxSubDeltaTime;
						tempDeltaTime -= maxSubDeltaTime;
					}
					else
					{
						subDeltaTime = tempDeltaTime;
						tempDeltaTime = 0;
					}
					
					levelManager.getScene().update(subDeltaTime);
					
				}
			}
			
			
		}
		
		displayCounter += deltaTime;
        
        if(displayCounter >= frameRefreshLimiter)
		{
			displayCounter = 0;
			
			window.clear(sf::Color::Black);
			
			if(hud.isSplashScreen())
			{
				window.draw(hud);
			}
			if(hud.showMenu)
			{
				window.draw(hud);
			}
			else
			{
				window.draw(levelManager.getScene());
				window.draw(hud);
			}
			
			if(deltaTime > frameRefreshLimiter)
			{
				std::cout << '\r' << (1/deltaTime) << " fps                ";
			}
			else
			{
				std::cout << '\r' << (1/frameRefreshLimiter) << " (" << (1/deltaTime) << ") fps          ";
			}
			
			//std::cin.get();
			window.display();
		}
        
    }
    
    return 0;
}
