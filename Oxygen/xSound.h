/*
 *  Project: Oxygen Game Engine
 *  Created by Egor Orachyov on 02.02.2018.
 *  Copyright
 *
 *  xSound releases sound data for
 *  Engine Sound System
 */

#ifndef OXYGEN_XSOUND_H
#define OXYGEN_XSOUND_H

// ----------------------------------------------------------------------
// Sound (data) class
// ----------------------------------------------------------------------

class xSound : public xResource
{
public:

    // ----------------------------------------------------------------------
    // Creates sound data from file name
    // ----------------------------------------------------------------------
    xSound(char * name, char * path = NULL);

    // ----------------------------------------------------------------------
    // Free memory for sound data
    // ----------------------------------------------------------------------
    ~xSound();

    // ----------------------------------------------------------------------
    // Returns buffer index
    // ----------------------------------------------------------------------
    ALuint GetSoundBuffer();

private:

    ALuint m_buffer;                // Buffer for sound data

};


#endif //OXYGEN_XSOUND_H
