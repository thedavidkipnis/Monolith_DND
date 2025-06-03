#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Constants.h"

class UIButton {

private:
	SDL_Rect buttonArea;
	SDL_Texture* buttonActiveTexture;
	SDL_Texture* buttonInactiveTexture;

	int buttonState;
	int buttonActionID;

public:

	UIButton(SDL_Rect buttonArea, SDL_Texture* buttonActiveTexture, SDL_Texture* buttonInactiveTexture, int buttonState, int buttonActionID);
	~UIButton();

	const SDL_Rect& getButtonArea() const { return buttonArea; }

	SDL_Texture* getButtonActiveTexture() const { return buttonActiveTexture; }
	SDL_Texture* getButtonInactiveTexture() const { return buttonInactiveTexture; }
	void setButtonTextures(SDL_Texture* activeTexture, SDL_Texture* inActiveTexture);

	int getButtonState() const { return buttonState; }
	void setButtonState(int state);

	int getButtonActionID() const { return buttonActionID; }
	void setButtonActionID(int actionID);

};

