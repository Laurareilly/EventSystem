#pragma once
#include <RakNet/WindowsIncludes.h>
#include <allegro5/allegro.h>
#include "Trackable.h"
#include "Vector2D.h"
#include "Color.h"

class GraphicsBuffer;
class Sprite;
class InputSystem;
class Font;

class GraphicsSystem:public Trackable
{
public:
	GraphicsSystem();
	~GraphicsSystem();

	bool init( int width, int height );
	void cleanup();

	void swap();
	void wrapCoordinates( Vector2D& vector );//change the x and y values in the vector to keep them on the visible screen

	//accessors
	int getWidth() const { return mWidth; };
	int getHeight() const { return mHeight; };
	inline GraphicsBuffer* getBackBuffer() { return mpBackBuffer; };

	//draw the contents of the sprite to the Backbuffer - may be rotated
	void draw(const Sprite& aSprite, float dx, float dy, float rotationInRadians = 0, int flags = 0);
	//draw the contents of the sprite to a GraphicsBuffer - may be rotated
	void draw(GraphicsBuffer& dest, const Sprite& aSprite, float dx, float dy, float rotationInRadians = 0, int flags = 0);
	//set entire buffer to given color
	void clear(GraphicsBuffer& buffer, const Color& color = BLACK_COLOR);
	//set a rectangular region to a color
	void fillRegion(GraphicsBuffer& buffer, const Vector2D& ul, const Vector2D& lr, const Color& color);

	void drawText(GraphicsBuffer& dest, const Font& font, float dx, float dy, const Color& color, const std::string& theText);
	void drawText(const Font& font, float dx, float dy, const Color& color, const std::string& theText);

private:
	ALLEGRO_DISPLAY* mpDisplay;
	GraphicsBuffer* mpBackBuffer;
	int mWidth;
	int mHeight;

	friend class InputSystem;
	ALLEGRO_DISPLAY* getDisplay() const { return mpDisplay; };//InputSystem will need this to turn cursor on/off


	static ALLEGRO_BITMAP* switchTargetBitmap( ALLEGRO_BITMAP* pNewTarget );//return old target
};