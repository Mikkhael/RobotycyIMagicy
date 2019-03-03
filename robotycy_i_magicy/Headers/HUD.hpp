#ifndef HUD_HPP_INCLUDED
#define HUD_HPP_INCLUDED

#include "TextureManager.hpp"
#include "Scene.hpp"
#include <sstream>

class HUD : public sf::Drawable
{
	sf::Sprite coverSprite;
	sf::Text coverText;
	sf::Sprite decoySprite;
	sf::Text decoyText;
	
	sf::RectangleShape gameLostScreen;
	
	sf::Font font;
    
    sf::View hudView;
	
	
	int covers = 0;
	int decoys = 0;
	bool isGameLost = false;
	
	void updateText()
	{
		std::stringstream ss;
		ss << "x" << covers;
		coverText.setString(ss.str());
		ss.str("");
		ss << "x" << decoys;
		decoyText.setString(ss.str());
	}
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::View lastView = target.getView();
		target.setView(hudView);
		if(isGameLost)
		{
			target.draw(gameLostScreen, states);
		}
		else
		{
			if(covers > 0)
			{
				target.draw(coverSprite, states);
				target.draw(coverText, states);
			}
			if(decoys > 0)
			{
				target.draw(decoySprite, states);
				target.draw(decoyText, states);
			}
		}
		target.setView(lastView);
	}
	
public:
	HUD()
		: hudView({0,0,800,600})
	{
		font.loadFromFile("assets/cour.ttf");
		
		hudView.setViewport({0,0,1,1});
		hudView.setCenter(400, 300);
		
		coverSprite.setTexture(TextureManager::get("assets/textures.png"));
		coverSprite.setTextureRect({0, 160, 16, 16});
		coverSprite.setScale(2, 2);
		coverSprite.setPosition(20, 500);
		
		coverText.setFont(font);
		coverText.setCharacterSize(36);
		coverText.setString("x0");
		coverText.setPosition(60, 500);
		coverText.setFillColor(sf::Color::White);
		
		decoySprite.setTexture(TextureManager::get("assets/textures.png"));
		decoySprite.setTextureRect({0, 176, 14, 10});
		decoySprite.setScale(2, 2);
		decoySprite.setPosition(20, 550);
		
		decoyText.setFont(font);
		decoyText.setCharacterSize(36);
		decoyText.setString("x0");
		decoyText.setPosition(60, 550);
		decoyText.setFillColor(sf::Color::White);
		
		gameLostScreen.setTexture(&TextureManager::get("assets/gameLostScreen.bmp"));
		gameLostScreen.setSize({800, 600});
	}
	
	void updateValuesFromScene(Scene& scene)
	{
		if(scene.player)
		{
			updateValues(scene.player->coversToPlace, scene.player->decoysToPlace, scene.isGameLost);
		}
	}
	
	void updateValues(int covers_, int decoys_, bool isGameLost_)
	{
		covers = covers_;
		decoys = decoys_;
		updateText();
		
		isGameLost = isGameLost_;
	}
};

#endif // HUD_HPP_INCLUDED
