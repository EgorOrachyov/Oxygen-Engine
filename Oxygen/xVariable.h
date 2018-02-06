/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 01.02.2018.
 * Copyright
 *
 * xVariable class provides mechanism for
 * creating, loading and working with
 * variables' data, which is got from
 * script files (writen by OxyScript Language:
 * small realization for engine)
 */

#ifndef OXYGEN_XVARIABLE_H
#define OXYGEN_XVARIABLE_H

// ----------------------------------------------------------------------
// Important type defines for variables from the OxyScript
// ----------------------------------------------------------------------

#define VARIABLE_BOOL       0x00
#define VARIABLE_FLOAT      0x01
#define VARIABLE_NUMBER     0x02
#define VARIABLE_STRING     0x03
#define VARIABLE_UNKNOWN    0x04
#define VARIABLE_VEC2       0x05
#define VARIABLE_VEC3       0x06
#define VARIABLE_VEC4       0x07

// ----------------------------------------------------------------------
// Variable class
// ----------------------------------------------------------------------

class xVariable
{
public:

    // ----------------------------------------------------------------------
    // Class constructor (from name and script file)
    // ----------------------------------------------------------------------
    xVariable(char * type, FILE * file);

    // ----------------------------------------------------------------------
    // Class constructor (from name, type and value). Can be used if script
    // is created by the program i the run-time mode
    // ----------------------------------------------------------------------
    xVariable( char * name, char type, void * value );

    // ----------------------------------------------------------------------
    // Class destructor
    // ----------------------------------------------------------------------
    virtual ~xVariable();

    // ----------------------------------------------------------------------
    // Returns type of the variable
    // ----------------------------------------------------------------------
    char GetType();

    // ----------------------------------------------------------------------
    // Returns name of the variable
    // ----------------------------------------------------------------------
    char * GetName();

    // ----------------------------------------------------------------------
    // Returns data (value) of the variable
    // ----------------------------------------------------------------------
    void * GetData();

private:

    char m_type;                // Data type of the variable
    char * m_name;              // Name of the variable
    void * m_data;              // Data (value) of the variable

};




#endif //OXYGEN_XVARIABLE_H
