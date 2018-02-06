/*
 *  Project: Oxygen Game Engine
 *  Created by Egor Orachyov on 05.02.2018.
 *  Copyright
 *
 *  xFreeCamera use class virtual camera as
 *  base to provide simple functions for
 *  work with cinematic camera (add inertia
 *  to movement). Allows: move forward, rotate
 *  and etc. with effect of sliding camera
 */

#ifndef OXYGEN_XFREECAMERA_H
#define OXYGEN_XFREECAMERA_H

#include "xVirtualCamera.h"

// ----------------------------------------------------------------------
//
// ----------------------------------------------------------------------

class xFreeCamera : public xVirtualCamera
{
public:

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    xFreeCamera(GLdouble angle, GLdouble front, GLdouble back) : xVirtualCamera(angle, front, back)
    {

    }

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual void Update(double elapsed)
    {
        m_elapsed = elapsed;

        // Count delta position by velocity and delta time
        // Warning: do not change sign of velocity
        // Count new position
        float velocity_to_elapsed = m_velocity * (float)m_elapsed;
        m_position->x += m_direction->x * (-velocity_to_elapsed);
        m_position->y += m_direction->y * (-velocity_to_elapsed);
        m_position->z += m_direction->z * (velocity_to_elapsed);

        if (!is_moving)
        {
            if (m_current_inertia_time < m_inertia_time)
            {
                float module = abs(m_velocity);

                if (module > 0.0) {
                    float sign = m_velocity / module;

                    m_velocity -= sign * m_attenuation_velocity;

                    if (sign * m_velocity < 0.0) {
                        m_velocity = 0.0;
                    }
                }

                m_current_inertia_time += m_elapsed;
            }
            else
            {
                m_velocity = 0.0;
            }
        }
    }

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual void Reset()
    {
        is_moving = false;
    }

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual void SetAttenuation(float attenuation)
    {
        if (attenuation > 0.0) {
            m_attenuation_velocity = attenuation;
        }
    }

    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    virtual void SetInertiaTime(float max_time)
    {
        if (max_time > 0.0) {
            m_inertia_time = max_time;
        }
    }

protected:

    bool is_moving;                     // Is camera moving in this time
    float m_inertia_time;               // Time in seconds which the camera continue to move forward by inertia
    float m_current_inertia_time;       // Current time after start of attenuation
    float m_attenuation_velocity;       // Attenuation of the camera velocity
    float m_velocity;       // Camera's velocity (moving forward)


};

#endif //OXYGEN_XFREECAMERA_H
