/*
 *  Project: Oxygen Game Engine
 *  Created by Egor Orachyov on 04.02.2018.
 *  Copyright
 *
 *  xDebugDrawManager allows you print all
 *  important information on screen using
 *  Draw Queue. Simply add a string with
 *  special properties in the queue and
 *  manager will print this info in window
 *  (Warning: there used orthographic
 *  coordination system with:
 *  left 0 -> width right x axis
 *  up 0 -> height down   y axis
 *  -1 and 1 z axis depth)
 */

#ifndef OXYGEN_XDEBUGDRAWMANAGER_H
#define OXYGEN_XDEBUGDRAWMANAGER_H

#include "xEngine.h"

// ----------------------------------------------------------------------
// Simple output text line with position
// ----------------------------------------------------------------------

struct xLine
{
public:

    wchar_t * m_text;
    float m_x, m_y;

    xLine(wchar_t * text, float x, float y) {
        m_x = x;
        m_y = y;
        m_text = text;
    }
};

// ----------------------------------------------------------------------
// Debug Drawing Manager
// ----------------------------------------------------------------------

class xDebugDrawManager
{
public:

    // ----------------------------------------------------------------------
    // Creates Manager from text size (standard font: Arial.ttf)
    // ----------------------------------------------------------------------
    xDebugDrawManager(unsigned int size)
    {
        char base_font[] = "Fonts/Arial.ttf";
        m_lines = new xLinkedList<xLine>;

        m_font = new xFont(base_font, size, 10);
        m_size = size;
        m_x = 20.0;
        m_y = 50.0;
        m_pass = 3 + (float)size;
        m_indentation = 20.0;
    }

    // ----------------------------------------------------------------------
    // Class Destructor
    // ----------------------------------------------------------------------
    ~xDebugDrawManager() 
    {
        SAFE_DELETE(m_font);
        SAFE_DELETE(m_lines);
    }

    // ----------------------------------------------------------------------
    // Sets start output position, pass between lines and right indentation
    // ----------------------------------------------------------------------
    void SetOutputProperties(float start_x, float start_y, float pass = 3.0, float indent = 20.0)
    {
        m_x = start_x;
        m_y = start_y;
        m_pass = pass + m_size;
        m_indentation = indent;
    }

    // ----------------------------------------------------------------------
    // Adds text line to draw queue
    // ----------------------------------------------------------------------
    void AddLine(wchar_t * text, bool indentation = false)
    {
        if (indentation) {
            m_lines->Add(new xLine(text, m_x + m_indentation, m_y));
        } else {
            m_lines->Add(new xLine(text, m_x, m_y));
        }

        m_y += m_pass;
    }

    // ----------------------------------------------------------------------
    // Adds text line to draw queue
    // ----------------------------------------------------------------------
    void AddLine(char * text, bool indentation = false)
    {
        wchar_t * txt = new wchar_t[STRING_SIZE];
        ConvertCharToWChar(txt, text);
        if (indentation) {
            m_lines->Add(new xLine(txt, m_x + m_indentation, m_y));
        } else {
            m_lines->Add(new xLine(txt, m_x, m_y));
        }

        m_y += m_pass;
    }

    // ----------------------------------------------------------------------
    // Adds text line to draw queue with random position
    // ----------------------------------------------------------------------
    void AddText(wchar_t * text, float x, float y)
    {
        m_lines->Add(new xLine(text, x, y));
    }

    // ----------------------------------------------------------------------
    // Clears queue (deletes all lines in buffer)
    // ----------------------------------------------------------------------
    void ClearQueue()
    {
        m_lines->Empty();
    }

    // ----------------------------------------------------------------------
    // Deletes last line in the buffer
    // ----------------------------------------------------------------------
    void DeleteLast()
    {
        m_lines->Remove(m_lines->GetLast());
    }

    // ----------------------------------------------------------------------
    // Should be called for each main loop cycle
    // ----------------------------------------------------------------------
    void Update()
    {
        m_lines->Iterate(true);
        while(m_lines->Iterate()) {
            xLine * line = m_lines->GetCurrent();
            m_font->Print(line->m_x, line->m_y, line->m_text);
        }
    }

    // ----------------------------------------------------------------------
    // From wchar_t* to char*
    // ----------------------------------------------------------------------
    void ConvertWCharToChar(char * des, wchar_t * res) {
        wcstombs(des, res, STRING_SIZE);
    }

    // ----------------------------------------------------------------------
    // From char* to wchar_t*
    // ----------------------------------------------------------------------
    void ConvertCharToWChar(wchar_t * des, char * res) {
        mbstowcs(des, res, STRING_SIZE);
    }

protected:

    xFont * m_font;                 // Font for printing text
    unsigned int m_size;            // Font size
    float m_x, m_y;                 // Printing position
    float m_indentation;            // Standard indentation for printing text in column
    float m_pass;                   // Distance between last and current line
    xLinkedList<xLine> * m_lines;   // List with lines of text
};

#endif //OXYGEN_XDEBUGDRAWMANAGER_H
