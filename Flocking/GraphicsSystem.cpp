#include <RakNet/WindowsIncludes.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Sprite.h"
#include "Font.h"
#include "Color.h"

GraphicsSystem::GraphicsSystem()
	:mpDisplay(NULL)
	,mpBackBuffer(NULL)
	,mWidth(0)
	,mHeight(0)
{
}

GraphicsSystem::~GraphicsSystem()
{
	cleanup();
}

bool GraphicsSystem::init( int width, int height )
{
	mWidth = width;
	mHeight = height;
	mpDisplay = al_create_display(width, height);
	if(!mpDisplay) 
	{
		fprintf(stderr, "GraphicsSystem::failed to create display!\n");
		return false;
	}
		
	//get the backbuffer
	ALLEGRO_BITMAP* pBackBuffer = al_get_backbuffer( mpDisplay );

	//set the back buffer as the target bitmap
	al_set_target_bitmap( pBackBuffer );

	//create the GraphicsBuffer backbuffer
	mpBackBuffer = new GraphicsBuffer( pBackBuffer );

	//al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, al_map_rgb(255,255,255));

	return true;
}

void GraphicsSystem::cleanup()
{
	al_destroy_display( mpDisplay );
	mpDisplay = NULL;

	mpBackBuffer->invalidate();
	delete mpBackBuffer;
	mpBackBuffer = NULL;
}

void GraphicsSystem::swap()
{
	al_flip_display();
}

void GraphicsSystem::wrapCoordinates( Vector2D& vector )
{
	if( vector.getX() < 0.0f )
	{
		vector.setX( getWidth() - 1.0f );
	}
	else if( vector.getX() >= getWidth() )
	{
		vector.setX( 0.0f );
	}

	if( vector.getY() < 0.0f )
	{
		vector.setY( getHeight() - 1.0f );
	}
	else if( vector.getY() >= getHeight() )
	{
		vector.setY( 0.0f );
	}
}

//static function
ALLEGRO_BITMAP* GraphicsSystem::switchTargetBitmap( ALLEGRO_BITMAP* pNewTarget )
{
	//get old target bitmap
	ALLEGRO_BITMAP* pOldTarget = al_get_target_bitmap();
	//set dest to be the target
	al_set_target_bitmap( pNewTarget );

	return pOldTarget;
}

void GraphicsSystem::draw(const Sprite& aSprite, float dx, float dy, float rotationInRadians /*= 0*/, int flags /*= 0*/)
{
	draw( *getBackBuffer(), aSprite, dx, dy, rotationInRadians, flags);
}

void GraphicsSystem::draw(GraphicsBuffer& dest, const Sprite& aSprite, float dx, float dy, float rotationInRadians /*= 0*/, int flags /*= 0*/)
{
	//set new target
	ALLEGRO_BITMAP* pOldTarget = GraphicsSystem::switchTargetBitmap(dest.getBitmap());
	//draw
	if (rotationInRadians != 0.0f)//need to rotate
	{
		float centerX = aSprite.getWidth() / 2.0f;
		float centerY = aSprite.getHeight() / 2.0f;
		al_draw_rotated_bitmap(aSprite.getBuffer()->getBitmap(), centerX, centerY, dx, dy, rotationInRadians, flags);
	}
	else//no need to rotate
	{
		al_draw_bitmap(aSprite.getBuffer()->getBitmap(), dx, dy, flags);
	}
	//restore old target
	GraphicsSystem::switchTargetBitmap(pOldTarget);

}

void GraphicsSystem::clear(GraphicsBuffer& buffer, const Color& color)
{
	ALLEGRO_BITMAP* pOldTarget = switchTargetBitmap(buffer.getBitmap());
	ALLEGRO_COLOR allegroColor = al_map_rgba(color.getR(), color.getG(), color.getB(), color.getA());
	al_clear_to_color(allegroColor);
	switchTargetBitmap(pOldTarget);
}

void GraphicsSystem::fillRegion(GraphicsBuffer& buffer, const Vector2D& ul, const Vector2D& lr, const Color& color)
{
	ALLEGRO_BITMAP* pOldTarget = switchTargetBitmap(buffer.getBitmap());
	ALLEGRO_COLOR allegroColor = al_map_rgba(color.getR(), color.getG(), color.getB(), color.getA());
	al_draw_filled_rectangle(ul.getX(), ul.getY(), lr.getX(), lr.getY(), allegroColor);
	GraphicsSystem::switchTargetBitmap(pOldTarget);

}

void GraphicsSystem::drawText(GraphicsBuffer& dest, const Font& font, float dx, float dy, const Color& color, const std::string& theText)
{
	ALLEGRO_COLOR aColor = al_map_rgb(color.getR(), color.getG(), color.getB());
	al_draw_text(font.mpAllegroFont, aColor, dx, dy, ALLEGRO_ALIGN_CENTRE, theText.c_str());

}

void GraphicsSystem::drawText(const Font& font, float dx, float dy, const Color& color, const std::string& theText)
{
	drawText(*getBackBuffer(), font, dx, dy, color, theText);
}
