#ifndef ANIMATIONS_HPP_INCLUDED
#define ANIMATIONS_HPP_INCLUDED
#include "TextureManager.hpp"

enum AnimationState
{
	Stop,
	Once,
	Loop
};


struct AnimatedSpritePreset
{
	std::string 	textureName;
	sf::IntRect 	baseFrame;
	double 			length;
	unsigned int	framesCount;
	unsigned int	framesPerRow;
	
	bool compare(const AnimatedSpritePreset& a) const
	{
		return 	textureName == a.textureName &&
				length		== a.length		 &&
				framesCount	== a.framesCount &&
				baseFrame	== a.baseFrame	 &&
				framesPerRow== a.framesPerRow;
	}
	
	AnimatedSpritePreset(const std::string& textureName_, const sf::IntRect& baseFrame_, double length_=0, unsigned int framesCount_=1, unsigned int framesPerRow_=0)
		: textureName(textureName_), baseFrame(baseFrame_), length(length_), framesCount(framesCount_), framesPerRow(framesPerRow_)
	{}
};

const Vector2i playerSpriteDimensions(16, 16);

namespace AnimatedSpritePresets
{
	const AnimatedSpritePreset	MageWalk		= AnimatedSpritePreset("assets/textures.png", sf::IntRect(16, 45, 7, 21), 	0.5, 		4);
	const AnimatedSpritePreset	RobotWalk		= AnimatedSpritePreset("assets/textures.png", sf::IntRect(16, 80, 16, 15), 	0.2, 		2);
	const AnimatedSpritePreset	CoverBlink		= AnimatedSpritePreset("assets/textures.png", sf::IntRect(0, 160, 16, 16), 	0.4, 		4);
	const AnimatedSpritePreset	CoverFade		= AnimatedSpritePreset("assets/textures.png", sf::IntRect(0, 208, 16, 16), 	0.4, 		5);
	const AnimatedSpritePreset	DecoyBlink		= AnimatedSpritePreset("assets/textures.png", sf::IntRect(0, 176, 14, 10), 	0.5, 		2);
}

class AnimatedSprite : public sf::Sprite
{
	double 			frameCounter 	= 0;
	unsigned int 	frame			= 0;
	sf::IntRect		frameRect;
	
	AnimatedSpritePreset preset;
	
	inline double getFrameLength() const
	{
		return preset.length / preset.framesCount;
	}
	
	void updateTextureRect()
	{
		unsigned int yOffset = preset.framesPerRow != 0 ? frame/preset.framesPerRow : 0;
		unsigned int xOffset = preset.framesPerRow != 0 ? frame%preset.framesPerRow : frame;		
				
		frameRect = sf::IntRect( 	preset.baseFrame.left 	+ xOffset * preset.baseFrame.width 	+ (flipX ? preset.baseFrame.width : 0),
									preset.baseFrame.top 	+ yOffset * preset.baseFrame.height + (flipY ? preset.baseFrame.height: 0),
									preset.baseFrame.width	* (flipX ? -1 : 1),
									preset.baseFrame.height * (flipY ? -1 : 1));
									
		setTextureRect(frameRect);
	}
public:
	
	bool flipX = false;
	bool flipY = false;
	
	bool noResetWheOnceCompleted = false;
	
	AnimationState state;
	
	void reset()
	{
		frame 			= 0;
		frameCounter 	= getFrameLength();
		updateTextureRect();
	}
	
	void reset(const AnimationState& newState)
	{
		frame 			= 0;
		frameCounter 	= getFrameLength();
		state			= newState;
		updateTextureRect();
	}
	
	void setFrame(unsigned int frame_)
	{
		frame 			= frame_;
		frameCounter 	= getFrameLength();
		updateTextureRect();
	}
	void setFrame(unsigned int frame_, const AnimationState& newState)
	{
		frame 			= frame_;
		frameCounter 	= getFrameLength();
		state			= newState;
		updateTextureRect();
	}
	
	void nextFrame()
	{
		frame++;
		frameCounter = getFrameLength();
		if(frame >= preset.framesCount)
		{
			frame = 0;
			
			if(state == Once)
			{
				state = Stop;
				if(noResetWheOnceCompleted)
				{
					frame = preset.framesCount - 1;
				}
			}
		}
		updateTextureRect();
	}
	
	void updateFrame(double deltaTime)
	{
		if(state == Stop || preset.length < 0)
			return;
		
		frameCounter -= deltaTime;
		if(frameCounter < 0)
		{
			nextFrame();
		}
	}	
	
	AnimatedSprite(const AnimatedSpritePreset& preset_, AnimationState state_ = Loop)
		: sf::Sprite(TextureManager::get(preset_.textureName)), preset(preset_), state(state_)
	{
		updateTextureRect();
	}
	
	void setPreset(const AnimatedSpritePreset& preset_, bool noReset = false)
	{
		if(!preset.compare(preset_))
		{
			preset = preset_;
			setTexture(TextureManager::get(preset.textureName));
			if(!noReset)
				reset();
			updateTextureRect();
		}
	}
};



#endif // ANIMATIONS_HPP_INCLUDED
