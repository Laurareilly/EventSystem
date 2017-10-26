#pragma once
#include <RakNet/WindowsIncludes.h>
#include "Trackable.h"
#include <allegro5/allegro.h>
#include <string>
#include "Color.h"

class Vector2D;
class GraphicsSystem;

class GraphicsBuffer:public Trackable
{
public:
	GraphicsBuffer();//default
	GraphicsBuffer( const std::string& filename );//load from a file
	GraphicsBuffer( int width, int height, const Color& color = BLACK_COLOR  );//create blank with given size

	//copy constructor
	GraphicsBuffer( const GraphicsBuffer& rhs );
	//assignment operator
	GraphicsBuffer& operator=( const GraphicsBuffer& rhs );

	//destructor
	~GraphicsBuffer();

	//set a rectangular region to a color
	void fillRegion( const Vector2D& ul, const Vector2D& lr, const Color& color );

	//accessors
	inline int getWidth() const { return al_get_bitmap_width(mpBitmap); };
	inline int getHeight() const { return al_get_bitmap_height(mpBitmap); };
	inline bool isValid() const { return mpBitmap != NULL; };
	inline void invalidate() { mpBitmap = NULL; };

private:
	ALLEGRO_BITMAP* mpBitmap;//never a subBitmap

	GraphicsBuffer(ALLEGRO_BITMAP* pBitmap);//create from allegro bitmap
	inline ALLEGRO_BITMAP* getBitmap() const { return mpBitmap; };

	friend class GraphicsSystem;
};