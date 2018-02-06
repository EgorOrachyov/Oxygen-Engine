/*
 *  Project: Oxygen Game Engine
 *  Created by Egor Orachyov on 02.02.2018.
 *  Copyright
 *
 *  xSoundSystem releases xEngine class
 *  for working with sound data. it allows
 *  programmer create device, context and play
 *  different sounds in the 3d space (by simple
 *  setting up of important values: listener position,
 *  velocity, orientation and etc.)
 */

#ifndef OXYGEN_XSOUNDSYSTEM_H
#define OXYGEN_XSOUNDSYSTEM_H

// ----------------------------------------------------------------------
// Engine's Sound System Class
// ----------------------------------------------------------------------

class xSoundSystem
{
public:

    // ----------------------------------------------------------------------
    // Class constructor and destructor
    // ----------------------------------------------------------------------
    xSoundSystem();
    ~xSoundSystem();

    // ----------------------------------------------------------------------
    // Creates and deletes sound sources
    // ----------------------------------------------------------------------
    xSoundSource * CreateSoundSource(char * name, char * path);
    void DeleteSoundSource(xSoundSource * source);

    // ----------------------------------------------------------------------
    // Functions for work with context
    // (You can create and set active different context to
    // be able to operate with all context sounds)
    // ----------------------------------------------------------------------
    ALCcontext * CreateContext();
    void DeleteContext(ALCcontext * context);
    void SetContextCurrent(ALCcontext * context);
    void DeleteAllContexts();

    // ----------------------------------------------------------------------
    // Listener set-up param functions
    // ----------------------------------------------------------------------
    void SetListenerPosition(xVector3 * position);
    void SetListenerPosition(float x, float y, float z);
    void SetListenerVelocity(xVector3 * velocity);
    void SetListenerVelocity(float x, float y, float z);
    void SetListenerOrientation(xVector3 * at, xVector3 * up);
    void SetListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z);

    // ----------------------------------------------------------------------
    // Global context functions
    // ----------------------------------------------------------------------
    void PlayAllSources();
    void StopAllSources();
    void PauseAllSources();
    void RewindAllSources();

    // ----------------------------------------------------------------------
    // Pause and Continue context sounds processing
    // ----------------------------------------------------------------------
    void SuspendContext();
    void ProcessContext();

    // ----------------------------------------------------------------------
    // Clear memory and delete all source
    // (should be called when you leave the state)
    // ----------------------------------------------------------------------
    void DeleteAllSources();

private:

    xVector3 * m_position;      // Listener position
    xVector3 * m_velocity;      // Listener velocity
    xVector3 * m_at;            // Listener direction forward
    xVector3 * m_up;            // Listener direction up

    ALCdevice * m_device;                           // Hardware audio device
    ALCcontext * m_context;                         // Current context
    xLinkedList<ALCcontext> * m_ContextList;        // List of contexts
    xResourceManager<xSound> * m_SoundManager;      // Manager for sound data
    xLinkedList<xSoundSource> * m_SoundSources;     // List of sound sources

};


#endif //OXYGEN_XSOUNDSYSTEM_H
