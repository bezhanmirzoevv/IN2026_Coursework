#ifndef __GUILABEL_H__
#define __GUILABEL_H__

#include "GameUtil.h"
#include "GUIComponent.h"

class GUILabel : public GUIComponent
{
public:
	GUILabel();
	GUILabel(const string& t);
	GUILabel(const string& t, int w, int h);
	virtual ~GUILabel();
	virtual void Draw();
	void SetText(const string& text) { mText = text; }
protected:
	string mText;
	int mFontWidth;
	int mFontHeight;
};

#endif