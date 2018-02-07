/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 07.02.2018.
 * Copyright
 *
 *
 */

#ifndef OXYGEN_XMODELLOADER_H
#define OXYGEN_XMODELLOADER_H


class xModelLoader {

public:

    xModelLoader();
    ~xModelLoader();

    // Вы будете вызывать только эту функцию. Просто передаёте структуру
    // модели для сохранения данных, и имя файла для загрузки.
    void ImportObj(xModel3d * pModel, char * strFileName);

    // Так как .obj файлы не хранят имен текстур и информации о материалах, мы создадим
    // функцию, устанавливающую их вручную. materialID - индекс для массива pMaterial нашей модели.
    void SetObjectMaterial(xModel3d * pModel, int whichObject, int materialID);

    // Чтобы проще присваивать материал к .obj обьекту, создадим для этого функцию.
    // Передаём в неё модель, имя материала, имя файла текстуры и цвет RGB.
    // Если нам нужен только цвет, передаём NULL для strFile.
    void AddMaterial(xModel3d *pModel, char *strName, char *strFile);

protected:

    // Главный загружающий цикл, вызывающийся из ImportObj()
    void ReadObjFile(xModel3d *pModel);

    // Вызывается в ReadObjFile() если линия начинается с 'v'
    void ReadVertexInfo();

    // Вызывается в ReadObjFile() если линия начинается с 'f'
    void ReadFaceInfo();

    // Вызывается после загрузки информации полигонов
    void FillInObjectInfo(xModel3d *pModel);

private:

    long vertex_offset;
    long texture_offset;
    long normal_offset;

    long vertex_offset_prev;
    long texture_offset_prev;
    long normal_offset_prev;

    bool m_bObjectHasUV;                        // If object has texture coordinates
    bool m_bObjectHasNormals;                   // If object has normak vectors
    bool m_bJustReadAFace;                      // If object is read and we need to write data in the xObject

    FILE * m_FilePointer;                       // Pointer to opening model file

    xDynamicArray<xPoint3>  m_pVertices;        // Object vertices
    xDynamicArray<xPoint2>  m_pTextureCoords;   // Object texture coordinates
    xDynamicArray<xPoint3> m_pNormals;          // Object normals
    xDynamicArray<xFace> m_pFaces;              // Object faces

};


#endif //OXYGEN_XMODELLOADER_H
