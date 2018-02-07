/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 07.02.2018.
 * Copyright
 *
 *
 */

#ifndef OXYGEN_XMODEL3D_H
#define OXYGEN_XMODEL3D_H


class xObject3d
{
public:

    friend class xModel3d;
    friend class xModelLoader;

    xObject3d()
    {
        is_active = true;
        m_MaterialId = -1;
        m_texture = NULL;

        num_vertexes = 0;
        num_textcords = 0;
        num_normals = 0;
        num_faces = 0;

        m_vertexes = new xDynamicArray<xPoint3>;
        m_textcords = new xDynamicArray<xPoint2>;
        m_normals = new xDynamicArray<xPoint3>;
        m_faces = new xDynamicArray<xFace>;
    }

    ~xObject3d()
    {
        SAFE_DELETE(m_vertexes);
        SAFE_DELETE(m_textcords);
        SAFE_DELETE(m_normals);
        SAFE_DELETE(m_faces);
        SAFE_DELETE(m_texture);
    }

    void LoadTexture(char * name, char * path)
    {
        m_texture = new xTexture(name, path);
    }

    void Render(xMaterial * material)
    {
        if (is_active)
        {
            if (m_texture != NULL) {
                glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureID());
            }

            if (num_normals && num_textcords) {
                for(long i = 0; i < num_faces; i++) {

                    glBegin(GL_TRIANGLES);

                    for(int j = 0; j < 3; j++) {

                        glTexCoord2f(m_textcords->GetElement(m_faces->GetElement(i)->texture[j])->x,
                                     m_textcords->GetElement(m_faces->GetElement(i)->texture[j])->y);

                        glNormal3f(m_normals->GetElement(m_faces->GetElement(i)->normal[j])->x,
                                   m_normals->GetElement(m_faces->GetElement(i)->normal[j])->y,
                                   m_normals->GetElement(m_faces->GetElement(i)->normal[j])->z);

                        glVertex3f(m_vertexes->GetElement(m_faces->GetElement(i)->vertex[j])->x,
                                   m_vertexes->GetElement(m_faces->GetElement(i)->vertex[j])->y,
                                   m_vertexes->GetElement(m_faces->GetElement(i)->vertex[j])->z);
                    }

                    glEnd();
                }
            }
            else if (num_textcords) {
                for(long i = 0; i < num_faces; i++) {

                    glBegin(GL_TRIANGLES);

                    for(int j = 0; j < 3; j++) {

                        glTexCoord2f(m_textcords->GetElement(m_faces->GetElement(i)->texture[j])->x,
                                     m_textcords->GetElement(m_faces->GetElement(i)->texture[j])->y);

                        glVertex3f(m_vertexes->GetElement(m_faces->GetElement(i)->vertex[j])->x,
                                   m_vertexes->GetElement(m_faces->GetElement(i)->vertex[j])->y,
                                   m_vertexes->GetElement(m_faces->GetElement(i)->vertex[j])->z);
                    }

                    glEnd();
                }
            }
            else if (num_normals) {
                for(long i = 0; i < num_faces; i++) {

                    glBegin(GL_TRIANGLES);

                    for(int j = 0; j < 3; j++) {

                        glNormal3f(m_normals->GetElement(m_faces->GetElement(i)->normal[j])->x,
                                   m_normals->GetElement(m_faces->GetElement(i)->normal[j])->y,
                                   m_normals->GetElement(m_faces->GetElement(i)->normal[j])->z);

                        glVertex3f(m_vertexes->GetElement(m_faces->GetElement(i)->vertex[j])->x,
                                   m_vertexes->GetElement(m_faces->GetElement(i)->vertex[j])->y,
                                   m_vertexes->GetElement(m_faces->GetElement(i)->vertex[j])->z);
                    }

                    glEnd();
                }
            }
            else {
                for(long i = 0; i < num_faces; i++) {

                    glBegin(GL_TRIANGLES);

                    for(int j = 0; j < 3; j++) {

                        glVertex3f(m_vertexes->GetElement(m_faces->GetElement(i)->vertex[j])->x,
                                   m_vertexes->GetElement(m_faces->GetElement(i)->vertex[j])->y,
                                   m_vertexes->GetElement(m_faces->GetElement(i)->vertex[j])->z);
                    }

                    glEnd();
                }
            }

        }
    }

private:

    long num_vertexes;      //
    long num_textcords;     //
    long num_normals;       //
    long num_faces;         //

    bool is_active;             // Should be rendered or not
    long m_MaterialId;          // Material ID
    char m_name[STRING_SIZE];   // Object name

    xTexture * m_texture;                    // Texture for model
    xDynamicArray<xPoint3> * m_vertexes;     // Array of vertexes
    xDynamicArray<xPoint2> * m_textcords;    // Array of texture coordinates
    xDynamicArray<xPoint3> * m_normals;      // Array of normal vectors
    xDynamicArray<xFace>   * m_faces;        // Array of faces (triangles) of object

};


class xModel3d
{
public:

    friend class xModelLoader;

    xModel3d()
    {
        is_active = true;

        num_objects = 0;
        num_materials = 0;

        m_objects = new xDynamicArray<xObject3d>;
        m_materials = new xDynamicArray<xMaterial>;
    }

    ~xModel3d()
    {
        SAFE_DELETE(m_objects);
        SAFE_DELETE(m_materials);
    }

    void Render()
    {
        if (is_active)
        {
            for(long i = 0; i < num_objects; i++) {
                m_objects->GetElement(i)->Render(m_materials->GetElement(m_objects->GetElement(i)->m_MaterialId));
            }
        }
    }

private:

    bool is_active;         //

    long num_objects;       //
    long num_materials;     //

    xDynamicArray<xObject3d> * m_objects;       //
    xDynamicArray<xMaterial> * m_materials;     //

};


#endif //OXYGEN_XMODEL3D_H
