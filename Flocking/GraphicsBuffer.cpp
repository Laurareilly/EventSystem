#include <RakNet/WindowsIncludes.h>

#include <allegro5/allegro_primitives.h>

#include "GraphicsBuffer.h"
#include "GraphicsSystem.h"
#include "Color.h"
#include "Game.h"
#include <Vector2D.h>
#include <assert.h>

using namespace std;

GraphicsBuffer::GraphicsBuffer()
	:mpBitmap(NULL)
{
}

GraphicsBuffer::GraphicsBuffer( const string& filename )
{
	mpBitmap = al_load_bitmap( filename.c_str() );
	assert(mpBitmap != NULL);
}

GraphicsBuffer::GraphicsBuffer( int width, int height, const Color& color )
{
	assert(width>0 && height>0);
	mpBitmap = al_create_bitmap( width, height );
	assert(mpBitmap != NULL);
	gpGame->getGraphicsSystem()->clear(*this, color);
}

GraphicsBuffer::GraphicsBuffer( ALLEGRO_BITMAP* pBitmap )
	:mpBitmap(pBitmap)
{
}

GraphicsBuffer::GraphicsBuffer( const GraphicsBuffer& rhs )
{
	mpBitmap = al_clone_bitmap( rhs.mpBitmap );
}

GraphicsBuffer& GraphicsBuffer::operator=( const GraphicsBuffer& rhs )
{
	if( &rhs == this )//self assignment
	{
		return *this;
	}

	//cleanup
	al_destroy_bitmap( mpBitmap );
	//clone bitmap
	mpBitmap = al_clone_bitmap( rhs.mpBitmap );

	return *this;
}

GraphicsBuffer::~GraphicsBuffer()
{
	al_destroy_bitmap( mpBitmap );
	//printf( " GraphicsBuffer destroyed!\n");
}
