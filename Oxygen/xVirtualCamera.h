/*
 *  Project: Oxygen Game Engine
 *  Created by Egor Orachyov on 04.02.2018.
 *  Copyright
 *
 *  xVirtualCamera release base class for
 *  implementation of different types of
 *  game camera. It has simple and useful
 *  methods for control such as move, rotate,
 *  set position, set orientation and etc.
 *
 *  Included functions:
 *  1. Frustum Culling
 *  2. Occlusion Culling
 *  3. Glare Diaphragm Effect
 */

#ifndef OXYGEN_XVIRTUALCAMERA_H
#define OXYGEN_XVIRTUALCAMERA_H

#include "xBaseGeometry.h"

// ----------------------------------------------------------------------
// Virtual Camera class
// ----------------------------------------------------------------------

class xVirtualCamera : public xVirtualObject
{
public:

    friend class xEngine;

    // ----------------------------------------------------------------------
    // Creates camera from frustum view angle, front and back culling planes
    // ----------------------------------------------------------------------
    xVirtualCamera(double angle, double front, double back) : xVirtualObject()
    {
        m_width = 1;
        m_height = 1;
        m_aspect = 1.0;
        m_angle = angle;
        m_front = front;
        m_back = back;
        m_yaw = 0.0;
        m_pitch = 0.0;
        m_elapsed = 0.0;
        m_position = new xVector3;
        m_direction = new xVector3;
    }

    // ----------------------------------------------------------------------
    // Class Destructor
    // ----------------------------------------------------------------------
    ~xVirtualCamera()
    {
        SAFE_DELETE(m_position);
        SAFE_DELETE(m_direction);

        SAFE_DELETE(m_StreaksTexture);
        SAFE_DELETE(m_GlowTexture);
        SAFE_DELETE(m_BigGlowTexture);
        SAFE_DELETE(m_HaloTexture);
    }

    // ----------------------------------------------------------------------
    // Only for Rendering System
    // ----------------------------------------------------------------------
    void Init()
    {
        char name[STRING_SIZE];
        char path[STRING_SIZE];

        strcpy(path, "GlareEffect/");

        strcpy(name, "streaks.bmp");
        m_StreaksTexture = new xTexture(name, path);

        strcpy(name, "glow.bmp");
        m_GlowTexture = new xTexture(name, path);

        strcpy(name, "big_glow.bmp");
        m_BigGlowTexture = new xTexture(name, path);

        strcpy(name, "halo.bmp");
        m_HaloTexture = new xTexture(name, path);
    }

    // ----------------------------------------------------------------------
    // Only for Rendering System
    // ----------------------------------------------------------------------
    virtual void SetPerspective(GLint * width, GLint * height)
    {
        m_width = *width;
        m_height = *height;

        // Set viewport properties
        glViewport(0, 0, *width, *height);
        m_aspect = (GLdouble)(*width) / (*height);

        // Set projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(m_angle, m_aspect, m_front, m_back);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Count direction vector by yaw and pitch rotations
        GLfloat Matrix[16];

        glRotatef(m_yaw, 0.0, 1.0, 0.0);
        glRotatef(m_pitch, 1.0, 0.0, 0.0);

        glGetFloatv(GL_MODELVIEW_MATRIX, Matrix);

        m_direction->x = Matrix[8];
        m_direction->z = -Matrix[10];

        glLoadIdentity();

        glRotatef(m_pitch, 1.0, 0.0, 0.0);

        glGetFloatv(GL_MODELVIEW_MATRIX, Matrix);
        m_direction->y = Matrix[9];

        glRotatef(m_yaw, 0.0, 1.0, 0.0);

        // Translate our camera to new position (remember to do it inverse)
        glTranslatef(-m_position->x, -m_position->y, -m_position->z);
    }

    // ----------------------------------------------------------------------
    // Set Virtual position
    // ----------------------------------------------------------------------
    void SetViewportAngle(float angle)
    {
        if (angle > 0.0 && angle < 180.0) {
            m_angle = angle;
        }
    }

    // ----------------------------------------------------------------------
    // Set Virtual position
    // ----------------------------------------------------------------------
    virtual void SetPosition(float x, float y, float z)
    {
        m_position->x = x;
        m_position->y = y;
        m_position->z = z;
    }

    // ----------------------------------------------------------------------
    // Set angle for oY rotation
    // ----------------------------------------------------------------------
    virtual void SetYaw(float y_angle)
    {
        m_yaw = y_angle;
    }

    // ----------------------------------------------------------------------
    // Set angle for oX rotation
    // ----------------------------------------------------------------------
    virtual void SetPitch(float x_angle)
    {
        m_pitch = x_angle;
    }

    // ----------------------------------------------------------------------
    // Set elapsed time after last loop
    // ----------------------------------------------------------------------
    virtual void SetElapsedTime(double elapsed)
    {
        m_elapsed = elapsed;
    }

    // ----------------------------------------------------------------------
    // Add oY angle rotation
    // ----------------------------------------------------------------------
    virtual void AddYaw(float y_angle)
    {
        m_yaw += y_angle;
    }

    // ----------------------------------------------------------------------
    // Add oX angle rotation
    // ----------------------------------------------------------------------
    virtual void AddPitch(float x_angle)
    {
        m_pitch += x_angle;
    }

    // ----------------------------------------------------------------------
    // Add vector to current position
    // ----------------------------------------------------------------------
    virtual void AddPosition(float x, float y, float z)
    {
        m_position->x += x;
        m_position->y += y;
        m_position->z += z;
    }

    // ----------------------------------------------------------------------
    // Add value to current view port angle (remember: 0.0 < angle < 180.0)
    // ----------------------------------------------------------------------
    virtual void AddViewportAngle(float angle)
    {
        if ((GLdouble)(m_angle + angle) < 180.0 && (GLdouble)(m_angle + angle) > 0.0) {
            m_angle += (GLdouble)angle;
        }
    }

    // ----------------------------------------------------------------------
    // Returns viewport size
    // ----------------------------------------------------------------------
    virtual void GetViewportSize(int * width, int * height)
    {
        *width = m_width;
        *height = m_height;
    }

    // ----------------------------------------------------------------------
    // Returns oY angle
    // ----------------------------------------------------------------------
    virtual float GetYaw()
    {
        return m_yaw;
    }

    // ----------------------------------------------------------------------
    // Returns oX angle
    // ----------------------------------------------------------------------
    virtual float GetPitch()
    {
        return m_pitch;
    }

    // ----------------------------------------------------------------------
    // Returns position
    // ----------------------------------------------------------------------
    virtual xVector3 * GetPosition()
    {
        return m_position;
    }

    // ----------------------------------------------------------------------
    // Return direction
    // ----------------------------------------------------------------------
    virtual xVector3 * GetDirection()
    {
        return m_direction;
    }

    // ----------------------------------------------------------------------
    // Calculates current frustum pyramid (6 planes) and
    // updates view port, model view and projection matrices
    // ----------------------------------------------------------------------
    virtual void UpdateFrustumPyramid()
    {
        GLdouble clip[16];
        GLfloat t;

        glGetIntegerv(GL_VIEWPORT, m_viewport);
        glGetDoublev(GL_PROJECTION_MATRIX, m_projection);
        glGetDoublev(GL_MODELVIEW_MATRIX, m_model);

        clip[ 0] = m_model[ 0] * m_projection[ 0] + m_model[ 1] * m_projection[ 4] + m_model[ 2] * m_projection[ 8] + m_model[ 3] * m_projection[12];
        clip[ 1] = m_model[ 0] * m_projection[ 1] + m_model[ 1] * m_projection[ 5] + m_model[ 2] * m_projection[ 9] + m_model[ 3] * m_projection[13];
        clip[ 2] = m_model[ 0] * m_projection[ 2] + m_model[ 1] * m_projection[ 6] + m_model[ 2] * m_projection[10] + m_model[ 3] * m_projection[14];
        clip[ 3] = m_model[ 0] * m_projection[ 3] + m_model[ 1] * m_projection[ 7] + m_model[ 2] * m_projection[11] + m_model[ 3] * m_projection[15];

        clip[ 4] = m_model[ 4] * m_projection[ 0] + m_model[ 5] * m_projection[ 4] + m_model[ 6] * m_projection[ 8] + m_model[ 7] * m_projection[12];
        clip[ 5] = m_model[ 4] * m_projection[ 1] + m_model[ 5] * m_projection[ 5] + m_model[ 6] * m_projection[ 9] + m_model[ 7] * m_projection[13];
        clip[ 6] = m_model[ 4] * m_projection[ 2] + m_model[ 5] * m_projection[ 6] + m_model[ 6] * m_projection[10] + m_model[ 7] * m_projection[14];
        clip[ 7] = m_model[ 4] * m_projection[ 3] + m_model[ 5] * m_projection[ 7] + m_model[ 6] * m_projection[11] + m_model[ 7] * m_projection[15];

        clip[ 8] = m_model[ 8] * m_projection[ 0] + m_model[ 9] * m_projection[ 4] + m_model[10] * m_projection[ 8] + m_model[11] * m_projection[12];
        clip[ 9] = m_model[ 8] * m_projection[ 1] + m_model[ 9] * m_projection[ 5] + m_model[10] * m_projection[ 9] + m_model[11] * m_projection[13];
        clip[10] = m_model[ 8] * m_projection[ 2] + m_model[ 9] * m_projection[ 6] + m_model[10] * m_projection[10] + m_model[11] * m_projection[14];
        clip[11] = m_model[ 8] * m_projection[ 3] + m_model[ 9] * m_projection[ 7] + m_model[10] * m_projection[11] + m_model[11] * m_projection[15];

        clip[12] = m_model[12] * m_projection[ 0] + m_model[13] * m_projection[ 4] + m_model[14] * m_projection[ 8] + m_model[15] * m_projection[12];
        clip[13] = m_model[12] * m_projection[ 1] + m_model[13] * m_projection[ 5] + m_model[14] * m_projection[ 9] + m_model[15] * m_projection[13];
        clip[14] = m_model[12] * m_projection[ 2] + m_model[13] * m_projection[ 6] + m_model[14] * m_projection[10] + m_model[15] * m_projection[14];
        clip[15] = m_model[12] * m_projection[ 3] + m_model[13] * m_projection[ 7] + m_model[14] * m_projection[11] + m_model[15] * m_projection[15];

        m_Frustum[0][0] = clip[ 3] - clip[ 0];
        m_Frustum[0][1] = clip[ 7] - clip[ 4];
        m_Frustum[0][2] = clip[11] - clip[ 8];
        m_Frustum[0][3] = clip[15] - clip[12];

        t = GLfloat(sqrt( m_Frustum[0][0] * m_Frustum[0][0] + m_Frustum[0][1] * m_Frustum[0][1] + m_Frustum[0][2] * m_Frustum[0][2] ));
        m_Frustum[0][0] /= t;
        m_Frustum[0][1] /= t;
        m_Frustum[0][2] /= t;
        m_Frustum[0][3] /= t;

        m_Frustum[1][0] = clip[ 3] + clip[ 0];
        m_Frustum[1][1] = clip[ 7] + clip[ 4];
        m_Frustum[1][2] = clip[11] + clip[ 8];
        m_Frustum[1][3] = clip[15] + clip[12];

        t = GLfloat(sqrt( m_Frustum[1][0] * m_Frustum[1][0] + m_Frustum[1][1] * m_Frustum[1][1] + m_Frustum[1][2] * m_Frustum[1][2] ));
        m_Frustum[1][0] /= t;
        m_Frustum[1][1] /= t;
        m_Frustum[1][2] /= t;
        m_Frustum[1][3] /= t;

        m_Frustum[2][0] = clip[ 3] + clip[ 1];
        m_Frustum[2][1] = clip[ 7] + clip[ 5];
        m_Frustum[2][2] = clip[11] + clip[ 9];
        m_Frustum[2][3] = clip[15] + clip[13];

        t = GLfloat(sqrt( m_Frustum[2][0] * m_Frustum[2][0] + m_Frustum[2][1] * m_Frustum[2][1] + m_Frustum[2][2] * m_Frustum[2][2] ));
        m_Frustum[2][0] /= t;
        m_Frustum[2][1] /= t;
        m_Frustum[2][2] /= t;
        m_Frustum[2][3] /= t;

        m_Frustum[3][0] = clip[ 3] - clip[ 1];
        m_Frustum[3][1] = clip[ 7] - clip[ 5];
        m_Frustum[3][2] = clip[11] - clip[ 9];
        m_Frustum[3][3] = clip[15] - clip[13];

        t = GLfloat(sqrt( m_Frustum[3][0] * m_Frustum[3][0] + m_Frustum[3][1] * m_Frustum[3][1] + m_Frustum[3][2] * m_Frustum[3][2] ));
        m_Frustum[3][0] /= t;
        m_Frustum[3][1] /= t;
        m_Frustum[3][2] /= t;
        m_Frustum[3][3] /= t;

        m_Frustum[4][0] = clip[ 3] - clip[ 2];
        m_Frustum[4][1] = clip[ 7] - clip[ 6];
        m_Frustum[4][2] = clip[11] - clip[10];
        m_Frustum[4][3] = clip[15] - clip[14];

        t = GLfloat(sqrt( m_Frustum[4][0] * m_Frustum[4][0] + m_Frustum[4][1] * m_Frustum[4][1] + m_Frustum[4][2] * m_Frustum[4][2] ));
        m_Frustum[4][0] /= t;
        m_Frustum[4][1] /= t;
        m_Frustum[4][2] /= t;
        m_Frustum[4][3] /= t;

        m_Frustum[5][0] = clip[ 3] + clip[ 2];
        m_Frustum[5][1] = clip[ 7] + clip[ 6];
        m_Frustum[5][2] = clip[11] + clip[10];
        m_Frustum[5][3] = clip[15] + clip[14];

        t = GLfloat(sqrt( m_Frustum[5][0] * m_Frustum[5][0] + m_Frustum[5][1] * m_Frustum[5][1] + m_Frustum[5][2] * m_Frustum[5][2] ));
        m_Frustum[5][0] /= t;
        m_Frustum[5][1] /= t;
        m_Frustum[5][2] /= t;
        m_Frustum[5][3] /= t;
    }

    // ----------------------------------------------------------------------
    // Returns true if point in the frustum pyramid
    // ----------------------------------------------------------------------
    virtual bool IsPointInFrustumPyramid(xVector3 * point)
    {
        int i;

        for(i = 0; i < 6; i++)
        {
            if(m_Frustum[i][0] * point->x + m_Frustum[i][1] * point->y + m_Frustum[i][2] * point->z + m_Frustum[i][3] <= 0)
            {
                return false;
            }
        }
        return true;
    }

    // ----------------------------------------------------------------------
    // Returns true if point occluded (is not visible on screen)
    // ----------------------------------------------------------------------
    virtual bool IsPointOccluded(xVector3 * point)
    {
        GLdouble winx, winy, winz;
        GLdouble flareZ;
        GLfloat bufferZ;

        gluProject(point->x, point->y, point->z, m_model, m_projection, m_viewport, &winx, &winy, &winz);
        flareZ = winz;

        glReadPixels(winx, winy,1,1,GL_DEPTH_COMPONENT, GL_FLOAT, &bufferZ);

        if (bufferZ < flareZ) {
            return true;
        } else {
            return false;
        }
    }

    // ----------------------------------------------------------------------
    // Returns true if full sphere in frustum
    // ----------------------------------------------------------------------
    virtual bool IsSphereInFrustum(xVector3 * point, float radius)
    {
        for(int i = 0; i < 6; i++ )
        {
            if( m_Frustum[i][0] * point->x + m_Frustum[i][1] * point->y + m_Frustum[i][2] * point->z +
                m_Frustum[i][3] <= -radius )
            {
                return false;
            }
        }

        return true;
    }

    // ----------------------------------------------------------------------
    // Returns true if full cube in frustum
    // ----------------------------------------------------------------------
    virtual bool IsCubeInFrustum(xVector3 * point, float size )
    {
        for(int i = 0; i < 6; i++ )
        {
            if(m_Frustum[i][0] * (point->x - size) + m_Frustum[i][1] * (point->y - size) +
               m_Frustum[i][2] * (point->z - size) + m_Frustum[i][3] > 0)
                continue;
            if(m_Frustum[i][0] * (point->x + size) + m_Frustum[i][1] * (point->y - size) +
               m_Frustum[i][2] * (point->z - size) + m_Frustum[i][3] > 0)
                continue;
            if(m_Frustum[i][0] * (point->x - size) + m_Frustum[i][1] * (point->y + size) +
               m_Frustum[i][2] * (point->z - size) + m_Frustum[i][3] > 0)
                continue;
            if(m_Frustum[i][0] * (point->x + size) + m_Frustum[i][1] * (point->y + size) +
               m_Frustum[i][2] * (point->z - size) + m_Frustum[i][3] > 0)
                continue;
            if(m_Frustum[i][0] * (point->x - size) + m_Frustum[i][1] * (point->y - size) +
               m_Frustum[i][2] * (point->z + size) + m_Frustum[i][3] > 0)
                continue;
            if(m_Frustum[i][0] * (point->x + size) + m_Frustum[i][1] * (point->y - size) +
               m_Frustum[i][2] * (point->z + size) + m_Frustum[i][3] > 0)
                continue;
            if(m_Frustum[i][0] * (point->x - size) + m_Frustum[i][1] * (point->y + size) +
               m_Frustum[i][2] * (point->z + size) + m_Frustum[i][3] > 0)
                continue;
            if(m_Frustum[i][0] * (point->x + size) + m_Frustum[i][1] * (point->y + size) +
               m_Frustum[i][2] * (point->z + size) + m_Frustum[i][3] > 0)
                continue;

            return false;
        }

        return true;
    }

    // ----------------------------------------------------------------------
    // Renders glare effect for light (with check of occlusion and frustum)
    // ----------------------------------------------------------------------
    virtual void RenderGlareEffect(xLight * light)
    {
        // Note: screen coordinates
        // ox: left 0 -> m_width right
        // oy: down 0 -> m_height up
        // Steps:
        // calculate light position in screen coordinates
        // calculate direction vector from light to center of screen
        // set orthographic projection
        // draw effects of camera

        GLfloat Length = 0;
        xArray4 * pos = light->GetPosition();
        xVector3 LightSourcePos;
        LightSourcePos.Set(pos->values[0], pos->values[1], pos->values[2]);

        // Draw Glare Effect if light in frustum and it is not occluded
        if (IsPointInFrustumPyramid(&LightSourcePos) && !IsPointOccluded(&LightSourcePos))
        {
            double x,y,z;
            gluProject(pos->values[0], pos->values[1], pos->values[2], m_model, m_projection, m_viewport, &x, &y, &z);
            xVector2 m_LightSourcePos;
            m_LightSourcePos.Set((float)x, (float)y);
            xVector2 vLightSourceToIntersect(m_width/2.0, m_height/2.0);
            vLightSourceToIntersect.Diff(&m_LightSourcePos);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_TEXTURE_2D);

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0.0, m_width, 0.0, m_height, -1.0, 1.0);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            float scale = 57;
            float trans = 1.4;

            RenderBigGlow(light->m_bih_glow_color->values[0],
                          light->m_bih_glow_color->values[1],
                          light->m_bih_glow_color->values[2],
                          light->m_bih_glow_color->values[3],
                          m_LightSourcePos,
                          320.0f * light->m_big_glow_scale);

            RenderStreaks(light->m_streaks_color->values[0],
                          light->m_streaks_color->values[1],
                          light->m_streaks_color->values[2],
                          light->m_streaks_color->values[3],
                          m_LightSourcePos,
                          320.0f * light->m_streaks_scale);

            RenderGlow(light->m_glow_color->values[0],
                       light->m_glow_color->values[1],
                       light->m_glow_color->values[2],
                       light->m_glow_color->values[3],
                       m_LightSourcePos,
                       70.0f * light->m_glow_scale);

            xVector2 pt;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(0.1);
            pt.Sum(&m_LightSourcePos);

            RenderGlow(0.9f, 0.6f, 0.4f, 0.5f, pt, scale * 6.0f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(-0.064);
            pt.Sum(&m_LightSourcePos);

            RenderHalo(0.3f, 0.8f, 0.6f, 0.5f, pt, scale * 1.1f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(0.0);
            pt.Sum(&m_LightSourcePos);

            RenderHalo(0.8f, 0.5f, 0.6f, 0.5f, pt, scale * 1.7f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(trans * 0.175);
            pt.Sum(&m_LightSourcePos);

            RenderHalo(0.9f, 0.2f, 0.1f, 0.5f, pt, scale * 0.83f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(trans * 0.285);
            pt.Sum(&m_LightSourcePos);

            RenderHalo(0.7f, 0.7f, 0.4f, 0.5f, pt, scale * 1.6f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(trans * 0.2755);
            pt.Sum(&m_LightSourcePos);

            RenderGlow(0.9f, 0.9f, 0.2f, 0.5f, pt, scale * 0.8f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(trans * 0.4775);
            pt.Sum(&m_LightSourcePos);

            RenderGlow(0.93f, 0.82f, 0.73f, 0.5f, pt, scale * 1.0f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(trans * 0.49);
            pt.Sum(&m_LightSourcePos);

            RenderHalo(0.7f, 0.6f, 0.5f, 0.5f, pt, scale * 1.4f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(trans * 0.65);
            pt.Sum(&m_LightSourcePos);

            RenderGlow(0.7f, 0.8f, 0.3f, 0.5f, pt, scale * 1.8f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(trans * 0.63);
            pt.Sum(&m_LightSourcePos);

            RenderGlow(0.4f, 0.3f, 0.2f, 0.5f, pt, scale *  1.4f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(trans * 0.8);
            pt.Sum(&m_LightSourcePos);

            RenderHalo(0.7f, 0.5f, 0.5f, 0.5f, pt, scale * 3.1f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(trans * 0.7825);
            pt.Sum(&m_LightSourcePos);

            RenderGlow(0.8f, 0.5f, 0.1f, 0.5f, pt, scale * 2.6f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(trans * 1.0);
            pt.Sum(&m_LightSourcePos);

            RenderHalo(0.5f, 0.5f, 0.7f, 0.5f, pt, scale * 4.5f);

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(trans * 0.975);
            pt.Sum(&m_LightSourcePos);

            RenderGlow(0.4f, 0.1f, 0.9f, 0.5f, pt, scale * 2.0f);

            glPopMatrix();
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);

            glDisable(GL_BLEND );
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_TEXTURE_2D);
        }
    }

    // ----------------------------------------------------------------------
    // Renders Halo using color, position and scale
    // ----------------------------------------------------------------------
    virtual void RenderHalo(GLfloat r, GLfloat g, GLfloat b, GLfloat a, xVector2 p, GLfloat scale)
    {
        xVector2 q[4];

        q[0].x = (p.x - scale);
        q[0].y = (p.y - scale);
        q[1].x = (p.x - scale);
        q[1].y = (p.y + scale);
        q[2].x = (p.x + scale);
        q[2].y = (p.y - scale);
        q[3].x = (p.x + scale);
        q[3].y = (p.y + scale);

        glBindTexture(GL_TEXTURE_2D, m_HaloTexture->GetTextureID());
        glColor4f(r, g, b, a);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(q[0].x, q[0].y);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(q[1].x, q[1].y);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(q[2].x, q[2].y);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(q[3].x, q[3].y);
        glEnd();
    }

    // ----------------------------------------------------------------------
    // Renders Glow using color, position and scale
    // ----------------------------------------------------------------------
    virtual void RenderGlow(GLfloat r, GLfloat g, GLfloat b, GLfloat a, xVector2 p, GLfloat scale)
    {
        xVector2 q[4];

        q[0].x = (p.x - scale);
        q[0].y = (p.y - scale);
        q[1].x = (p.x - scale);
        q[1].y = (p.y + scale);
        q[2].x = (p.x + scale);
        q[2].y = (p.y - scale);
        q[3].x = (p.x + scale);
        q[3].y = (p.y + scale);

        glBindTexture(GL_TEXTURE_2D, m_GlowTexture->GetTextureID());
        glColor4f(r, g, b, a);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(q[0].x, q[0].y);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(q[1].x, q[1].y);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(q[2].x, q[2].y);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(q[3].x, q[3].y);
        glEnd();
    }

    // ----------------------------------------------------------------------
    // Renders Big Glow using color, position and scale
    // ----------------------------------------------------------------------
    virtual void RenderBigGlow(GLfloat r, GLfloat g, GLfloat b, GLfloat a, xVector2 p, GLfloat scale)
    {
        xVector2 q[4];

        q[0].x = (p.x - scale);
        q[0].y = (p.y - scale);
        q[1].x = (p.x - scale);
        q[1].y = (p.y + scale);
        q[2].x = (p.x + scale);
        q[2].y = (p.y - scale);
        q[3].x = (p.x + scale);
        q[3].y = (p.y + scale);

        glBindTexture(GL_TEXTURE_2D, m_BigGlowTexture->GetTextureID());
        glColor4f(r, g, b, a);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(q[0].x, q[0].y);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(q[1].x, q[1].y);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(q[2].x, q[2].y);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(q[3].x, q[3].y);
        glEnd();
    }

    // ----------------------------------------------------------------------
    // Renders Streaks using color, position and scale
    // ----------------------------------------------------------------------
    virtual void RenderStreaks(GLfloat r, GLfloat g, GLfloat b, GLfloat a, xVector2 p, GLfloat scale)
    {
        xVector2 q[4];

        q[0].x = (p.x - scale);
        q[0].y = (p.y - scale);
        q[1].x = (p.x - scale);
        q[1].y = (p.y + scale);
        q[2].x = (p.x + scale);
        q[2].y = (p.y - scale);
        q[3].x = (p.x + scale);
        q[3].y = (p.y + scale);

        glBindTexture(GL_TEXTURE_2D, m_StreaksTexture->GetTextureID());
        glColor4f(r, g, b, a);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(q[0].x, q[0].y);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(q[1].x, q[1].y);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(q[2].x, q[2].y);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(q[3].x, q[3].y);
        glEnd();
    }

protected:

    int m_width;            // Viewport width
    int m_height;           // Viewport height
    GLdouble m_aspect;      // Viewport proportion (width / height)
    GLdouble m_angle;       // Angle of view for virtual frustum pyramid
    GLdouble m_front;       // Front plane of view
    GLdouble m_back;        // Back Plane of View
    float m_yaw;            // Camera's angle for global y axis
    float m_pitch;          // Camera's angle for global x axis
    xVector3 * m_direction; // Camera's direction (some kind of focus)
    xVector3 * m_position;  // Camera's global position
    double m_elapsed;       // Time to apply position change in dependence of velocity and time

    GLdouble m_Frustum[6][4];       // Current Frustum planes
    GLdouble m_projection[16];      // Current projection matrix
    GLdouble m_model[16];           // Current model view matrix
    GLint m_viewport[4];            // Current viewport matrix
    xTexture * m_StreaksTexture;    // Texture (camera effect)
    xTexture * m_BigGlowTexture;    // Texture (camera effect)
    xTexture * m_GlowTexture;       // Texture (camera effect)
    xTexture * m_HaloTexture;       // Texture (camera effect)



};

#endif //OXYGEN_XVIRTUALCAMERA_H
