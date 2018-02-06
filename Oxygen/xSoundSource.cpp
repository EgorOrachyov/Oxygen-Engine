/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 02.02.2018.
 * Copyright
 *
 * Realisation of functions defined in the file
 * xSoundSource.h. Go there to find more information
 * and interface specifications
 */

#include "xEngine.h"

xSoundSource::xSoundSource(xSound * sound, xResourceManager<xSound> * manager)
{
    if (sound == NULL) {
        printf("ERROR: Sound Data has wrong format \n");
        exit(1);
    }

    if (manager == NULL) {
        printf("ERROR: Manager has wrong format \n");
        exit(1);
    }

    alGenSources(1, &m_source);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: Cannot create sound source \n");
        //exit(1);
    }

    m_position = new xVector3;
    m_velocity = new xVector3;
    m_direction = new xVector3;

    m_sound = sound;
    m_sound_manager = manager;

    alSourcei(m_source, AL_BUFFER, m_sound->GetSoundBuffer());
}

xSoundSource::~xSoundSource()
{
    SAFE_DELETE(m_position);
    SAFE_DELETE(m_velocity);
    SAFE_DELETE(m_direction);

    m_sound_manager->Remove(m_sound);
    alDeleteSources(1, &m_source);
}

void xSoundSource::SetGain(float gain)
{
    m_gain = gain;
    alSourcef(m_source, AL_GAIN, m_gain);
}

void xSoundSource::SetMaxGain(float gain)
{
    alSourcef(m_source, AL_MAX_GAIN, gain);
}

void xSoundSource::SetMinGain(float gain)
{
    alSourcef(m_source, AL_MIN_GAIN, gain);
}

void xSoundSource::SetPitch(float pitch)
{
    m_pitch = pitch;
    alSourcef(m_source, AL_PITCH, m_pitch);
}

void xSoundSource::SetPosition(xVector3 * position)
{
    memcpy(m_position, position, sizeof(xVector3));
    alSource3f(m_source, AL_POSITION, m_position->x, m_position->y, m_position->z);
}

void xSoundSource::SetPosition(float x, float y, float z)
{
    m_position->x = x;
    m_position->y = y;
    m_position->z = z;
    alSource3f(m_source, AL_POSITION, m_position->x, m_position->y, m_position->z);
}

void xSoundSource::SetVelocity(xVector3 * velocity)
{
    memcpy(m_velocity, velocity, sizeof(xVector3));
    alSource3f(m_source, AL_VELOCITY, m_velocity->x, m_velocity->y, m_velocity->z);
}

void xSoundSource::SetVelocity(float x, float y, float z)
{
    m_velocity->x = x;
    m_velocity->y = y;
    m_velocity->z = z;
    alSource3f(m_source, AL_VELOCITY, m_velocity->x, m_velocity->y, m_velocity->z);
}

void xSoundSource::SetDirection(xVector3 * direction)
{
    memcpy(m_direction, direction, sizeof(xVector3));
    alSource3f(m_source, AL_DIRECTION, m_direction->x, m_direction->y, m_direction->z);
}

void xSoundSource::SetDirection(float x, float y, float z)
{
    m_direction->x = x;
    m_direction->y = y;
    m_direction->z = z;
    alSource3f(m_source, AL_DIRECTION, m_direction->x, m_direction->y, m_direction->z);
}

void xSoundSource::SetLoop(int is_looped)
{
    m_is_looped = is_looped;
    alSourcei(m_source, AL_LOOPING, m_is_looped);
}

void xSoundSource::SetSound(xSound * sound)
{
    Stop();
    m_sound_manager->Remove(m_sound);
    m_sound = sound;
    alSourcei(m_source, AL_BUFFER, m_sound->GetSoundBuffer());
}

void xSoundSource::Play()
{
    alSourcePlay(m_source);
}

void xSoundSource::Stop()
{
    alSourceStop(m_source);
}

void xSoundSource::Pause()
{
    alSourcePause(m_source);
}

void xSoundSource::Rewind()
{
    alSourceRewind(m_source);
}