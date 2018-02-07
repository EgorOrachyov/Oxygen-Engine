/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 06.02.2018.
 * Copyright
 *
 * Realisation of functions defined in the file
 * xFont.h. Go there to find more information
 * and interface specifications
 */

#include "xEngine.h"

xFont::xFont(char * ttf, unsigned int FSize, unsigned int FDepth)
{
    m_color = new xVector4(1.0, 1.0, 1.0, 1.0);

    FILE * file = fopen(ttf, "r");
    if (file == NULL) {
        printf("ERROR: Cannot find/open Font %s \n", ttf);
        exit(1);
    } else {
        fclose(file);
    }

    m_Font = new FTGLBitmapFont(ttf);
    if ( m_Font == NULL) {
        printf("ERROR: Cannot create new Font %s \n", ttf);
        exit(1);
    }

    if (m_Font->FaceSize(FSize) == false) {
        printf("ERROR: Cannot set font size for %s \n", ttf);
        exit(1);
    }
    m_Font->Depth(FDepth);
    m_Font->CharMap(ft_encoding_unicode);
}

xFont::~xFont()
{
    SAFE_DELETE(m_Font);
    SAFE_DELETE(m_color);
}

void xFont::SetColor(float r, float g, float b, float a)
{
    m_color->x = r;
    m_color->y = g;
    m_color->z = b;
    m_color->w = a;
}

void xFont::Print(float x, float y, const wchar_t *text)
{
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(x,y,-1);

    glColor4f(m_color->x, m_color->y, m_color->z, m_color->w);
    glRasterPos2f(-1,0.5);
    m_Font->Render(text);

    glPopMatrix();
}