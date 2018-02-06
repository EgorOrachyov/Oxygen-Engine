/*
 *  Project: Oxygen Game Engine
 *  Created by Egor Orachyov on 06.02.2018.
 *  Copyright
 *
 *  xFont releases class for simple
 *  work with fonts on your OpenGL
 *  rendering window
 */

#ifndef OXYGEN_XFONT_H
#define OXYGEN_XFONT_H

#include <FTGL/ftgl.h>

// ----------------------------------------------------------------------
// Base Font Class
// ----------------------------------------------------------------------

class xFont
{
public:

    // ----------------------------------------------------------------------
    // Creates font from path to font, wanted size and depth
    // ----------------------------------------------------------------------
    xFont(char *ttf, unsigned int FSize, unsigned int FDepth);

    // ----------------------------------------------------------------------
    // Class Destructor
    // ----------------------------------------------------------------------
    ~xFont();

    // ----------------------------------------------------------------------
    // Sets font color
    // ----------------------------------------------------------------------
    void SetColor(float r, float g, float b, float a);

    // ----------------------------------------------------------------------
    // Print text int x y screen position
    // ----------------------------------------------------------------------
    void Print(float x, float y, const wchar_t *text);

protected:

    FTFont * m_Font;      // Font object
    xVector4 * m_color;   // Font color
};


#endif //OXYGEN_XFONT_H
