#include "Font.h"
#include <cassert>

using namespace std;

Font::Font(const string& filename, UINT size)
{
	//should be somewhere else!
	al_init_font_addon();
	if (!al_init_ttf_addon())
	{
		printf("ttf font addon not initted properly!\n");
	}

	mpAllegroFont = al_load_ttf_font(filename.c_str(), size, 0);
	assert(mpAllegroFont != NULL);
}

Font::~Font()
{
	al_destroy_font(mpAllegroFont);
}

