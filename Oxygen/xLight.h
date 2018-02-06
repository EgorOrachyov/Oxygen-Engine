/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 04.02.2018.
 * Copyright
 *
 * Structure xLight releases all necessary
 * methods for creating graphics light
 * sources (set-up ambient, diffuse, specular
 * and direction components)
 *
 * You can create 3 type of lights:
 * 1. Point Light Source
 * 2. Direct Spot Light Source
 * 3. Directed Parallel Light Source
 */

#ifndef OXYGEN_XLIGHT_H
#define OXYGEN_XLIGHT_H

#define LIGHT_TYPE_POINT    0
#define LIGHT_TYPE_SPOT     1
#define LIGHT_TYPE_DIRECTED 2

// ----------------------------------------------------------------------
// Light Source Class
// ----------------------------------------------------------------------

struct xLight
{
public:

    friend class xVirtualCamera;

    // ----------------------------------------------------------------------
    // Class Constructor from light type
    // ----------------------------------------------------------------------
    xLight(char light_type)
    {
        is_active = false;
        is_changed = true;
        m_type = light_type;
        m_position = new xArray4;
        m_direction = new xArray3;
        m_ambient = new xArray4;
        m_diffuse = new xArray4;
        m_specular = new xArray4;

        m_spot_exponent = 10.0;
        m_const_attenuation = 1.0;
        m_linear_attenuation = 0.4;
        m_quadratic_attenuation = 0.01;

        is_glare_effected = false;
        m_glow_scale = 1.0;
        m_big_glow_scale = 1.0;
        m_streaks_scale = 1.0;
        m_glow_color = new xArray4(0.8f, 0.8f, 1.0f, 0.5f);
        m_bih_glow_color = new xArray4(0.60f, 0.60f, 0.8f, 1.0f);
        m_streaks_color = new xArray4(0.60f, 0.60f, 0.8f, 1.0f);

        if (light_type == LIGHT_TYPE_DIRECTED) {

        } else if (light_type == LIGHT_TYPE_SPOT) {
            m_spot_cutoff = 15.0;
        } else if (light_type == LIGHT_TYPE_POINT) {

        } else {
            printf("ERROR: Wrong light type \n");
            exit(1);
        }
    }

    // ----------------------------------------------------------------------
    // Class Destructor
    // ----------------------------------------------------------------------
    ~xLight()
    {
        SAFE_DELETE(m_direction);
        SAFE_DELETE(m_position);
        SAFE_DELETE(m_ambient);
        SAFE_DELETE(m_diffuse);
        SAFE_DELETE(m_specular);
    }

    // ----------------------------------------------------------------------
    // Set ambient light component
    // ----------------------------------------------------------------------
    void SetAmbient(xVector4 * color)
    {
        m_ambient->VectorToArray(color);
        is_changed = true;
    }

    // ----------------------------------------------------------------------
    // Set diffuse light component
    // ----------------------------------------------------------------------
    void SetDiffuse(xVector4 * color)
    {
        m_diffuse->VectorToArray(color);
        is_changed = true;
    }

    // ----------------------------------------------------------------------
    // Set Specular light component
    // ----------------------------------------------------------------------
    void SetSpecular(xVector4 * color)
    {
        m_specular->VectorToArray(color);
        is_changed = true;
    }

    // ----------------------------------------------------------------------
    // Set light position
    // ----------------------------------------------------------------------
    void SetPosition(xVector4 * position)
    {
        m_position->VectorToArray(position);
        is_changed = true;
    }

    // ----------------------------------------------------------------------
    // Set light direction (only for directed and spot)
    // ----------------------------------------------------------------------
    void SetDirection(xVector3 * direction)
    {
        if (m_type == LIGHT_TYPE_DIRECTED) {
            m_position->values[0] = direction->x;
            m_position->values[1] = direction->y;
            m_position->values[2] = direction->z;
            m_position->values[3] = 0.0;
        } else {
            m_direction->VectorToArray(direction);
        }
        is_changed = true;
    }

    // ----------------------------------------------------------------------
    // Set angle between main axis and border (only for spot)
    // ----------------------------------------------------------------------
    void SetSpotCutOff(float spot_cutoff)
    {
        if (spot_cutoff > 0.0 && spot_cutoff < 90.0) {
            m_spot_cutoff = spot_cutoff;
            is_changed = true;
        }
    }

    // ----------------------------------------------------------------------
    // Set light index (need for OpenGL)
    // ----------------------------------------------------------------------
    void SetLightIndex(GLenum ingex)
    {
        if (ingex >= 0) {
            m_index = ingex;
            is_changed = true;
        }
    }

    // ----------------------------------------------------------------------
    // Returns true if light active on the render scene
    // ----------------------------------------------------------------------
    bool IsActive()
    {
        return  is_active;
    }

    // ----------------------------------------------------------------------
    // Set light active
    // ----------------------------------------------------------------------
    void SetActive(bool active)
    {
        is_active = active;
        is_changed = true;
    }

    // ----------------------------------------------------------------------
    // Set attenuation (for point light)
    // ----------------------------------------------------------------------
    void SetAttenuation(float const_att, float linear_att, float quad_att)
    {
        m_const_attenuation = const_att;
        m_linear_attenuation = linear_att;
        m_quadratic_attenuation = quad_att;
        is_changed = true;
    }

    // ----------------------------------------------------------------------
    // Set attenuation (for spot light)
    // ----------------------------------------------------------------------
    void SetExponentAttenuation(float exp_at)
    {
        m_spot_exponent = exp_at;
        is_changed = true;
    }

    // ----------------------------------------------------------------------
    // (Applies setting before each rendering)
    // ----------------------------------------------------------------------
    void ApplySettingToRenderer()
    {
        if (is_changed && is_active)
        {
            // To avoid repeat of this operations
            // in the next render cycle
            is_changed = false;

            // Common setting for all sources
            glLightfv(m_index, GL_AMBIENT, m_ambient->values);
            glLightfv(m_index, GL_DIFFUSE, m_diffuse->values);
            glLightfv(m_index, GL_SPECULAR, m_specular->values);
            glLightfv(m_index, GL_POSITION, m_position->values);

            if (m_type == LIGHT_TYPE_POINT)
            {   // Only for pint light
                glLightf(m_index, GL_CONSTANT_ATTENUATION, m_const_attenuation);
                glLightf(m_index, GL_LINEAR_ATTENUATION, m_linear_attenuation);
                glLightf(m_index, GL_QUADRATIC_ATTENUATION, m_quadratic_attenuation);
            }
            else if (m_type == LIGHT_TYPE_SPOT)
            {   // Only for spot light
                glLightfv(m_index, GL_SPOT_DIRECTION, m_direction->values);
                glLightfv(m_index, GL_SPOT_CUTOFF, &m_spot_cutoff);
            }
            else if (m_type == LIGHT_TYPE_DIRECTED)
            {   // Only for direct light
                glLightf(m_index, GL_SPOT_EXPONENT, m_spot_exponent);
            }
        }
    }

    // ----------------------------------------------------------------------
    // Set Glare Effect for camera active or not
    // ----------------------------------------------------------------------
    void SetGlareEffect(bool is_effected)
    {
        is_glare_effected = is_effected;
    }

    // ----------------------------------------------------------------------
    // Is Glare Effected
    // ----------------------------------------------------------------------
    bool IsGlareEffected()
    {
        return is_glare_effected;
    }

    // ----------------------------------------------------------------------
    // Sets small glow params (for camera effect)
    // ----------------------------------------------------------------------
    void SetGlow(xArray3 * color, float scale)
    {
        m_glow_scale = scale;
        m_glow_color->values[0] = color->values[0];
        m_glow_color->values[1] = color->values[1];
        m_glow_color->values[2] = color->values[2];
    }

    // ----------------------------------------------------------------------
    // Ssts Streaks params (for camera effect)
    // ----------------------------------------------------------------------
    void SetStreaks(xArray3 * color, float scale)
    {
        m_streaks_scale = scale;
        m_streaks_color->values[0] = color->values[0];
        m_streaks_color->values[1] = color->values[1];
        m_streaks_color->values[2] = color->values[2];
    }

    // ----------------------------------------------------------------------
    // Sets Big Glow params (for camera effect)
    // ----------------------------------------------------------------------
    void SetBigGlow(xArray3 * color, float scale)
    {
        m_big_glow_scale = scale;
        m_bih_glow_color->values[0] = color->values[0];
        m_bih_glow_color->values[1] = color->values[1];
        m_bih_glow_color->values[2] = color->values[2];
    }

    // ----------------------------------------------------------------------
    // Returns Light source position (actual only for spot and point)
    // ----------------------------------------------------------------------
    xArray4 * GetPosition()
    {
        return m_position;
    }

private:

    GLenum m_index;            // Index of the light for Open GL
    char m_type;               // Type of the Light Source
    bool is_active;            // Is Light Source active
    bool is_changed;           // Shows us, was params changed after last set up
    bool is_glare_effected;    // Has or not glare effect for camera
    float m_spot_cutoff;       // Angle between spot and main axis

    xArray4 * m_position;      // Light position (global)
    xArray3 * m_direction;     // Light direction (global)
    xArray4 * m_ambient;       // Ambient Light color
    xArray4 * m_diffuse;       // Diffuse Light color
    xArray4 * m_specular;      // Specular Light color

    float m_const_attenuation;      // Const attenuation for point light
    float m_linear_attenuation;     // Linear attenuation for point light
    float m_quadratic_attenuation;  // Quadratic attenuation for linear light
    float m_spot_exponent;          // Exponent attenuation for spot light

    float m_glow_scale;             // Customisable small glow size (camera effect)
    float m_streaks_scale;          // Customisable streaks size (camera effect)
    float m_big_glow_scale;         // Customisable big glow size (camera effect)
    xArray4 * m_glow_color;         // Color for small glow in the center of the light (camera effect)
    xArray4 * m_streaks_color;      // Color for 4 streaks (camera effect)
    xArray4 * m_bih_glow_color;     // Color for big glow (camera effect)

};


#endif //OXYGEN_XLIGHT_H
