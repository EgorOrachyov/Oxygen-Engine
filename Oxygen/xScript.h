/* 
 *  Project: Oxygen Game Engine
 *  Created by Egor Orachyov on 01.02.2018.
 *  Copyright
 * 
 *  xScript provides functionality for
 *  working with loaded script file in the
 *  RAM in the rile-time mode
 */

#ifndef OXYGEN_XSCRIPT_H
#define OXYGEN_XSCRIPT_H

#include "xVariable.h"

// ----------------------------------------------------------------------
//
// ----------------------------------------------------------------------

class xScript : public xResource
{
public:

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    xScript(char * name, char * path = NULL);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual ~xScript();

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    void AddVariable(char * name, char type, void * value);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    void SetVariable(char * name, void * value);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    void SaveScript(char * filename = NULL);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    bool * GetBoolData(char * variable);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    float * GetFloatData(char * variable);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    long * GetNumberData(char * variable);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    char * GetStringData(char * variable);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    xVector2 * GetVec2Data(char *  variable);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    xVector3 * GetVec3Data(char * variable);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    xVector4 * GetVec4Data(char * variable);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    void *  GetUnknownData(char * variable);

private:

    xLinkedList<xVariable> * m_variables;   // Linked List of variables
};


#endif //OXYGEN_XSCRIPT_H
