/*
* Using the input manager from Game Architecture last year
* Code by: James Keats
* http://www.jameskeats.com/blogs/post/Allegro-Input-Handling-Game-Architecture
*/

#include "InputManager.h"
#include <stdio.h>

InputManager::InputManager()
{
	mpKeyboardEventQueue = nullptr;
	mpMouseEventQueue = nullptr;
}


InputManager::~InputManager()
{
	cleanup();
}

void InputManager::init()
{
	if (!al_install_keyboard())
	{
		printf("Keyboard not installed!\n");
	}

	if (!al_install_mouse())
	{
		printf("Mouse not installed!\n");
	}

	mpKeyboardEventQueue = al_create_event_queue();
	mpMouseEventQueue = al_create_event_queue();

	al_register_event_source(mpKeyboardEventQueue, al_get_keyboard_event_source());

	al_register_event_source(mpMouseEventQueue, al_get_mouse_event_source());

	mLeftMouseState = 0x0;
	mRightMouseState = 0x0;
	for (int i = 0; i < static_cast<int>(KeyCode::MY_MAX_KEY); i++)
	{
		mBitwiseKeyStates[i] = 0x0;
	}
}

void InputManager::cleanup()
{
	if (mpKeyboardEventQueue != nullptr)
	{
		al_unregister_event_source(mpMouseEventQueue, al_get_mouse_event_source());
		al_uninstall_mouse();

		al_unregister_event_source(mpKeyboardEventQueue, al_get_keyboard_event_source());
		al_uninstall_keyboard();

		al_destroy_event_queue(mpMouseEventQueue);
		mpMouseEventQueue = nullptr;
		al_destroy_event_queue(mpKeyboardEventQueue);
		mpKeyboardEventQueue = nullptr;
	}
}

void InputManager::update()
{
	updateKeyboard();
	updateMouse();
}

void InputManager::updateKeyboard()
{
	//Clear single-frame flags
	for (int i = 0; i < static_cast<int>(KeyCode::MY_MAX_KEY); i++)
	{
		//bitwise "and" the bitwise not == "subtract" that bit
		mBitwiseKeyStates[i] &= (~StateBitValues::JUST_PRESSED & ~StateBitValues::JUST_RELEASED);
	}

	//Loop over all recorded key events this frame
	while (!al_event_queue_is_empty(mpKeyboardEventQueue))
	{
		al_get_next_event(mpKeyboardEventQueue, &mEvent);

		//Prevent access out of range--we're not using all of allegro's potential keys
		if (mEvent.keyboard.keycode >= static_cast<int>(KeyCode::MY_MAX_KEY))
			continue;

		if (mEvent.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			//If it was just pressed, set the two pressed flags for that key
			mBitwiseKeyStates[mEvent.keyboard.keycode] |= StateBitValues::JUST_PRESSED;
			mBitwiseKeyStates[mEvent.keyboard.keycode] |= StateBitValues::CURRENTLY_PRESSED;
		}
		else if (mEvent.type == ALLEGRO_EVENT_KEY_UP)
		{
			//If it was just released, clear the pressed flag and set the just released flag
			mBitwiseKeyStates[mEvent.keyboard.keycode] &= ~StateBitValues::CURRENTLY_PRESSED;
			mBitwiseKeyStates[mEvent.keyboard.keycode] |= StateBitValues::JUST_RELEASED;
		}
	}
}

void InputManager::updateMouse()
{
	//Clear single-frame flags
	mLeftMouseState &= ~StateBitValues::JUST_PRESSED;
	mLeftMouseState &= ~StateBitValues::JUST_RELEASED;
	mRightMouseState &= ~StateBitValues::JUST_PRESSED;
	mRightMouseState &= ~StateBitValues::JUST_RELEASED;

	//Loop through all recorded mouse events
	while (!al_event_queue_is_empty(mpMouseEventQueue))
	{
		al_get_next_event(mpMouseEventQueue, &mEvent);

		//Set the mouse position
		if (mEvent.type == ALLEGRO_EVENT_MOUSE_AXES || mEvent.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
		{
			mMouseX = mEvent.mouse.x;
			mMouseY = mEvent.mouse.y;
		}
		else
		{
			//Mouse clicks
			byte_t* whichButton = (mEvent.mouse.button == static_cast<int>(MouseCode::LEFT)) ? &mLeftMouseState : &mRightMouseState;
			if (mEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				//If it was pressed, set the two pressed flags
				*whichButton |= StateBitValues::JUST_PRESSED;
				*whichButton |= StateBitValues::CURRENTLY_PRESSED;
			}
			else if (mEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
			{
				//If it was released, clear the pressed flag and set the released flag
				*whichButton &= ~StateBitValues::CURRENTLY_PRESSED;
				*whichButton |= StateBitValues::JUST_RELEASED;
			}
		}
	}
}

bool InputManager::getPressed(KeyCode key) const
{
	return getHasByte(mBitwiseKeyStates[static_cast<int>(key)], StateBitValues::JUST_PRESSED);
}

bool InputManager::getDown(KeyCode key) const
{
	return getHasByte(mBitwiseKeyStates[static_cast<int>(key)], StateBitValues::CURRENTLY_PRESSED);
}

bool InputManager::getReleased(KeyCode key) const
{
	return getHasByte(mBitwiseKeyStates[static_cast<int>(key)], StateBitValues::JUST_RELEASED);
}

bool InputManager::getPressed(MouseCode button) const
{
	if (button == MouseCode::LEFT)
		return getHasByte(mLeftMouseState, StateBitValues::JUST_PRESSED);
	else
		return getHasByte(mRightMouseState, StateBitValues::JUST_PRESSED);
}

bool InputManager::getDown(MouseCode button) const
{
	if (button == MouseCode::LEFT)
		return getHasByte(mLeftMouseState, StateBitValues::CURRENTLY_PRESSED);
	else
		return getHasByte(mRightMouseState, StateBitValues::CURRENTLY_PRESSED);
}

bool InputManager::getReleased(MouseCode button) const
{
	if (button == MouseCode::LEFT)
		return getHasByte(mLeftMouseState, StateBitValues::JUST_RELEASED);
	else
		return getHasByte(mRightMouseState, StateBitValues::JUST_RELEASED);
}

bool InputManager::getHasByte(const byte_t value, const byte_t comparison) const
{
	//check if the given comparison bit is set in "value"
	//0101 & 0001 == 0001
	return (value & comparison);
}

int InputManager::getMouseX() const
{
	return mMouseX;
}

int InputManager::getMouseY() const
{
	return mMouseY;
}

void InputManager::getMousePos(int& x, int& y) const
{
	x = mMouseX;
	y = mMouseY;
}
