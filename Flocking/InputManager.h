/*
* Using the input manager from Game Architecture last year
* Code by: James Keats
* http://www.jameskeats.com/blogs/post/Allegro-Input-Handling-Game-Architecture
*/
#pragma once
#include <RakNet/WindowsIncludes.h>
#include <allegro5/allegro.h>

typedef unsigned char byte_t;

class InputManager
{
public:
	enum class KeyCode
	{
		A = 1,
		B = 2,
		C = 3,
		D = 4,
		E = 5,
		F = 6,
		G = 7,
		H = 8,
		I = 9,
		J = 10,
		K = 11,
		L = 12,
		M = 13,
		N = 14,
		O = 15,
		P = 16,
		Q = 17,
		R = 18,
		S = 19,
		T = 20,
		U = 21,
		V = 22,
		W = 23,
		X = 24,
		Y = 25,
		Z = 26,

		N0 = 27,
		N1 = 28,
		N2 = 29,
		N3 = 30,
		N4 = 31,
		N5 = 32,
		N6 = 33,
		N7 = 34,
		N8 = 35,
		N9 = 36,

		ESCAPE = 59,
		TILDE = 60,
		MINUS = 61,
		EQUALS = 62,
		BACKSPACE = 63,
		TAB = 64,
		OPENBRACE = 65,
		CLOSEBRACE = 66,
		ENTER = 67,
		SEMICOLON = 68,
		QUOTE = 69,
		BACKSLASH = 70,
		BACKSLASH2 = 71,
		COMMA = 72,
		FULLSTOP = 73,
		SLASH = 74,
		SPACE = 75,

		LEFT = 82,
		RIGHT = 83,
		UP = 84,
		DOWN = 85,
		MY_MAX_KEY = 86
	};

	enum class MouseCode
	{
		LEFT = 1,
		RIGHT = 2
	};

private:
	struct StateBitValues
	{
		static const byte_t JUST_PRESSED = 0x1;			//0001
		static const byte_t CURRENTLY_PRESSED = 0x2;	//0010
		static const byte_t JUST_RELEASED = 0x4;		//0100
	};
	byte_t mBitwiseKeyStates[static_cast<int>(KeyCode::MY_MAX_KEY)];
	byte_t mLeftMouseState, mRightMouseState;
	int mMouseX, mMouseY;


	ALLEGRO_EVENT_QUEUE* mpKeyboardEventQueue;
	ALLEGRO_EVENT_QUEUE* mpMouseEventQueue;
	ALLEGRO_EVENT mEvent;

	void updateKeyboard();
	void updateMouse();

	bool getHasByte(const byte_t value, const byte_t comparison) const;

public:
	InputManager();
	~InputManager();

	void init();
	void cleanup();

	void update();

	bool getPressed(KeyCode key) const;
	bool getDown(KeyCode key) const;
	bool getReleased(KeyCode key) const;

	bool getPressed(MouseCode button) const;
	bool getDown(MouseCode button) const;
	bool getReleased(MouseCode button) const;

	int getMouseX() const;
	int getMouseY() const;
	void getMousePos(int& x, int& y) const;
};

