/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 25.01.2018.
 * Copyright
 *
 * xEngine core class is main header file which links
 * all of other engine modules. It provides unique
 * entry point and it should be added to your other projects
 * created by using this software
 */

#ifndef OXYGEN_XENGINE_H
#define OXYGEN_XENGINE_H

// ----------------------------------------------------------------------
// System includes
// ----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// ----------------------------------------------------------------------
// Project specific includes
// ----------------------------------------------------------------------

#include <ode/ode.h>
#include <AL/alut.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>

// ----------------------------------------------------------------------
// Project specific macros
// ----------------------------------------------------------------------

#define STRING_SIZE 256
#define DEFAULT_X_SIZE 1000
#define DEFAULT_Y_SIZE 600

#define SAFE_DELETE(p) { if (p) { delete(p); (p) = NULL; } };
#define SAFE_DELETE_ARRAY(p) { if (p) {delete[] (p); (p) = NULL; } }

// ----------------------------------------------------------------------
// Engine includes
// ----------------------------------------------------------------------

#include "xVirtualObject.h"
#include "xLinkedList.h"
#include "xDynamicArray.h"
#include "xBaseGeometry.h"
#include "xResourceManager.h"
#include "xTexture.h"
#include "xVariable.h"
#include "xScript.h"
#include "xInput.h"
#include "xSound.h"
#include "xSoundSource.h"
#include "xSoundSystem.h"
#include "xLight.h"
#include "xVirtualCamera.h"
#include "xFreeCamera.h"
#include "xRenderSystem.h"
#include "xState.h"
#include "xStateManager.h"

// ----------------------------------------------------------------------
// Base Engine Setup structure
// ----------------------------------------------------------------------

struct xEngineSetup
{
    int argc;
    char ** argv;

    char name[STRING_SIZE];         // Application name
    int size_x, size_y;             // Window size
    bool full_screen;               // Is it in full screen mode

    void (* StateSetup)();          //
    xVirtualCamera * camera;        //

    // ----------------------------------------------------------------------
    // xEngineSetup constructor
    // ----------------------------------------------------------------------
    xEngineSetup(int argc, char ** argv)
    {
        /* Standard settings */
        this->argc = argc;
        this->argv = argv;
        size_x = DEFAULT_X_SIZE;
        size_y = DEFAULT_Y_SIZE;
        full_screen = false;
        StateSetup = NULL;
        strcpy(name, "Application");
    }

    // ----------------------------------------------------------------------
    // Sets Caption for the window of application
    // ----------------------------------------------------------------------
    void SetName(char * name)
    {
        if (name != NULL) {
            strcpy(this->name, name);
        } else {
            printf("WARNING: name has wrong format \n");
        }
    }

    // ----------------------------------------------------------------------
    // Turns on (or off) full screen mode
    // ----------------------------------------------------------------------
    void SetFullScreen(bool isFullScreen = true)
    {
        full_screen = isFullScreen;
    }

    // ----------------------------------------------------------------------
    // Sets size of the working space for the window
    // ----------------------------------------------------------------------
    void SetWindowSize(int x, int y)
    {
        size_x = x;
        size_y = y;
    }

    // ----------------------------------------------------------------------
    // Sets Setup Function for States and state manager
    // ----------------------------------------------------------------------
    void SetStateSetupFunction(void (*setup)())
    {
        if (setup != NULL) {
            StateSetup = setup;
        } else {
            printf("WARNING: StateSetup Function has wrong format \n");
        }
    }

    // ----------------------------------------------------------------------
    // Sets Virtual Camera
    // ----------------------------------------------------------------------
    void SetVirtualCamera(xVirtualCamera * v_camera)
    {
        if (v_camera != NULL) {
            camera = v_camera;
        } else {
            printf("ERROR: Virtual Camera has wrong format \n");
            exit(1);
        }
    }
};

// ----------------------------------------------------------------------
// Base Engine Class
// ----------------------------------------------------------------------

class xEngine
{
public:

    // ----------------------------------------------------------------------
    // Creates Engine Core (main part)
    // ----------------------------------------------------------------------
    xEngine(xEngineSetup * setup);

    // ----------------------------------------------------------------------
    // Deletes Engine
    // ----------------------------------------------------------------------
    virtual ~xEngine();

    // ----------------------------------------------------------------------
    // Main Loop for message processing
    // ----------------------------------------------------------------------
    void RunMainLoop();

    // ----------------------------------------------------------------------
    // Pass true to leave main loop of Engine
    // ----------------------------------------------------------------------
    void LeaveMainLoop(bool should_leave);

    // ----------------------------------------------------------------------
    // Returns Engine's Script Manager
    // ----------------------------------------------------------------------
    xStateManager * GetStateManager();

    // ----------------------------------------------------------------------
    // Returns Engine's Input Wrapper
    // ----------------------------------------------------------------------
    xInput * GetInput();

    // ----------------------------------------------------------------------
    // Returns Engine's Sound System
    // ----------------------------------------------------------------------
    xSoundSystem * GetSoundSystem();

    // ----------------------------------------------------------------------
    // Returns Engine's Script Manager
    // ----------------------------------------------------------------------
    xResourceManager<xScript> * GetScriptManager();

    // ----------------------------------------------------------------------
    // Returns Engine's Render System
    // ----------------------------------------------------------------------
    xRenderSystem * GetRenderSystem();

    // ----------------------------------------------------------------------
    // Returns Engine's Virtual Camera for 3d rendering
    // ----------------------------------------------------------------------
    xVirtualCamera * GetVirtualCamera();

private:

    GLFWwindow * m_window;              // Application's window's descriptorxs
    bool m_loaded;                      // Is Engine loaded (created and ready to be use)
    bool m_active;                      // Is Application Window active
    bool m_isDone;                      // Is main loop should be closed
    xVirtualCamera * m_camera;          //
    xEngineSetup * m_setup;             // Pointer to Setup structure
    xState * m_currentState;            // Current (active) state in manager
    xInput * m_input;                   //
    xSoundSystem * m_soundSystem;       //
    xResourceManager<xScript> * m_scriptManager; //
    xStateManager * m_stateManager;     //
    xRenderSystem * m_renderSystem;     //


};

// ----------------------------------------------------------------------
// External variables and pointers
// ----------------------------------------------------------------------

extern xEngine * g_engine;              // Global Engine pointer


#endif //OXYGEN_XENGINE_H
