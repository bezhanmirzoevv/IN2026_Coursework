#ifndef __GUIButton_H__
#define __GUIButton_H__

#include "GameUtil.h"
#include "GUIComponent.h"

class GUIButton : public GUIComponent
{
public:
	GUIButton(float x, float y, float width, float height);
	virtual ~GUIButton();
	virtual void Draw();
protected:
	float mX, mY; // Position of the button
	float mWidth, mHeight; // Dimensions of the button
};

#endif