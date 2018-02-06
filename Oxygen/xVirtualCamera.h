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
    //
    // ----------------------------------------------------------------------
    virtual void UpdateFrustumPyramid()
    {
        GLfloat  clip[16];
        GLfloat  projection[16];
        GLfloat  model[16];
        GLfloat  t;

        // Взять текущую матрицу проекции
        glGetFloatv( GL_PROJECTION_MATRIX, projection );

        // Взять текущую матрицу вида и модели
        glGetFloatv( GL_MODELVIEW_MATRIX, model );

        // Умножить две матрицы
        clip[ 0] = model[ 0] * projection[ 0] + model[ 1] * projection[ 4] + model[ 2] * projection[ 8] + model[ 3] * projection[12];
        clip[ 1] = model[ 0] * projection[ 1] + model[ 1] * projection[ 5] + model[ 2] * projection[ 9] + model[ 3] * projection[13];
        clip[ 2] = model[ 0] * projection[ 2] + model[ 1] * projection[ 6] + model[ 2] * projection[10] + model[ 3] * projection[14];
        clip[ 3] = model[ 0] * projection[ 3] + model[ 1] * projection[ 7] + model[ 2] * projection[11] + model[ 3] * projection[15];

        clip[ 4] = model[ 4] * projection[ 0] + model[ 5] * projection[ 4] + model[ 6] * projection[ 8] + model[ 7] * projection[12];
        clip[ 5] = model[ 4] * projection[ 1] + model[ 5] * projection[ 5] + model[ 6] * projection[ 9] + model[ 7] * projection[13];
        clip[ 6] = model[ 4] * projection[ 2] + model[ 5] * projection[ 6] + model[ 6] * projection[10] + model[ 7] * projection[14];
        clip[ 7] = model[ 4] * projection[ 3] + model[ 5] * projection[ 7] + model[ 6] * projection[11] + model[ 7] * projection[15];

        clip[ 8] = model[ 8] * projection[ 0] + model[ 9] * projection[ 4] + model[10] * projection[ 8] + model[11] * projection[12];
        clip[ 9] = model[ 8] * projection[ 1] + model[ 9] * projection[ 5] + model[10] * projection[ 9] + model[11] * projection[13];
        clip[10] = model[ 8] * projection[ 2] + model[ 9] * projection[ 6] + model[10] * projection[10] + model[11] * projection[14];
        clip[11] = model[ 8] * projection[ 3] + model[ 9] * projection[ 7] + model[10] * projection[11] + model[11] * projection[15];

        clip[12] = model[12] * projection[ 0] + model[13] * projection[ 4] + model[14] * projection[ 8] + model[15] * projection[12];
        clip[13] = model[12] * projection[ 1] + model[13] * projection[ 5] + model[14] * projection[ 9] + model[15] * projection[13];
        clip[14] = model[12] * projection[ 2] + model[13] * projection[ 6] + model[14] * projection[10] + model[15] * projection[14];
        clip[15] = model[12] * projection[ 3] + model[13] * projection[ 7] + model[14] * projection[11] + model[15] * projection[15];

        // Извлечь правую плоскость
        m_Frustum[0][0] = clip[ 3] - clip[ 0];
        m_Frustum[0][1] = clip[ 7] - clip[ 4];
        m_Frustum[0][2] = clip[11] - clip[ 8];
        m_Frustum[0][3] = clip[15] - clip[12];

        // Нормализовать результат
        t = GLfloat(sqrt( m_Frustum[0][0] * m_Frustum[0][0] + m_Frustum[0][1] * m_Frustum[0][1] + m_Frustum[0][2] * m_Frustum[0][2] ));
        m_Frustum[0][0] /= t;
        m_Frustum[0][1] /= t;
        m_Frustum[0][2] /= t;
        m_Frustum[0][3] /= t;

        // Извлечь левую плоскость
        m_Frustum[1][0] = clip[ 3] + clip[ 0];
        m_Frustum[1][1] = clip[ 7] + clip[ 4];
        m_Frustum[1][2] = clip[11] + clip[ 8];
        m_Frustum[1][3] = clip[15] + clip[12];

        // Нормализовать результат
        t = GLfloat(sqrt( m_Frustum[1][0] * m_Frustum[1][0] + m_Frustum[1][1] * m_Frustum[1][1] + m_Frustum[1][2] * m_Frustum[1][2] ));
        m_Frustum[1][0] /= t;
        m_Frustum[1][1] /= t;
        m_Frustum[1][2] /= t;
        m_Frustum[1][3] /= t;

        // Извлечь нижнюю плоскость
        m_Frustum[2][0] = clip[ 3] + clip[ 1];
        m_Frustum[2][1] = clip[ 7] + clip[ 5];
        m_Frustum[2][2] = clip[11] + clip[ 9];
        m_Frustum[2][3] = clip[15] + clip[13];

        // Нормализовать результат
        t = GLfloat(sqrt( m_Frustum[2][0] * m_Frustum[2][0] + m_Frustum[2][1] * m_Frustum[2][1] + m_Frustum[2][2] * m_Frustum[2][2] ));
        m_Frustum[2][0] /= t;
        m_Frustum[2][1] /= t;
        m_Frustum[2][2] /= t;
        m_Frustum[2][3] /= t;

        // Извлечь верхнюю плоскость
        m_Frustum[3][0] = clip[ 3] - clip[ 1];
        m_Frustum[3][1] = clip[ 7] - clip[ 5];
        m_Frustum[3][2] = clip[11] - clip[ 9];
        m_Frustum[3][3] = clip[15] - clip[13];

        // Нормализовать результат
        t = GLfloat(sqrt( m_Frustum[3][0] * m_Frustum[3][0] + m_Frustum[3][1] * m_Frustum[3][1] + m_Frustum[3][2] * m_Frustum[3][2] ));
        m_Frustum[3][0] /= t;
        m_Frustum[3][1] /= t;
        m_Frustum[3][2] /= t;
        m_Frustum[3][3] /= t;

        // Извлечь дальнюю плоскость
        m_Frustum[4][0] = clip[ 3] - clip[ 2];
        m_Frustum[4][1] = clip[ 7] - clip[ 6];
        m_Frustum[4][2] = clip[11] - clip[10];
        m_Frustum[4][3] = clip[15] - clip[14];

        // Нормализовать результат
        t = GLfloat(sqrt( m_Frustum[4][0] * m_Frustum[4][0] + m_Frustum[4][1] * m_Frustum[4][1] + m_Frustum[4][2] * m_Frustum[4][2] ));
        m_Frustum[4][0] /= t;
        m_Frustum[4][1] /= t;
        m_Frustum[4][2] /= t;
        m_Frustum[4][3] /= t;

        // Извлечь ближнюю плоскость
        m_Frustum[5][0] = clip[ 3] + clip[ 2];
        m_Frustum[5][1] = clip[ 7] + clip[ 6];
        m_Frustum[5][2] = clip[11] + clip[10];
        m_Frustum[5][3] = clip[15] + clip[14];

        // Нормализовать результат
        t = GLfloat(sqrt( m_Frustum[5][0] * m_Frustum[5][0] + m_Frustum[5][1] * m_Frustum[5][1] + m_Frustum[5][2] * m_Frustum[5][2] ));
        m_Frustum[5][0] /= t;
        m_Frustum[5][1] /= t;
        m_Frustum[5][2] /= t;
        m_Frustum[5][3] /= t;
    }

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual bool IsPointInFrustumPyramid(xVector3 * point)
    {
        int i;
        // Идея этого алгоритма в том, что если точка
        // внутри всех 6 отсекающих плоскостей,
        // тогда она внутри пирамиды видимости,
        // и тогда функция вернет True.
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
    //
    // ----------------------------------------------------------------------
    virtual bool IsPointOccluded(xVector3 * point)
    {
        GLint viewport[4]; // Место для данных окна просмотра
        GLdouble mvmatrix[16], projmatrix[16]; // Место для матрицы трансформации
        GLdouble winx, winy, winz; // Место для координат проекции
        GLdouble flareZ; // Z-координата
        GLfloat bufferZ; // Глубина

        glGetIntegerv (GL_VIEWPORT, viewport); // Извлечь текущее окно просмотра
        glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix); // Извлечь текущую матрицу просмотра
        glGetDoublev (GL_PROJECTION_MATRIX, projmatrix); // Извлечь матрицу проекции

        // Определим 2D позиции 3D точки
        gluProject(point->x, point->y, point->z, mvmatrix, projmatrix, viewport, &winx, &winy, &winz);
        flareZ = winz;

        // Прочитаем один пиксель из буфера глубины (точно там, где наш блик будет нарисован)
        glReadPixels(winx, winy,1,1,GL_DEPTH_COMPONENT, GL_FLOAT, &bufferZ);

        // Если текущая z-координата пикселя там, где будет блик меньше, чем
        // z-координата точки p, то перед бликом что-то есть.
        if (bufferZ < flareZ) {
            return true;
        } else {
            return false;
        }
    }

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual bool IsSphereInFrustum(xVector3 * point, float radius)
    {
        // Проходим через все стороны пирамиды
        for(int i = 0; i < 6; i++ )
        {
            // Если центр сферы дальше от плоскости, чем её радиус
            if( m_Frustum[i][0] * point->x + m_Frustum[i][1] * point->y + m_Frustum[i][2] * point->z +
                m_Frustum[i][3] <= -radius )
            {
                // То и вся сфера снаружи, возвращаем false
                return false;
            }
        }

        // Иначе сфера внутри
        return true;
    }

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual bool IsCubeInFrustum(xVector3 * point, float size )
    {
        // Тут работы немного больше, но тоже не слишком много.
        // Нам передаётся центр куба и половина его длинны. Думайте о длинне так же,
        // как о радиусе для сферы. Мы проверяем каждую точку куба на нахождение внутри
        // пирамиды. Если точка находится перед стороной, переходим к следующей сторону.

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

            // Если мы дошли досюда, куб не внутри пирамиды.
            return false;
        }

        return true;
    }

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual void RenderGlareEffect(xLight * light)
    {
        GLfloat Length = 0.0f;
        xArray4 * pos = light->GetPosition();
        xVector3 m_LightSourcePos;
        m_LightSourcePos.Set(pos->values[0], pos->values[1], pos->values[2]);

        // Нарисовать блик, если источник света на одной линии обзора
        if (!IsPointOccluded(&m_LightSourcePos))
        {
            // Вектор между камерой и источником света
            xVector3 vLightSourceToCamera;
            vLightSourceToCamera.Set(m_position);
            vLightSourceToCamera.Diff(&m_LightSourcePos);
            // Длина
            Length = vLightSourceToCamera.Module();
            // Найдем точку на векторе направления камеры
            xVector3 ptIntersect;
            ptIntersect.Set(m_direction);
            ptIntersect.MultScalar(Length);
            // Найдем действительные координаты точки на вектор направления камеры
            ptIntersect.Sum(m_position);
            // Вычислим вектор, между источником света и точкой пересечения
            xVector3 vLightSourceToIntersect;
            vLightSourceToIntersect.Set(&ptIntersect);
            vLightSourceToIntersect.Diff(&m_LightSourcePos);
            Length = vLightSourceToIntersect.Module(); // Длина
            vLightSourceToIntersect.Normalize(); // Нормализуем вектор

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_TEXTURE_2D);

            // нарисовать большое неясное световое пятно
            RenderBigGlow(light->m_bih_glow_color->values[0],
                          light->m_bih_glow_color->values[1],
                          light->m_bih_glow_color->values[2],
                          light->m_bih_glow_color->values[3],
                          m_LightSourcePos,
                          16.0f * light->m_big_glow_scale);
            // Полосы
            RenderStreaks(light->m_streaks_color->values[0],
                          light->m_streaks_color->values[1],
                          light->m_streaks_color->values[2],
                          light->m_streaks_color->values[3],
                          m_LightSourcePos,
                          16.0f * light->m_streaks_scale);
            // маленькое пятно
            RenderGlow(light->m_glow_color->values[0],
                       light->m_glow_color->values[1],
                       light->m_glow_color->values[2],
                       light->m_glow_color->values[3],
                       m_LightSourcePos,
                       3.5f * light->m_glow_scale);

            xVector3 pt;

            // Точка на 15% дальше от источника света по направлению к точке пересечения
            //pt = vLightSourceToIntersect * (Length * 0.1f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 0.1);
            pt.Sum(&m_LightSourcePos);

            // Маленькое пятно
            RenderGlow(0.9f, 0.6f, 0.4f, 0.5f, pt, 0.6f);

            // Точка на 30% дальше от источника света по направлению к точке пересечения
            //pt = vLightSourceToIntersect * (Length * 0.15f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 0.15);
            pt.Sum(&m_LightSourcePos);

            // Ореол
            RenderHalo(0.8f, 0.5f, 0.6f, 0.5f, pt, 1.7f);

            // Точка на 35% дальше от источника света по направлению к точке пересечения
            //pt = vLightSourceToIntersect * (Length * 0.175f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 0.175);
            pt.Sum(&m_LightSourcePos);

            // Ореол
            RenderHalo(0.9f, 0.2f, 0.1f, 0.5f, pt, 0.83f);

            // Точка на 57% дальше от источника света по направлению к точке пересечения
            //pt = vLightSourceToIntersect * (Length * 0.285f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 0.285);
            pt.Sum(&m_LightSourcePos);

            // Ореол
            RenderHalo(0.7f, 0.7f, 0.4f, 0.5f, pt, 1.6f);

            // Точка на 55.1% дальше от источника света по направлению к точке пересечения
            //pt = vLightSourceToIntersect * (Length * 0.2755f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 0.2755);
            pt.Sum(&m_LightSourcePos);

            // Маленькое пятно
            RenderGlow(0.9f, 0.9f, 0.2f, 0.5f, pt, 0.8f);

            // Точка на 95.5% дальше от источника света по направлению к точке пересечения
            //pt = vLightSourceToIntersect * (Length * 0.4775f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 0.4775);
            pt.Sum(&m_LightSourcePos);

            // Маленькое пятно
            RenderGlow(0.93f, 0.82f, 0.73f, 0.5f, pt, 1.0f);

            // Точка на 98% дальше от источника света по направлению к точке пересечения
            //pt = vLightSourceToIntersect * (Length * 0.49f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 0.49);
            pt.Sum(&m_LightSourcePos);

            // Ореол
            RenderHalo(0.7f, 0.6f, 0.5f, 0.5f, pt, 1.4f);

            // Точка на 130% дальше от источника света по направлению к точке пересечения
            //pt = vLightSourceToIntersect * (Length * 0.65f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 0.65);
            pt.Sum(&m_LightSourcePos);

            // Маленькое пятно
            RenderGlow(0.7f, 0.8f, 0.3f, 0.5f, pt, 1.8f);

            // Точка на 126% дальше от источника света по направлению к точке пересечения
            //pt = vLightSourceToIntersect * (Length * 0.63f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 0.63);
            pt.Sum(&m_LightSourcePos);

            // Маленькое пятно
            RenderGlow(0.4f, 0.3f, 0.2f, 0.5f, pt, 1.4f);

            // Точка на 160% дальше от источника света по направлению к точке пересечения
            //pt = vLightSourceToIntersect * (Length * 0.8f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 0.8);
            pt.Sum(&m_LightSourcePos);

            // Ореол
            RenderHalo(0.7f, 0.5f, 0.5f, 0.5f, pt, 3.1f);

            // Точка на 156.5% дальше от источника света по направлению к точке пересечения
            //pt = vLightSourceToIntersect * (Length * 0.7825f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 0.7825);
            pt.Sum(&m_LightSourcePos);

            // Маленькое пятно
            RenderGlow(0.8f, 0.5f, 0.1f, 0.5f, pt, 2.6f);

            //pt = vLightSourceToIntersect * (Length * 1.0f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 1.0);
            pt.Sum(&m_LightSourcePos);

            // Ореол
            RenderHalo(0.5f, 0.5f, 0.7f, 0.5f, pt, 4.5f);

            // Точка на 195% дальше от источника света по направлению к точке пересечения
            //pt = vLightSourceToIntersect * (Length * 0.975f);
            //pt += m_LightSourcePos;

            pt.Set(&vLightSourceToIntersect);
            pt.MultScalar(Length * 0.975);
            pt.Sum(&m_LightSourcePos);

            // Маленькое пятно
            RenderGlow(0.4f, 0.1f, 0.9f, 0.5f, pt, 2.0f);

            glDisable(GL_BLEND );
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_TEXTURE_2D);
        }
    }

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual void RenderHalo(GLfloat r, GLfloat g, GLfloat b, GLfloat a, xVector3 p, GLfloat scale)
    {
        xVector2 q[4];

        // По существу мы делаем 2D прямоугольник из 4 точек
        // при этом мы не нуждаемся в z-координате, поскольку
        // мы инверсно вращаем камеру, поэтому
        // текстурированный прямоугольник будет всегда обращен
        // на зрителя.
        q[0].x = (p.x - scale); // Задать x-координату минус масштаб относительно центральной точки.
        q[0].y = (p.y - scale); // Задать y-координату минус масштаб относительно центральной точки.
        q[1].x = (p.x - scale); // Задать x-координату минус масштаб относительно центральной точки.
        q[1].y = (p.y + scale); // Задать y-координату плюс масштаб относительно центральной точки.
        q[2].x = (p.x + scale); // Задать x-координату плюс масштаб относительно центральной точки.
        q[2].y = (p.y - scale); // Задать y-координату минус масштаб относительно центральной точки.
        q[3].x = (p.x + scale); // Задать x-координату плюс масштаб относительно центральной точки.
        q[3].y = (p.y + scale); // Задать y-координату плюс масштаб относительно центральной точки.

        glPushMatrix(); // Сохранить матрицу вида модели
        glTranslatef(p.x, p.y, p.z); // Сместить в точку
        glRotatef(-m_yaw, 0.0f, 1.0f, 0.0f);
        glRotatef(-m_pitch, 1.0f, 0.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, m_HaloTexture->GetTextureID()); // Привязать текстуру ореола
        glColor4f(r, g, b, a); // Задать цвет, так как текстура в градациях серого
        glBegin(GL_TRIANGLE_STRIP); // Нарисовать ореол
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(q[0].x, q[0].y);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(q[1].x, q[1].y);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(q[2].x, q[2].y);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(q[3].x, q[3].y);
        glEnd();
        glPopMatrix(); // Восстановить матрицу вида модели
    }

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual void RenderGlow(GLfloat r, GLfloat g, GLfloat b, GLfloat a, xVector3 p, GLfloat scale)
    {
        xVector2 q[4];

        // По существу мы делаем 2D прямоугольник из 4 точек
        // при этом мы не нуждаемся в z-координате, поскольку
        // мы инверсно вращаем камеру, поэтому
        // текстурированный прямоугольник будет всегда обращен
        // на зрителя.
        q[0].x = (p.x - scale); // Задать x-координату минус масштаб относительно центральной точки.
        q[0].y = (p.y - scale); // Задать y-координату минус масштаб относительно центральной точки.
        q[1].x = (p.x - scale); // Задать x-координату минус масштаб относительно центральной точки.
        q[1].y = (p.y + scale); // Задать y-координату плюс масштаб относительно центральной точки.
        q[2].x = (p.x + scale); // Задать x-координату плюс масштаб относительно центральной точки.
        q[2].y = (p.y - scale); // Задать y-координату минус масштаб относительно центральной точки.
        q[3].x = (p.x + scale); // Задать x-координату плюс масштаб относительно центральной точки.
        q[3].y = (p.y + scale); // Задать y-координату плюс масштаб относительно центральной точки.

        glPushMatrix(); // Сохранить матрицу вида модели
        glTranslatef(p.x, p.y, p.z); // Сместить в точку
        glRotatef(-m_yaw, 0.0f, 1.0f, 0.0f);
        glRotatef(-m_pitch, 1.0f, 0.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, m_GlowTexture->GetTextureID()); // Привязать текстуру пятна
        glColor4f(r, g, b, a); // Задать цвет, так как текстура в градациях серого
        glBegin(GL_TRIANGLE_STRIP); // Нарисовать пятно
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(q[0].x, q[0].y);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(q[1].x, q[1].y);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(q[2].x, q[2].y);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(q[3].x, q[3].y);
        glEnd();
        glPopMatrix(); // Восстановить матрицу вида модели
    }

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual void RenderBigGlow(GLfloat r, GLfloat g, GLfloat b, GLfloat a, xVector3 p, GLfloat scale)
    {
        xVector2 q[4];

        // По существу мы делаем 2D прямоугольник из 4 точек
        // при этом мы не нуждаемся в z-координате, поскольку
        // мы инверсно вращаем камеру, поэтому
        // текстурированный прямоугольник будет всегда обращен
        // на зрителя.
        q[0].x = (p.x - scale); // Задать x-координату минус масштаб относительно центральной точки.
        q[0].y = (p.y - scale); // Задать y-координату минус масштаб относительно центральной точки.
        q[1].x = (p.x - scale); // Задать x-координату минус масштаб относительно центральной точки.
        q[1].y = (p.y + scale); // Задать y-координату плюс масштаб относительно центральной точки.
        q[2].x = (p.x + scale); // Задать x-координату плюс масштаб относительно центральной точки.
        q[2].y = (p.y - scale); // Задать y-координату минус масштаб относительно центральной точки.
        q[3].x = (p.x + scale); // Задать x-координату плюс масштаб относительно центральной точки.
        q[3].y = (p.y + scale); // Задать y-координату плюс масштаб относительно центральной точки.

        glPushMatrix(); // Сохранить матрицу вида модели
        glTranslatef(p.x, p.y, p.z); // Сместить в точку
        glRotatef(-m_yaw, 0.0f, 1.0f, 0.0f);
        glRotatef(-m_pitch, 1.0f, 0.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, m_BigGlowTexture->GetTextureID()); // Привязать текстуру пятна
        glColor4f(r, g, b, a); // Задать цвет, так как текстура в градациях серого
        glBegin(GL_TRIANGLE_STRIP); // Нарисовать пятно
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(q[0].x, q[0].y);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(q[1].x, q[1].y);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(q[2].x, q[2].y);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(q[3].x, q[3].y);
        glEnd();
        glPopMatrix(); // Восстановить матрицу вида модели
    }

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual void RenderStreaks(GLfloat r, GLfloat g, GLfloat b, GLfloat a, xVector3 p, GLfloat scale)
    {
        xVector2 q[4];

        // По существу мы делаем 2D прямоугольник из 4 точек
        // при этом мы не нуждаемся в z-координате, поскольку
        // мы инверсно вращаем камеру, поэтому
        // текстурированный прямоугольник будет всегда обращен
        // на зрителя.
        q[0].x = (p.x - scale); // Задать x-координату минус масштаб относительно центральной точки.
        q[0].y = (p.y - scale); // Задать y-координату минус масштаб относительно центральной точки.
        q[1].x = (p.x - scale); // Задать x-координату минус масштаб относительно центральной точки.
        q[1].y = (p.y + scale); // Задать y-координату плюс масштаб относительно центральной точки.
        q[2].x = (p.x + scale); // Задать x-координату плюс масштаб относительно центральной точки.
        q[2].y = (p.y - scale); // Задать y-координату минус масштаб относительно центральной точки.
        q[3].x = (p.x + scale); // Задать x-координату плюс масштаб относительно центральной точки.
        q[3].y = (p.y + scale); // Задать y-координату плюс масштаб относительно центральной точки.

        glPushMatrix(); // Сохранить матрицу вида модели
        glTranslatef(p.x, p.y, p.z); // Сместить в точку
        glRotatef(-m_yaw, 0.0f, 1.0f, 0.0f);
        glRotatef(-m_pitch, 1.0f, 0.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, m_StreaksTexture->GetTextureID()); // Привязать текстуру полос
        glColor4f(r, g, b, a); // Задать цвет, так как текстура в градациях серого
        glBegin(GL_TRIANGLE_STRIP); // Нарисовать полосы
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(q[0].x, q[0].y);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(q[1].x, q[1].y);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(q[2].x, q[2].y);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(q[3].x, q[3].y);
        glEnd();
        glPopMatrix(); // Восстановить матрицу вида модели
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

    GLfloat m_Frustum[6][4];        //
    xTexture * m_StreaksTexture;    //
    xTexture * m_BigGlowTexture;    //
    xTexture * m_GlowTexture;       //
    xTexture * m_HaloTexture;       //



};

#endif //OXYGEN_XVIRTUALCAMERA_H
