/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 07.02.2018.
 * Copyright
 *
 * Realisation of functions defined in the file
 * xModelLoader.h. Go there to find more information
 * and interface specifications
 */

#include "xEngine.h"

xModelLoader::xModelLoader()
{
    m_bObjectHasUV = false;
    m_bObjectHasNormals = false;
    m_bJustReadAFace = false;

    vertex_offset = 0;
    texture_offset = 0;
    normal_offset = 0;

    vertex_offset_prev = 0;
    texture_offset_prev = 0;
    normal_offset_prev = 0;
}

xModelLoader::~xModelLoader()
{

}

void xModelLoader::ImportObj(xModel3d *pModel, char *strFileName)
{
    if (!pModel || !strFileName) {
        printf("ERROR: Model or file \"%s\" name have wrong format \n", strFileName);
        exit(1);
    }

    m_FilePointer = fopen(strFileName, "r");
    if (!m_FilePointer) {
        printf("ERROR: Cannot open file %s \n", strFileName);
        exit(1);
    }

    ReadObjFile(pModel);
    fclose(m_FilePointer);

    vertex_offset = 0;
    texture_offset = 0;
    normal_offset = 0;

    vertex_offset_prev = 0;
    texture_offset_prev = 0;
    normal_offset_prev = 0;
}

void xModelLoader::ReadObjFile(xModel3d *pModel)
{
    char strLine[STRING_SIZE] = {0};
    char ch  = 0;

    while(!feof(m_FilePointer))
    {
        float x = 0.0f, y = 0.0f, z = 0.0f;

        ch = fgetc(m_FilePointer);
        switch(ch)
        {
            // v - vertexes, vt - texture coordinates, vn - normals
            case 'v':

                // If we stopped read data about previous object
                // we write it int the xObject
                if(m_bJustReadAFace) {
                    FillInObjectInfo(pModel);
                }

                ReadVertexInfo();
                break;

            // f - faces
            case 'f':
                ReadFaceInfo();
                break;
            default:
                fgets(strLine, 100, m_FilePointer);
                break;
        }
    }

    // End file reading, write data in the model
    FillInObjectInfo(pModel);
}

void xModelLoader::ReadVertexInfo()
{
    float x = 0, y = 0, z = 0;
    char strLine[STRING_SIZE] = {0};
    char ch = 0;

    ch = fgetc(m_FilePointer);

    // then read vertex coordinates "v x y z"
    if(ch == ' ') {
        fscanf(m_FilePointer, "%f %f %f", &x, &y, &z);
        fgets(strLine, 100, m_FilePointer);
        m_pVertices.Add(new xPoint3(x, y, z));
        vertex_offset += 1;
    }
    // then read texture coordinates "vt x y"
    else if(ch == 't')
    {
        fscanf(m_FilePointer, "%f %f", &x, &y);
        fgets(strLine, 100, m_FilePointer);
        m_pTextureCoords.Add(new xPoint2(x, y));
        m_bObjectHasUV = true;
        texture_offset += 1;
    }
    // then read normal coordinates "vn x y z"
    else if (ch == 'n') {
        fscanf(m_FilePointer, "%f %f %f", &x, &y, &z);
        fgets(strLine, 100, m_FilePointer);
        m_pNormals.Add(new xPoint3(x, y, z));
        m_bObjectHasNormals = true;
        normal_offset += 1;
    }
    // then skip the line
    else
    {
        fgets(strLine, 100, m_FilePointer);
    }
}

void xModelLoader::ReadFaceInfo()
{
    xFace * face = new xFace;
    char strLine[STRING_SIZE];
    int scanned = 0;

    if (m_bObjectHasNormals && m_bObjectHasUV) {
        scanned = fscanf(m_FilePointer, "%li/%li/%li %li/%li/%li %li/%li/%li",
               &face->vertex[0], &face->texture[0], &face->normal[0],
               &face->vertex[1], &face->texture[1], &face->normal[1],
               &face->vertex[2], &face->texture[2], &face->normal[2]);

        if (scanned < 9) {
            printf("ERROR: Mismatched number of scanned params");
            exit(1);
        }
    }
    else if (m_bObjectHasUV) {
        scanned = fscanf(m_FilePointer, "%li/%li %li/%li %li/%li",
               &face->vertex[0], &face->texture[0],
               &face->vertex[1], &face->texture[1],
               &face->vertex[2], &face->texture[2]);

        if (scanned < 6) {
            printf("ERROR: Mismatched number of scanned params");
            exit(1);
        }
    }
    else if (m_bObjectHasNormals) {
        scanned = fscanf(m_FilePointer, "%li//%li %li//%li %li//%li",
               &face->vertex[0], &face->normal[0],
               &face->vertex[1], &face->normal[1],
               &face->vertex[2], &face->normal[2]);

        if (scanned < 6) {
            printf("ERROR: Mismatched number of scanned params");
            exit(1);
        }
    }
    else {
        scanned = fscanf(m_FilePointer, "%li %li %li",
               &face->vertex[0],
               &face->vertex[1],
               &face->vertex[2]);

        if (scanned < 3) {
            printf("ERROR: Mismatched number of scanned params");
            exit(1);
        }
    }

    m_pFaces.Add(face);
    fgets(strLine, 100, m_FilePointer);
    m_bJustReadAFace = true;
}

void xModelLoader::FillInObjectInfo(xModel3d *pModel)
{
    xObject3d * pObject = new xObject3d;
    pModel->m_objects->Add(pObject);
    pModel->num_objects += 1;

    pObject->num_vertexes = m_pVertices.GetNumOfElements();
    pObject->num_textcords = m_pTextureCoords.GetNumOfElements();
    pObject->num_normals = m_pNormals.GetNumOfElements();
    pObject->num_faces = m_pFaces.GetNumOfElements();

    for(long i = 0; i < pObject->num_vertexes; i++) {
        pObject->m_vertexes->Add(m_pVertices.GetElement(i));
    }

    for(long i = 0; i < pObject->num_textcords; i++) {
        pObject->m_textcords->Add(m_pTextureCoords.GetElement(i));
    }

    for(long i = 0; i < pObject->num_normals; i++) {
        pObject->m_normals->Add(m_pNormals.GetElement(i));
    }

    for(long i = 0; i < pObject->num_faces; i++) {
        pObject->m_faces->Add(m_pFaces.GetElement(i));

        for(int j = 0; j < 3; j++) {
            m_pFaces.GetElement(i)->vertex[j] -= 1 + vertex_offset_prev;
            m_pFaces.GetElement(i)->texture[j] -= 1 + texture_offset_prev;
            m_pFaces.GetElement(i)->normal[j] -= 1 + normal_offset_prev;
        }
    }

    m_pVertices.Clear();
    m_pTextureCoords.Clear();
    m_pNormals.Clear();
    m_pFaces.Clear();

    m_bObjectHasUV = false;
    m_bObjectHasNormals = false;
    m_bJustReadAFace = false;

    vertex_offset_prev += vertex_offset;
    texture_offset_prev += texture_offset;
    normal_offset_prev += normal_offset;

    vertex_offset = 0;
    texture_offset = 0;
    normal_offset = 0;
}

void xModelLoader::SetObjectMaterial(xModel3d *pModel, int whichObject, int materialID)
{

}

void xModelLoader::AddMaterial(xModel3d *pModel, char *strName, char *strFile)
{

}