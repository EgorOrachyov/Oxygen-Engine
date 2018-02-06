/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 25.01.2018.
 * Copyright
 *
 * Realisation of functions defined in the file
 * xEngine.h. Go there to find more information
 * and interface specifications
 */

#include "xEngine.h"

xEngine * g_engine = NULL;

xEngine::xEngine(xEngineSetup * setup)
{
    printf("\n");
    printf("INFO: Oxygen Engine (c) 2018 \n");
    printf("INFO: Start Initialization ... \n");
    printf("\n");

    if (setup == NULL)
    {
        printf("ERROR: Setup structure has wrong format \n");
        exit(1);
    }
    if (setup->camera == NULL)
    {
        printf("ERROR: Virtual Camera has wrong format \n");
        exit(1);
    }

    m_setup = setup;
    m_camera = setup->camera;
    m_loaded = true;
    m_active = true;

    // OpenGL, GLFW initialization
    int isInitialized = glfwInit();
    if (!isInitialized) {
        printf("ERROR: cannot initialize GLFW \n");
        glfwTerminate();
        exit(1);
    }

    // Create window, save its descriptor and make context current
    if (m_setup->full_screen) {
        printf("%i \n", m_setup->full_screen);
        m_window = glfwCreateWindow(setup->size_x, setup->size_y, setup->name, glfwGetPrimaryMonitor(), NULL);
    }else {
        m_window = glfwCreateWindow(setup->size_x, setup->size_y, setup->name, NULL, NULL);
    }
    glfwMakeContextCurrent(m_window);

    // Print info about renderer and OpenGL
    const GLubyte * renderer = glGetString(GL_RENDERER);
    const GLubyte * vendor = glGetString(GL_VENDOR);
    const GLubyte * version = glGetString(GL_VERSION);
    const GLubyte * glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GL Vendor    : %s\n", vendor);
    printf("GL Renderer  : %s\n", renderer);
    printf("GL Version   : %s\n", version);
    printf("GLSL Version : %s\n", glslVersion);
    printf("\n");

    srand(time(NULL));

    m_soundSystem = new xSoundSystem;                       printf("INFO: Initialized Sound System \n");
    m_renderSystem = new xRenderSystem(m_window, m_camera); printf("INFO: Initialized Render System \n");
    m_scriptManager = new xResourceManager<xScript>;        printf("INFO: Initialized Script Manager \n");
    m_stateManager = new xStateManager;                     printf("INFO: Initialized State Manager \n");
    m_input = new xInput(m_window);                         printf("INFO: Initialized Input Manager \n");
    FreeImage_Initialise();                                 printf("INFO: Initialized FreeImage loading System \n");
    m_camera->Load();
    m_camera->Init();
    g_engine = this;

    if (setup->StateSetup != NULL) {
        setup->StateSetup();
    }

    printf("INFO: Initialized Engine Core \n");
}

xEngine::~xEngine()
{
    if (m_loaded)
    {
        /* All important deletes of components*/

        SAFE_DELETE(m_stateManager);
        printf("INFO: State Manager has been deleted \n");

        SAFE_DELETE(m_input);
        printf("INFO: Input Wrapper has been deleted \n");

        SAFE_DELETE(m_scriptManager)
        printf("INFO: Script Manager has been deleted \n");

        SAFE_DELETE(m_soundSystem);
        printf("INFO: Sound System has been deleted \n");

        SAFE_DELETE(m_renderSystem);
        printf("INFO: Render System has been deleted \n");

        glfwTerminate();
        printf("INFO: GLFW has been de-initialized \n");

        FreeImage_DeInitialise();
        printf("INFO: FreeImage loading System has been de-initialized \n");
    }

    SAFE_DELETE(m_setup);
    printf("INFO: Setup Structure has been deleted \n");

    m_camera->Close();
    SAFE_DELETE(m_camera);
    printf("INFO: Virtual Camera has been deleted \n");

    printf("INFO: Engine has been deleted \n");
}

void xEngine::RunMainLoop()
{
    m_isDone = false;

    double startTime = glfwGetTime();
    double lastTime = glfwGetTime();
    double currentTime = 0.0;
    double ellapsedTime = 0.0;

    // ----------------------------------------------------------------------
    // Order of Main loop operations
    // ----------------------------------------------------------------------
    //
    // Get elapsed time
    // Update state
    //   process input
    //   process sound
    // Render State
    // Input Update
    // Poll events
    //

    while (!m_isDone)
    {
        if (m_active)
        {
            /* Some code */

            // Get elapsed time for our cycle
            currentTime = glfwGetTime();
            ellapsedTime = (currentTime - lastTime);
            lastTime = currentTime;
            // Elapsed - time, which is used to count previous loop cycle

            // Should we close window (if user press red button on window)
            m_isDone = (bool)glfwWindowShouldClose(m_window);

            // Sets all settings to default for this iteration
            // (Flag is_State_Changed to false)
            m_stateManager->SetToDefault();

            // Asks for current state (if it exists)
            m_currentState = m_stateManager->GetCurrentState();

            // Update state and set up viewer
            if (m_currentState != NULL) {
                m_currentState->Update(ellapsedTime);
                m_currentState->RequestViewer(m_camera);
            }
            // Update Camera before main rendering
            m_camera->SetElapsedTime(ellapsedTime);
            m_camera->Update();

            // Update and apply settings for render system
            m_renderSystem->UpdateSettings(m_camera);
            m_renderSystem->ApplySettings();

            // Separately do 3d rendering
            m_renderSystem->PrepareRendering3D();
            m_renderSystem->Rendering3D();
            m_renderSystem->FinishRendering3D();

            // Separately do 2d rendering
            m_renderSystem->PrepareRendering2D();
            m_renderSystem->Rendering2D();
            m_renderSystem->FinishRendering2D();

            // Continue loop or render scene, if current
            // state was not changed
            if (m_stateManager->IsStateChanged()) {
                continue;
            } else if (m_currentState != NULL) {
                m_currentState->Render();
            }
            // Update input (to default) before next process polling
            m_input->Update();

            // For some time
            glfwSwapBuffers(m_window);

            // Update window system
            glfwPollEvents();
        }
    }

    printf("\nINFO: working time (total): %lf \n", glfwGetTime() - startTime);

    SAFE_DELETE(g_engine);
}

void xEngine::LeaveMainLoop(bool should_leave)
{
    m_isDone = should_leave;
}

xStateManager * xEngine::GetStateManager()
{
    return m_stateManager;
}

xInput * xEngine::GetInput()
{
    return m_input;
}

xSoundSystem* xEngine::GetSoundSystem()
{
    return m_soundSystem;
}

xResourceManager<xScript> * xEngine::GetScriptManager()
{
    return m_scriptManager;
}

xRenderSystem* xEngine::GetRenderSystem()
{
    return  m_renderSystem;
}

xVirtualCamera* xEngine::GetVirtualCamera()
{
    return m_camera;
}