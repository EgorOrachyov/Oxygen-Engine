/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 07.02.2018.
 * Copyright
 *
 *
 */

#ifndef OXYGEN_XMATERIAL_H
#define OXYGEN_XMATERIAL_H

struct xMaterial
{
public:

    friend class xModelLoader;

    xMaterial()
    {
        m_ambient = new xArray3;
        m_diffuse = new xArray3;
        m_specular = new xArray3;
        m_emission = new xArray3;
    }

    ~xMaterial()
    {
        SAFE_DELETE(m_ambient);
        SAFE_DELETE(m_diffuse);
        SAFE_DELETE(m_specular);
        SAFE_DELETE(m_emission);
    }

    void ApplyMaterial()
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient->values);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse->values);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular->values);
        glMaterialfv(GL_FRONT, GL_EMISSION, m_emission->values);
        glMaterialf(GL_FRONT, GL_SHININESS, m_shininess);
    }

    void SetAmbient(float x, float y, float z)
    {
        m_ambient->values[0] = x;
        m_ambient->values[1] = y;
        m_ambient->values[2] = z;
    }

    void SetDiffuse(float x, float y, float z)
    {
        m_diffuse->values[0] = x;
        m_diffuse->values[1] = y;
        m_diffuse->values[2] = z;
    }

    void SetSpecular(float x, float y, float z)
    {
        m_specular->values[0] = x;
        m_specular->values[1] = y;
        m_specular->values[2] = z;
    }

    void SetEmission(float x, float y, float z)
    {
        m_emission->values[0] = x;
        m_emission->values[1] = y;
        m_emission->values[2] = z;
    }

    void SetShininess(int s)
    {
        if (s >= 0 && s <= 128){
            m_shininess = s;
        }
    }

private:

    char materialName[STRING_SIZE];    // Full material name and path
    int m_shininess;                   // Material shininess 0..128
    xArray3 * m_ambient;               // Ambient component
    xArray3 * m_diffuse;               // Diffuse component
    xArray3 * m_specular;              // Specular component
    xArray3 * m_emission;              // Emission component

};

#endif //OXYGEN_XMATERIAL_H
