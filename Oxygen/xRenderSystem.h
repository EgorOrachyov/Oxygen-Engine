/*
 *  Project: Oxygen Game Engine
 *  Created by Egor Orachyov on 03.02.2018.
 *  Copyright
 *
 *  xRenderSystem releases main functionality
 *  for rendering 3d objects by using OpenGL
 *  in the co-operation with GLEW and GLFW for
 *  creations and working with windows and
 *  contexts
 */

#ifndef OXYGEN_XRENDERCORE_H
#define OXYGEN_XRENDERCORE_H

#include "xState.h"

// ----------------------------------------------------------------------
//
// ----------------------------------------------------------------------

class xRenderSystem
{
public:

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    xRenderSystem(GLFWwindow * window, xVirtualCamera * camera);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    ~xRenderSystem();

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    void UpdateSettings(xVirtualCamera * camera);

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    void ApplySettings();

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    void PrepareRendering3D();

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    void PrepareRendering2D();

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    void Rendering3D();

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    void Rendering2D();

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    void AddLightSource(xLight * light);

private:

    int m_width;                    //
    int m_height;                   //
    GLFWwindow * m_window;          //

    xVirtualCamera * m_camera;      //
    xLinkedList<xLight> * m_lights; //

    CLoadObj g_LoadObj;
    t3DModel g_3DModel;

    xModel3d model3d;
    xModelLoader modelLoader;
};


#endif //OXYGEN_XRENDERCORE_H
