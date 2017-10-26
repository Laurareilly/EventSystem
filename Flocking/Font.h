#include <RakNet/WindowsIncludes.h>
#include <Trackable.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>

class GraphicsSystem;

class Font :public Trackable
{
public:
	Font(const std::string& filename, UINT size);
	~Font();
private:
	ALLEGRO_FONT* mpAllegroFont;

	friend class GraphicsSystem;
};