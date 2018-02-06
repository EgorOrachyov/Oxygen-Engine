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

void xRenderSystem::FinishRendering3D()
{

}

void xRenderSystem::FinishRendering2D()
{

}

void xRenderSystem::Rendering3D()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

/*
    glEnable(GL_DEPTH_TEST);

    glColor3f(0.99, 0.99, 0.99);
    static float angle1 = 0.0;
    static float angle2 = 0.0;

    glPushMatrix();
    glRotatef(angle1, 0.0, 1.0, 0.0);
    glRotatef(angle2, 0.0, 0.0, 1.0);

    angle1 += 0.1;
    angle2 += 0.2;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBegin(GL_TRIANGLES);

    glVertex3f(0.0, 0.0, 1.0);
    glVertex3f(-0.5, 0.0, -0.5);
    glVertex3f(0.5, 0.0, -0.5);

    glVertex3f(0.0, 0.0, 1.0);
    glVertex3f(0.5, 0.0, -0.5);
    glVertex3f(0.0, 1.0, 0.0);

    glVertex3f(-0.5, 0.0, -0.5);
    glVertex3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 1.0, 0.0);

    glVertex3f(0.5, 0.0, -0.5);
    glVertex3f(-0.5, 0.0, -0.5);
    glVertex3f(0.0, 1.0, 0.0);

    glEnd();

    glPopMatrix();
    //glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0);

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

    glDisable(GL_DEPTH_TEST);

*/
    m_camera->UpdateFrustumPyramid();
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