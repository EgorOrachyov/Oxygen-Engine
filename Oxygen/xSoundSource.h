/*
 *  Project: Oxygen Game Engine
 *  Created by Egor Orachyov on 02.02.2018.
 *  Copyright
 *
 *  xSoundSource releases virtual sound
 *  source of the game scene. It has own position,
 *  speed, gain and etc. You can create sound
 *  sources as much as you can, because of resource
 *  manager, which will prevent unwanted loading
 *  of duplicated data.
 */

#ifndef OXYGEN_XSOUNDSOURCE_H
#define OXYGEN_XSOUNDSOURCE_H

// ----------------------------------------------------------------------
// Sound Source class
// ----------------------------------------------------------------------

class xSoundSource {
public:

    // ----------------------------------------------------------------------
    // Creates Sound Source from Sound data and gets pointer to manager
    // to delete the sound data when you delete sound source
    // ----------------------------------------------------------------------
    xSoundSource(xSound * sound, xResourceManager<xSound> * manager);

    // ----------------------------------------------------------------------
    // Class Destructor
    // ----------------------------------------------------------------------
    ~xSoundSource();

    // ----------------------------------------------------------------------
    // Set-Up functions
    // ----------------------------------------------------------------------
    void SetGain(float gain);
    void SetMaxGain(float gain);
    void SetMinGain(float gain);
    void SetPitch(float pitch);
    void SetPosition(xVector3 * position);
    void SetPosition(float x, float y, float z);
    void SetVelocity(xVector3 * velocity);
    void SetVelocity(float x, float y, float z);
    void SetDirection(xVector3 * direction);
    void SetDirection(float x, float y, float z);
    void SetLoop(int is_looped = 0);
    void SetSound(xSound * sound);

    // ----------------------------------------------------------------------
    // Use Function
    // ----------------------------------------------------------------------
    void Play();
    void Stop();
    void Pause();
    void Rewind();

private:

    ALuint m_source;                            // Identification of the source
    xSound * m_sound;                           // Pointer to the sound source
    xResourceManager<xSound> * m_sound_manager; // Pointer to sound data manager
    xVector3 * m_position;                      // Vector of position
    xVector3 * m_velocity;                      // Vector of speed
    xVector3 * m_direction;                     // Vector of direction
    int m_is_looped;                            // Is sound looped (when play it)
    float m_gain;                               // The gain param (in model of distance)
    float m_pitch;
};


#endif //OXYGEN_XSOUNDSOURCE_H
