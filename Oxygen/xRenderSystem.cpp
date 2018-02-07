/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 03.02.2018.
 * Copyright
 *
 * Realisation of functions defined in the file
 * xRenderSystem.h. Go there to find more information
 * and interface specifications
 */

#include "xEngine.h"

#define MAX_LIGHT_COUNT 32

xRenderSystem::xRenderSystem(GLFWwindow * window, xVirtualCamera * camera)
{
    m_window = window;
    m_lights = new xLinkedList<xLight>;

    UpdateSettings(camera);

    glHint(GL_FOG_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);


    char path[] = "Models/foot.obj";
    g_LoadObj.ImportObj(&g_3DModel, path);         // Load our .Obj file into our model structure

    char path2[] = "Models/cube.obj";
    modelLoader.ImportObj(&model3d, path2);
}

xRenderSystem::~xRenderSystem()
{
}

void xRenderSystem::UpdateSettings(xVirtualCamera * camera)
{
    m_camera = camera;
    glfwGetFramebufferSize(m_window, &m_width, &m_height);
    if (m_height <= 0) {
        m_height = 1;
    }
    glViewport(0, 0, m_width, m_height);
}

void xRenderSystem::ApplySettings()
{

}

void xRenderSystem::PrepareRendering3D()
{
    m_camera->SetPerspective(&m_width, &m_height);
}

void xRenderSystem::PrepareRendering2D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0 , m_width, m_height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void xRenderSystem::Rendering3D()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    glColor3f(0.1,0.1,0.1);
    glEnable(GL_DEPTH_TEST);
    glTranslatef(0.0, -4.0, 0.0);
    glBegin(GL_QUADS);
    for(int i = -100; i < 100; i++) {
        for(int j = -100; j < 100; j++) {
            glVertex3i(i, 0.0, j);
            glVertex3i(i, 0.0, j+1);
            glVertex3i(i+1, 0.0, j+1);
            glVertex3i(i+1, 0.0, j);
        }
    }
    glEnd();
    glTranslatef(0.0, 4.0, 0.0);

    m_camera->UpdateFrustumPyramid();

    glEnable(GL_LIGHTING);                              // Turn on lighting
    glEnable(GL_LIGHT0);                                // Turn on a light with defaults set
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

    xLight * light = m_lights->GetFirst();
    light->SetLightIndex(GL_LIGHT0);
    light->ApplySettingToRenderer();

    float amb[] = {0.0,0.0,0.0};
    glLightModelfv(GL_AMBIENT, amb);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    static float rotation = 0;
    glRotatef(rotation, 1,1,1);
    rotation+= 1;

    float diff[] = {1,1,1,1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);

    char name[] = "bone.bmp";
    char path[] = "Models/";

    glColor3f(1.,1.,1.);
    model3d.Render();

    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHT0);                                // Turn on a light with defaults set
    glDisable(GL_LIGHTING);                              // Turn on lighting
    glDisable(GL_DEPTH_TEST);

    m_lights->Iterate(true);
    while (m_lights->Iterate()) {
        xLight * light = m_lights->GetCurrent();
        if (light->IsActive() && light->IsGlareEffected()) {
            m_camera->RenderGlareEffect(light);
        }
    }
}

void xRenderSystem::Rendering2D()
{

}

void xRenderSystem::AddLightSource(xLight * light)
{
    m_lights->Add(light);
}