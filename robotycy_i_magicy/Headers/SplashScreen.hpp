#ifndef SPLASHSCREEN_HPP_INCLUDED
#define SPLASHSCREEN_HPP_INCLUDED

#include "TextureManager.hpp"
#include <sstream>
#include <map>

static std::string getSpalshScreenName(const std::string& name)
{
	std::stringstream ss;
	ss << "assets/splashScreens/";
	ss << name;
	ss << ".png";
	return ss.str();
}

class SplashScreen : public sf::Drawable
{
	sf::RectangleShape sprite;
	SplashScreen* nextSplashScreen = nullptr;
	
	static std::map<std::string, SplashScreen> list;
	
	static void addToList(const std::string& name, const std::string& next)
	{
		list[name] = SplashScreen(name, &list.at(next));
	}
	
	static void addSequence(const std::vector<std::string>& names)
	{
		list[names[names.size() - 1]] = SplashScreen(names[names.size() - 1], nullptr);
		for(int i=names.size() - 1; i > 0; i--)
		{
			addToList(names[i-1], names[i]);
		}
	}
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
	}	
public:
	SplashScreen()
	{
	}
	
	SplashScreen(const std::string& name, SplashScreen* nextSplashScreen_ = nullptr)
		: nextSplashScreen(nextSplashScreen_)
	{
		sprite.setTexture(&TextureManager::get(getSpalshScreenName(name)));
		sprite.setSize({800, 600});
	}
	
	SplashScreen* getNext()
	{
		return nextSplashScreen;
	}
	
	static SplashScreen* get(const std::string& name)
	{
		if(list.count(name) <= 0)
		{
			std::cout << "NOT FOUND SPLASH" << std::endl;
			return nullptr;
		}
		return &list.at(name);
	}
	
	static void load()
	{
		if(list.size() > 0)
		{
			return;
		}
		
		addSequence({"story", "story2"});
		addSequence({"controlsBasics", "controlsBasics2"});
		
	}
};

std::map<std::string, SplashScreen> SplashScreen::list;

#endif // SPLASHSCREEN_HPP_INCLUDED
