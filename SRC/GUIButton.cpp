#include <string>
#include "GUIButton.h"

GUIButton::GUIButton(float x, float y, float width, float height) : mX(x), mY(y), mWidth(width), mHeight(height) {}

/** Destructor. */
GUIButton::~GUIButton()
{
}

void GUIButton::Draw() {
        glColor3f(0.5f, 0.5f, 0.5f); // Button color
        glBegin(GL_QUADS);
        glVertex2f(mX, mY);
        glVertex2f(mX + mWidth, mY);
        glVertex2f(mX + mWidth, mY + mHeight);
        glVertex2f(mX, mY + mHeight);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f); // Text color
        glRasterPos2f(mX + 0.5f * mWidth - 5.0f, mY + 0.5f * mHeight - 5.0f); // Adjust text position
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"Button");
    }