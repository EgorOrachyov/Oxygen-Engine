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



class xSoundSystem
{
public:
    xSoundSystem();
    ~xSoundSystem();

    xSoundSource * CreateSoundSource(char * name, char * path);
    void DeleteSoundSource(xSoundSource * source);

    ALCcontext * CreateContext();
    void DeleteContext(ALCcontext * context);
    void SetContextCurrent(ALCcontext * context);

    void SetListenerPosition(xVector3 * position);
    void SetListenerPosition(float x, float y, float z);
    void SetListenerVelocity(xVector3 * velocity);
    void SetListenerVelocity(float x, float y, float z);
    void SetListenerOrientation(xVector3 * at, xVector3 * up);
    void SetListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z);

    void PlayAllSources();
    void StopAllSources();
    void PauseAllSources();
    void RewindAllSources();

    void SuspendContext();
    void ProcessContext();

    void DeleteAllSources();

private:

    xVector3 * m_position;
    xVector3 * m_velocity;
    xVector3 * m_at;
    xVector3 * m_up;

    ALCdevice * m_device;
    ALCcontext * m_context;
    xLinkedList<ALCcontext> * m_ContextList;
    xResourceManager<xSound> * m_SoundManager;
    xLinkedList<xSoundSource> * m_SoundSources;

};


#endif //OXYGEN_XSOUNDSYSTEM_H
