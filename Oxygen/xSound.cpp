/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 02.02.2018.
 * Copyright
 *
 * Realisation of functions defined in the file
 * xSound.h. Go there to find more information
 * and interface specifications
 */

#include "xEngine.h"

xSound::xSound(char * name, char * path) : xResource(name, path)
{
    ALenum      format;
    ALsizei     size;
    ALvoid    * data;
    ALsizei     freq;

    alGenBuffers(1, &m_buffer);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: Cannot crate buffer for file \n");
    }

    alutLoadWAVFile(GetFilename(), &format, &data, &size, &freq);
    alBufferData(m_buffer, format, data, size, freq);
    alutUnloadWAV(format, data, size, freq);
}

xSound::~xSound()
{
    alDeleteBuffers(1, &m_buffer);
}

ALuint xSound::GetSoundBuffer()
{
    return m_buffer;
}