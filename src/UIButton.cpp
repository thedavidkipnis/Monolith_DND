#include "UIButton.h"

UIButton::UIButton(SDL_Rect buttonArea, SDL_Texture* buttonTexture, SDL_Texture* buttonInactiveTexture, int buttonState, int buttonActionID)
	: buttonArea(buttonArea), buttonActiveTexture(buttonTexture), buttonInactiveTexture(buttonInactiveTexture), buttonState(buttonState), buttonActionID(buttonActionID)
{}

UIButton::~UIButton() {
	if (buttonActiveTexture) {
		SDL_DestroyTexture(buttonActiveTexture);
	}
	if (buttonInactiveTexture) {
		SDL_DestroyTexture(buttonInactiveTexture);
	}
}

void UIButton::setButtonTextures(SDL_Texture* activeTexture, SDL_Texture* inActiveTexture) {
	buttonActiveTexture = activeTexture;
	buttonInactiveTexture = inActiveTexture;
}

void UIButton::setButtonState(int state) {
	buttonState = state;
}

void UIButton::setButtonActionID(int actionID) {
	buttonActionID = actionID;
}