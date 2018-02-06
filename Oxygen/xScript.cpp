/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 01.02.2018.
 * Copyright
 *
 * Realisation of functions defined in the file
 * xScript.h. Go there to find more information
 * and interface specifications
 */

#include "xEngine.h"

xScript::xScript(char * name, char * path) : xResource(name, path)
{
    // Создаём связный список (linked list) в котором будут храниться все переменные скрипта.
    m_variables = new xLinkedList<xVariable>;

    // Открываем скрипт, указав имя его файла (filename).
    FILE * file = NULL;
    if ((file = fopen(GetFilename(), "r")) == NULL)
        return;

    // Продолжаем чтение файла, пока не достигнем eof (end of file = конец файла).
    bool read = false;
    char buffer[STRING_SIZE];
    fscanf(file, "%s", buffer);
    while(feof(file) == 0)
    {
        // Проверяем, находится ли индикатор позиции файла между тегами #begin и #end.
        // Если да, то читаем данные в связный список переменных.
        if (read == true)
        {
            // Останавливаем чтение данных, если встретили тег #end.
            if (strcmp(buffer, "end") == 0) {
                read = false;
            }
            else {
                if (strcmp(buffer, "float") == 0 ||
                    strcmp(buffer, "bool") == 0 ||
                    strcmp(buffer, "number") == 0 ||
                    strcmp(buffer, "string") == 0 ||
                    strcmp(buffer, "unknown") == 0 ||
                    strcmp(buffer, "vec2") == 0 ||
                    strcmp(buffer, "vec3") == 0 ||
                    strcmp(buffer, "vec4") == 0) {
                    char * type = buffer;
                    m_variables->Add(new xVariable(type, file));
                }

            }
        }
        else if (strcmp(buffer, "begin") == 0)
            read = true;

        // Читаем следующую строку.
        fscanf(file, "%s", buffer);
    }

    // Закрываем файл скрипта.
    fclose(file);
}

//-----------------------------------------------------------------------------
// The xScript class destructor.
//-----------------------------------------------------------------------------
xScript::~xScript()
{
    SAFE_DELETE(m_variables);
}

//-----------------------------------------------------------------------------
// Добавляет новую переменную в скрипт.
//-----------------------------------------------------------------------------
void xScript::AddVariable(char * name, char type, void * value)
{
    m_variables->Add(new xVariable(name, type, value));
}

//-----------------------------------------------------------------------------
// Устанавливает значение существующей переменной в скрипте.
//-----------------------------------------------------------------------------
void xScript::SetVariable(char * name, void * value)
{
    // Ищем переменную.
    xVariable * variable = NULL;
    m_variables->Iterate(true);
    while(m_variables->Iterate() != NULL)
    {
        if (strcmp(m_variables->GetCurrent()->GetName(), name) == 0)
        {
            variable = m_variables->GetCurrent();
            break;
        }
    }

    // Проверяем, если переменная не была найдена.
    if (variable == NULL)
        return;

    // Получаем тип переменной.
    char type = variable->GetType();

    // Уничтожаем переменную.
    m_variables->Remove(variable);

    // Добавляем переменную на прежнее место с новым значением.
    AddVariable(name, type, value);
}

//-----------------------------------------------------------------------------
// Сохраняет скрипт в файл.
//-----------------------------------------------------------------------------
void xScript::SaveScript(char * filename)
{
    FILE * file = NULL;
    char output[STRING_SIZE];

    // Открываем указанное имя файла, если таковое имеется. В противном случае используем внутреннее имя файла.
    if (filename != NULL)
    {
        if ((file = fopen(filename, "w")) == NULL)
            return;
    }
    else
    {
        if ((file = fopen(GetFilename(), "w")) == NULL)
            return;
    }

    // Пишем тег #begin в файл.
    fputs("begin\n", file);

    // Пишем каждую переменную в файл.
    m_variables->Iterate(true);
    while(m_variables->Iterate() != NULL)
    {
        switch(m_variables->GetCurrent()->GetType())
        {
            case VARIABLE_BOOL:
                if (*((bool *)m_variables->GetCurrent()->GetData()) == true)
                    sprintf(output, "bool %s = true", m_variables->GetCurrent()->GetName());
                else
                    sprintf(output, "bool %s = false", m_variables->GetCurrent()->GetName());
                fputs(output, file);
                fputs("\n", file);
                continue;

            case VARIABLE_FLOAT:
                sprintf(output, "float %s = %f", m_variables->GetCurrent()->GetName(), *(float *)m_variables->GetCurrent()->GetData());
                fputs(output, file);
                fputs("\n", file);
                continue;

            case VARIABLE_NUMBER:
                sprintf(output, "number %s = %li", m_variables->GetCurrent()->GetName(), *(long *)m_variables->GetCurrent()->GetData());
                fputs(output, file);
                fputs("\n", file);
                continue;

            case VARIABLE_STRING:
                sprintf(output, "string %s = \"%s\"", m_variables->GetCurrent()->GetName(), (char *)m_variables->GetCurrent()->GetData());
                fputs(output, file);
                fputs("\n", file);
                continue;

            case VARIABLE_VEC2:
                sprintf(output, "vec2 %s = %f %f", m_variables->GetCurrent()->GetName(),
                        ((xVector2 *)m_variables->GetCurrent()->GetData())->x,
                        ((xVector2 *)m_variables->GetCurrent()->GetData())->y);
                fputs(output, file);
                fputs("\n", file);
                continue;

            case VARIABLE_VEC3:
                sprintf(output, "vec3 %s = %f %f %f", m_variables->GetCurrent()->GetName(),
                        ((xVector3 *)m_variables->GetCurrent()->GetData())->x,
                        ((xVector3 *)m_variables->GetCurrent()->GetData())->y,
                        ((xVector3 *)m_variables->GetCurrent()->GetData())->z);
                fputs(output, file);
                fputs("\n", file);
                continue;

            case VARIABLE_VEC4:
                sprintf(output, "vec4 %s = %f %f %f %f", m_variables->GetCurrent()->GetName(),
                        ((xVector4 *)m_variables->GetCurrent()->GetData())->x,
                        ((xVector4 *)m_variables->GetCurrent()->GetData())->y,
                        ((xVector4 *)m_variables->GetCurrent()->GetData())->z,
                        ((xVector4 *)m_variables->GetCurrent()->GetData())->w);
                fputs(output, file);
                fputs("\n", file);
                continue;

            default:
                sprintf(output, "unknown %s = %s", m_variables->GetCurrent()->GetName(), (char *)m_variables->GetCurrent()->GetData());
                fputs(output, file);
                fputs("\n", file);
                continue;
        }
    }

    // Пишем тег end в файл.
    fputs("end", file);

    // Закрываем файл скрипта.
    fclose(file);
}

bool * xScript::GetBoolData(char * variable)
{
    m_variables->Iterate(true);
    while(m_variables->Iterate() != NULL)
        if (strcmp(m_variables->GetCurrent()->GetName(), variable) == 0)
            return (bool *)m_variables->GetCurrent()->GetData();

    return NULL;
}

float * xScript::GetFloatData(char * variable)
{
    m_variables->Iterate(true);
    while(m_variables->Iterate() != NULL)
        if (strcmp(m_variables->GetCurrent()->GetName(), variable) == 0)
            return (float *)m_variables->GetCurrent()->GetData();

    return NULL;
}

long * xScript::GetNumberData(char * variable)
{
    m_variables->Iterate(true);
    while(m_variables->Iterate() != NULL)
        if (strcmp(m_variables->GetCurrent()->GetName(), variable) == 0)
            return (long *)m_variables->GetCurrent()->GetData();

    return NULL;
}

char * xScript::GetStringData(char * variable)
{
    m_variables->Iterate(true);
    while(m_variables->Iterate() != NULL)
        if (strcmp(m_variables->GetCurrent()->GetName(), variable) == 0)
            return (char *)m_variables->GetCurrent()->GetData();

    return NULL;
}

xVector2 * xScript::GetVec2Data(char * variable)
{
    m_variables->Iterate(true);
    while(m_variables->Iterate() != NULL)
        if (strcmp(m_variables->GetCurrent()->GetName(), variable) == 0)
            return (xVector2 *)m_variables->GetCurrent()->GetData();

    return NULL;
}

xVector3 * xScript::GetVec3Data(char * variable)
{
    m_variables->Iterate(true);
    while(m_variables->Iterate() != NULL)
        if (strcmp(m_variables->GetCurrent()->GetName(), variable) == 0)
            return (xVector3 *)m_variables->GetCurrent()->GetData();

    return NULL;
}

xVector4 * xScript::GetVec4Data(char * variable)
{
    m_variables->Iterate(true);
    while(m_variables->Iterate() != NULL)
        if (strcmp(m_variables->GetCurrent()->GetName(), variable) == 0)
            return (xVector4 *)m_variables->GetCurrent()->GetData();

    return NULL;
}

void * xScript::GetUnknownData(char * variable)
{
    m_variables->Iterate(true);
    while(m_variables->Iterate() != NULL)
        if (strcmp(m_variables->GetCurrent()->GetName(), variable) == 0)
            return m_variables->GetCurrent()->GetData();

    return NULL;
}