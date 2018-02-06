/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 02.02.2018.
 * Copyright
 *
 * Realisation of functions defined in the file
 * xSoundSystem.h. Go there to find more information
 * and interface specifications
 */

#include "xEngine.h"

xSoundSystem::xSoundSystem()
{
    m_device = alcOpenDevice(NULL);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: Cannot open audio device \n");
        exit(1);
    }

    m_context = alcCreateContext(m_device, NULL);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: Cannot create audio context \n");
        exit(1);
    }

    m_ContextList = new xLinkedList<ALCcontext>;
    m_ContextList->Add(m_context);

    m_SoundSources = new xLinkedList<xSoundSource>;
    m_SoundManager = new xResourceManager<xSound>;

    alcMakeContextCurrent(m_context);
    alDistanceModel(AL_INVERSE_DISTANCE); // !!!

    const ALchar * version = alGetString(AL_VERSION);
    const ALchar * vendor = alGetString(AL_VENDOR);
    const ALchar * renderer = alGetString(AL_RENDERER);
    const ALchar * extensions = alGetString(AL_EXTENSIONS);

    printf("AL Vendor     : %s\n", vendor);
    printf("AL Version    : %s\n", version);
    printf("AL Renderer   : %s\n", renderer);
    printf("AL Extensions : %s\n", extensions);
    printf("\n");

    m_position = new xVector3;
    m_velocity = new xVector3;
    m_at = new xVector3;
    m_up = new xVector3;
}

xSoundSystem::~xSoundSystem()
{
    SAFE_DELETE(m_SoundSources);
    SAFE_DELETE(m_SoundManager);

    while(m_ContextList->Iterate()) {
        alcSuspendContext(m_ContextList->GetCurrent());
        alcDestroyContext(m_ContextList->GetCurrent());
    }

    //SAFE_DELETE(m_ContextList);
    alcCloseDevice(m_device);
}

xSoundSource * xSoundSystem::CreateSoundSource(char * name, char * path)
{
    xSound * sound = m_SoundManager->Add(name, path);
    xSoundSource * source = new xSoundSource(sound, m_SoundManager);
    m_SoundSources->Add(source);
    return source;
}

void xSoundSystem::DeleteSoundSource(xSoundSource * source)
{
    m_SoundSources->Remove(source);
}

ALCcontext * xSoundSystem::CreateContext()
{
    ALCcontext * context = alcCreateContext(m_device, NULL);
    m_ContextList->Add(context);
}

void xSoundSystem::DeleteContext(ALCcontext * context)
{
    alcDestroyContext(context);
    m_ContextList->Remove(context);
}

void xSoundSystem::SetContextCurrent(ALCcontext *context)
{
    alcMakeContextCurrent(context);
}

void xSoundSystem::DeleteAllContexts()
{
    while(m_ContextList->Iterate()) {
        alcSuspendContext(m_ContextList->GetCurrent());
        alcDestroyContext(m_ContextList->GetCurrent());
    }
}

void xSoundSystem::SetListenerPosition(xVector3 * position)
{
    memcpy(m_position, position, sizeof(xVector3));
    alListener3f(AL_POSITION, m_position->x, m_position->y, m_position->z);
}

void xSoundSystem::SetListenerPosition(float x, float y, float z)
{
    m_position->x = x;
    m_position->y = y;
    m_position->z = z;
    alListener3f(AL_POSITION, m_position->x, m_position->y, m_position->z);
}

void xSoundSystem::SetListenerVelocity(xVector3 * velocity)
{
    memcpy(m_velocity, velocity, sizeof(xVector3));
    alListener3f(AL_VELOCITY, m_velocity->x, m_velocity->y, m_velocity->z);
}

void xSoundSystem::SetListenerVelocity(float x, float y, float z)
{
    m_velocity->x = x;
    m_velocity->y = y;
    m_velocity->z = z;
    alListener3f(AL_VELOCITY, m_velocity->x, m_velocity->y, m_velocity->z);
}

void xSoundSystem::SetListenerOrientation(xVector3 * at, xVector3 * up)
{
    memcpy(m_at, at, sizeof(xVector3));
    memcpy(m_up, up, sizeof(xVector3));
    float tmp[6] = {m_at->x, m_at->y, m_at->z, m_up->x, m_up->y, m_up->z};
    alListenerfv(AL_ORIENTATION, tmp);
}

void xSoundSystem::SetListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z)
{
    m_at->x = at_x;
    m_at->y = at_y;
    m_at->z = at_z;

    m_up->x = up_x;
    m_up->y = up_y;
    m_up->z = up_z;

    float tmp[6] = {m_at->x, m_at->y, m_at->z, m_up->x, m_up->y, m_up->z};
    alListenerfv(AL_ORIENTATION, tmp);
}

void xSoundSystem::PlayAllSources()
{
    m_SoundSources->Iterate(true);
    while(m_SoundSources->Iterate()) {
        m_SoundSources->GetCurrent()->Play();
    }
}

void xSoundSystem::StopAllSources()
{
    m_SoundSources->Iterate(true);
    while(m_SoundSources->Iterate()) {
        m_SoundSources->GetCurrent()->Stop();
    }
}

void xSoundSystem::PauseAllSources()
{
    m_SoundSources->Iterate(true);
    while(m_SoundSources->Iterate()) {
        m_SoundSources->GetCurrent()->Pause();
    }
}

void xSoundSystem::RewindAllSources()
{
    m_SoundSources->Iterate(true);
    while(m_SoundSources->Iterate()) {
        m_SoundSources->GetCurrent()->Rewind();
    }
}

void xSoundSystem::SuspendContext()
{
    alcSuspendContext(m_context);
}

void xSoundSystem::ProcessContext()
{
    alcProcessContext(m_context);
}

void xSoundSystem::DeleteAllSources()
{
    m_SoundSources->Empty();
}

