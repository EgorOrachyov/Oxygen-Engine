/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 25.01.2018.
 * Copyright
 *
 * xResourceManager provides optimized safe
 * and useful loading of different assets
 * (such as sound, texture, mesh and object data).
 * One resource will be loaded only one time if
 * you tries to do it many times. Moreover, it has
 * pointer counter for each resource, therefore you
 * cannot delete it by a mistake
 */

#ifndef OXYGEN_XRESOURCEMANAGER_H
#define OXYGEN_XRESOURCEMANAGER_H

#include "xEngine.h"

// ----------------------------------------------------------------------
// Resource Class
// ----------------------------------------------------------------------

class xResource
{
public:
    // ----------------------------------------------------------------------
    // The resource class constructor
    // ----------------------------------------------------------------------
    xResource(char * name, char * path = NULL)
    {
        if (path == NULL) {
            char standard_way[] = "./";
            path = standard_way;
        }

        // save file name
        if (name != NULL)
        {
            m_name = new char[strlen(name) + 1];
            memcpy(m_name, name, (strlen(name) + 1) * sizeof(char));
        }

        // save file path
        if (path != NULL)
        {
            m_path = new char[strlen(path) + 1];
            memcpy(m_path, path, (strlen(path) + 1) * sizeof(char));
        }

        // full file name (with path)
        if (name != NULL && path != NULL)
        {
            m_filename = new char[strlen(name) + strlen(path) + 1];
            sprintf(m_filename, "%s%s", path, name);
        }

        // Starts the references counter
        m_refCount = 1;
    }

    // ----------------------------------------------------------------------
    // The resource class destructor
    // ----------------------------------------------------------------------
    virtual ~xResource()
    {
        SAFE_DELETE_ARRAY(m_name);
        SAFE_DELETE_ARRAY(m_path);
        SAFE_DELETE_ARRAY(m_filename);
    }

    // ----------------------------------------------------------------------
    // Returns the name of resource
    // ----------------------------------------------------------------------
    char * GetName()
    {
        return m_name;
    }

    // ----------------------------------------------------------------------
    // Returns the path of resource
    // ----------------------------------------------------------------------
    char * GetPath()
    {
        return m_path;
    }

    // ----------------------------------------------------------------------
    // Returns the full name and path of resource
    // ----------------------------------------------------------------------
    char * GetFilename()
    {
        return m_filename;
    }

    // ----------------------------------------------------------------------
    // Increments the references counter to 1
    // ----------------------------------------------------------------------
    void IncRef()
    {
        m_refCount += 1;
    }

    // ----------------------------------------------------------------------
    // Decrements the references counter to 1
    // ----------------------------------------------------------------------
    void DecRef()
    {
        m_refCount -= 1;
    }

    // ----------------------------------------------------------------------
    // Returns the references counter value
    // ----------------------------------------------------------------------
    unsigned long GetRefCount()
    {
        return m_refCount;
    }

private:
    char * m_name;                  // Name of the resource.
    char * m_path;                  // Path to the resource.
    char * m_filename;              // Filename (name + path) of the resource.
    unsigned long m_refCount;       // Reference count.
};

// ----------------------------------------------------------------------
// Resource Manager Class
// ----------------------------------------------------------------------

template <class Type> class xResourceManager
{
public:
    // ----------------------------------------------------------------------
    // The resource manager class constructor
    // ----------------------------------------------------------------------
    xResourceManager(void (* CreateResourceFunction)(Type * resource, char * name, char * path) = NULL)
    {
        m_list = new xLinkedList<Type>;
        CreateResource = CreateResourceFunction;
    }

    // ----------------------------------------------------------------------
    // The resource manager class destructor
    // ----------------------------------------------------------------------
    ~xResourceManager()
    {
        SAFE_DELETE(m_list);
    }

    // ----------------------------------------------------------------------
    // Adds a new resource in the manager
    // ----------------------------------------------------------------------
    Type * Add(char * name, char * path = NULL)
    {
        if (path == NULL) {
            char standard_way[] = "./";
            path = standard_way;
        }

        // Check the settings and params
        if (m_list == NULL || name == NULL || path == NULL)
            return NULL;

        // If element exists it wil be returned
        Type * element = GetElement(name, path);
        if (element != NULL)
        {
            element->IncRef();
            return element;
        }

        // Creates the resource (better, if by useng of special construct
        // function or by using standard method)
        Type * resource = NULL;
        if (CreateResource != NULL)
            CreateResource(resource, name, path);
        else
            resource = new Type(name, path);

        // Adds new resource in manager and returns the pointer to that
        return m_list->Add(resource);
    }

    // ----------------------------------------------------------------------
    // Deletes the resource from manager
    // ----------------------------------------------------------------------
    void Remove(Type * resource)
    {
        // Check that this resource exist
        if (resource == NULL || m_list == NULL)
            return;

        // Decrements the references counter
        resource->DecRef();

        // If references counter is equal to the 1 (it means that resource
        // do not used any more) it will delete that resource
        if ((resource)->GetRefCount() == 0)
            m_list->Remove(resource);
    }

    // ----------------------------------------------------------------------
    // Deletes all the resources and free all the data
    // ----------------------------------------------------------------------
    void EmptyList()
    {
        if (m_list != NULL)
            m_list->Empty();
    }

    // ----------------------------------------------------------------------
    // Returns the list of the resources
    // ----------------------------------------------------------------------
    xLinkedList<Type> * GetList()
    {
        return m_list;
    }

    // ----------------------------------------------------------------------
    // returns resource by name and path
    // ----------------------------------------------------------------------
    Type * GetElement(char * name, char * path = NULL)
    {
        if (path == NULL) {
            char standard_way[] = "./";
            path = standard_way;
        }

        // Check the settings and params
        if (name == NULL || path == NULL || m_list == NULL)
            return NULL;
        if (m_list->GetFirst() == NULL)
            return NULL;

        // Iterates whole the list to fin the pointer to the resource
        m_list->Iterate(true);
        while(m_list->Iterate())
        {
            if (strcmp(m_list->GetCurrent()->GetName(), name) == 0) {
                if (strcmp(m_list->GetCurrent()->GetPath(), path) == 0) {
                    return m_list->GetCurrent();
                }
            }
        }

        // Returns NULL if resource is not found
        return NULL;
    }

private:
    xLinkedList<Type> * m_list;                                             // Linked List of resources
    void (* CreateResource)(Type * resource, char * name, char * path);     // Special loading function for current resource type
};


#endif //OXYGEN_XRESOURCEMANAGER_H
