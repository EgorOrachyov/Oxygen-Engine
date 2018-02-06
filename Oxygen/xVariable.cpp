/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 25.01.2018.
 * Copyright
 *
 * Realisation of functions defined in the file
 * xVariable.h. Go there to find more information
 * and interface specifications
 */

#include "xEngine.h"

xVariable::xVariable(char * type, FILE * file)
{
    // Проверяем, что указатель на файл существует и верен.
    if(file == NULL) {
        printf("ERROR: FILE hase wrong format \n");
        exit(1);
    }

    // Читаем имя переменной.
    char buffer[STRING_SIZE];
    fscanf(file, "%s", buffer);

    // Сохраняем имя переменной
    m_name = new char[strlen(buffer) + 1];
    strcpy(m_name, buffer);

    strcpy(buffer, type);

    if(strcmp(buffer, "bool") == 0)
    {
        // Переменная булева типа (BOOL).
        m_type = VARIABLE_BOOL;

        // Читаем и устанавливаем булево значение переменной.
        bool value;
        fscanf(file, "%s", buffer);
        fscanf(file, "%s", buffer);

        if(strcmp(buffer, "true") == 0)
            value = true;
        else
            value = false;
        m_data = new bool;
        memcpy(m_data, &value, sizeof(bool));
    }
    else if(strcmp(buffer, "float") == 0)
    {
        // Переменная типа float (с плавающей точкой).
        m_type = VARIABLE_FLOAT;

        // Читаем и устанавливаем переменную типа float.
        float value;
        fscanf(file, "%s", buffer);
        fscanf(file, "%s", buffer);

        value = (float)atof(buffer);
        m_data = new float;
        memcpy(m_data, &value, sizeof(float));
    }
    else if(strcmp(buffer, "number") == 0)
    {
        // Переменная является числом.
        m_type = VARIABLE_NUMBER;

        // Читаем и устанавливаем числовую (целочисленную) перемнную.
        long value;
        fscanf(file, "%s", buffer);
        fscanf(file, "%s", buffer);

        value = atol(buffer);
        m_data = new long;
        memcpy(m_data, &value, sizeof(long));
    }
    else if(strcmp(buffer, "string") == 0)
    {
        // Переменная является строкой (тип string).
        m_type = VARIABLE_STRING;
        fscanf(file, "%s", buffer);


        // Ищем открывающие обратные слэши (начало блока переменных).
        bool commasFound = false;
        //ZeroMemory(buffer, STRING_SIZE * sizeof(char));
        fscanf(file, "%c", buffer);
        while(true)
        {
            if(strcmp(buffer, "\"") == 0)
            {
                commasFound = true;
                break;
            }

            if(strcmp(buffer, " ") != 0)
            {
                fpos_t pos;
                fgetpos(file, &pos);
                fsetpos(file, &--pos);
                break;
            }

            fscanf(file, "%c", buffer);
        }

        // Читаем и устанавливаем переменную типа string (строка).
        char completeString[STRING_SIZE];
        //ZeroMemory(completeString, STRING_SIZE * sizeof(char));
        bool addSpacing = false;
        do
        {
            fscanf(file, "%s", buffer);
            if(strcmp(&buffer[strlen(buffer) - 1], "\"") == 0)
            {
                buffer[strlen(buffer) - 1] = 0;
                commasFound = false;
            }

            if(addSpacing == false)
                addSpacing = true;
            else
                strcat(completeString, " ");

            strcat(completeString, buffer);
        } while(commasFound == true);

        m_data = new char[strlen(completeString) + 1];
        strcpy((char*)m_data, completeString);
    }
    else if(strcmp(buffer, "vec2") == 0)
    {
        // Переменная является вектором (тип vector).
        m_type = VARIABLE_VEC2;
        fscanf(file, "%s", buffer);
        
        // Читаем и устанавливаем переменную типа vector.
        xVector2 vector;
        fscanf(file, "%s", buffer);
        vector.x = (float)atof(buffer);
        fscanf(file, "%s", buffer);
        vector.y = (float)atof(buffer);
        m_data = new xVector2;
        memcpy(m_data, &vector, sizeof(xVector2));
    }
    else if(strcmp(buffer, "vec3") == 0) {
        // Переменная является вектором (тип vector).
        m_type = VARIABLE_VEC3;
        fscanf(file, "%s", buffer);

        // Читаем и устанавливаем переменную типа vector.
        xVector3 vector;
        fscanf(file, "%s", buffer);
        vector.x = (float) atof(buffer);
        fscanf(file, "%s", buffer);
        vector.y = (float) atof(buffer);
        fscanf(file, "%s", buffer);
        vector.z = (float) atof(buffer);
        m_data = new xVector3;
        memcpy(m_data, &vector, sizeof(xVector3));
    }
    else if(strcmp(buffer, "vec4") == 0) {
        // Переменная является вектором (тип vector).
        m_type = VARIABLE_VEC4;
        fscanf(file, "%s", buffer);

        // Читаем и устанавливаем переменную типа vector.
        xVector4 vector;
        fscanf(file, "%s", buffer);
        vector.x = (float) atof(buffer);
        fscanf(file, "%s", buffer);
        vector.y = (float) atof(buffer);
        fscanf(file, "%s", buffer);
        vector.z = (float) atof(buffer);
        fscanf(file, "%s", buffer);
        vector.w = (float) atof(buffer);
        m_data = new xVector3;
        memcpy(m_data, &vector, sizeof(xVector4));
    }
    else
    {
        // Переменная неизвестного типа (unknown).
        m_type = VARIABLE_UNKNOWN;
        fscanf(file, "%s", buffer);

        // Читаем и устанавливаем данные (также как для типа string) для переменной.
        fscanf(file, "%s", buffer);
        m_data = new char[strlen(buffer) + 1];
        strcpy((char*)m_data, buffer);
    }
}

xVariable::xVariable(char * name, char type, void * value)
{
    // Сохраняем имя переменной.
    m_name = new char[strlen(name) + 1];
    strcpy(m_name, name);

    // Сохраняем тип переменной.
    m_type = type;

    // Устанавливаем данные переменной, в зависимости от её типа.
    switch(m_type)
    {
        case VARIABLE_BOOL:
            m_data = new bool;
            memcpy(m_data, (bool*)value, sizeof(bool));
            return;

        case VARIABLE_FLOAT:
            m_data = new float;
            memcpy(m_data, (float*)value, sizeof(float));
            return;

        case VARIABLE_NUMBER:
            m_data = new long;
            memcpy(m_data, (long*)value, sizeof(long));
            return;

        case VARIABLE_STRING:
            m_data = new char[strlen((char*)value) + 1];
            strcpy((char*)m_data, (char*)value);
            return;

        case VARIABLE_VEC2:
            m_data = new xVector2;
            memcpy(m_data, (xVector2*)value, sizeof(xVector2));
            return;

        case VARIABLE_VEC3:
            m_data = new xVector3;
            memcpy(m_data, (xVector3*)value, sizeof(xVector3));
            return;

        case VARIABLE_VEC4:
            m_data = new xVector4;
            memcpy(m_data, (xVector4*)value, sizeof(xVector4));
            return;

        default:
            m_data = new char[strlen((char*)value) + 1];
            strcpy((char*)m_data, (char*)value);
            return;
    }
}

xVariable::~xVariable()
{
    SAFE_DELETE_ARRAY(m_name);
    SAFE_DELETE(m_data);
}

char xVariable::GetType()
{
    return m_type;
}

char * xVariable::GetName()
{
    return m_name;
}

void * xVariable::GetData()
{
    switch(m_type)
    {
        case VARIABLE_BOOL:
            return (bool *)m_data;

        case VARIABLE_FLOAT:
            return (float *)m_data;

        case VARIABLE_NUMBER:
            return (long *)m_data;

        case VARIABLE_STRING:
            return (char *)m_data;

        case VARIABLE_VEC2:
            return (xVector2 *)m_data;

        case VARIABLE_VEC3:
            return (xVector3 *)m_data;

        case VARIABLE_VEC4:
            return (xVector4 *)m_data;

        default:
            return m_data;
    }
}