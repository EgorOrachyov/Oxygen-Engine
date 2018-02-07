/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 29.01.2018.
 * Copyright
 *
 * xDynamicArray releases interface of
 * dynamic array (reference to vector)
 */

#ifndef OXYGEN_XDYNAMICARRAY_H
#define OXYGEN_XDYNAMICARRAY_H

#include "xEngine.h"

// ----------------------------------------------------------------------
// Dynamic Array Class
// ----------------------------------------------------------------------

template <class Type> class xDynamicArray
{
public:
    // ----------------------------------------------------------------------
    //  Class constructor
    // ----------------------------------------------------------------------
    xDynamicArray()
    {
        m_size = 0;
        m_numOfElements = 0;
        m_array = NULL;
    }

    // ----------------------------------------------------------------------
    // Class destructor
    // ----------------------------------------------------------------------
    ~xDynamicArray()
    {
        EmptyMass();
    }

    // ----------------------------------------------------------------------
    // Adds element in the end of the array
    // ----------------------------------------------------------------------
    void Add(Type * element)
    {
        // If array is not created it will allocate memory for that
        if (m_array == NULL)
        {
            m_size = 16;
            m_array = (Type **)calloc(m_size, sizeof(Type *));

            if (m_array == NULL) {
                printf("ERROR: cannot allocate memory for m_array \n");
                exit(1);
            }
        }

        // If we have enough place we will put it into the array
        if (m_numOfElements < m_size)
        {
            m_array[m_numOfElements] = element;
            m_numOfElements += 1;
        }
        // Else we have to reallocate and expand array
        else
        {
            m_size *= 2;
            m_array = (Type **)realloc(m_array, sizeof(Type *) * m_size);

            if (m_array == NULL) {
                printf("ERROR: cannot reallocate memory for m_array \n");
                exit(1);
            }

            m_array[m_numOfElements] = element;
            m_numOfElements += 1;
        }
    }

    // ----------------------------------------------------------------------
    // Deletes the element with that index from the array (if it exist) and
    // moves all the elements from right to the left on 1 position
    // ----------------------------------------------------------------------
    void Remove(long index)
    {
        if (index >= m_numOfElements || index < 0)
        {
            return;
        }
        else
        {
            SAFE_DELETE(m_array[index]);

            for(long i = index; i < m_numOfElements - 1; i += 1)
            {
                m_array[i] = m_array[i + 1];
            }

            m_numOfElements -= 1;
        }
    }

    // ----------------------------------------------------------------------
    // Deletes all the elements from the array with their data and frees
    // memory allocated for the array
    // ----------------------------------------------------------------------
    void EmptyMass()
    {
        if (m_numOfElements > 0) {
            for(long i = 0; i < m_numOfElements; i += 1) {
                SAFE_DELETE(m_array[i]);
            }
            SAFE_DELETE_ARRAY(m_array);
        }

        m_size = 0;
        m_numOfElements = 0;
        m_array = NULL;
    }

    // ----------------------------------------------------------------------
    // Clear mass, do not delete data of the elements (clear pointers)
    // ----------------------------------------------------------------------
    void Clear()
    {
        if (m_numOfElements > 0) {
            SAFE_DELETE(m_array);

            m_size = 0;
            m_numOfElements = 0;
            m_array = NULL;
        }
    }

    // ----------------------------------------------------------------------
    // Returns the element by its index (if it exists) or NULL
    // ----------------------------------------------------------------------
    Type * GetElement(long index)
    {
        if (index >= m_numOfElements || index < 0) {
            return NULL;
        } else {
            return m_array[index];
        }
    }

    // ----------------------------------------------------------------------
    // Returns current allocated memory (in bytes) for the array
    // ----------------------------------------------------------------------
    long GetSize()
    {
        return m_size;
    }

    // ----------------------------------------------------------------------
    // Returns number of elements in the array (real size)
    // ----------------------------------------------------------------------
    long GetNumOfElements()
    {
        return m_numOfElements;
    }

    // ----------------------------------------------------------------------
    // Returns the pointer to the array with data
    // ----------------------------------------------------------------------
    Type ** GetMass()
    {
        return m_array;
    }

private:
    Type ** m_array;                    // Dynamic array with
    long m_size;               // Max size of array
    long m_numOfElements;      // Number of elements in the array

};

#endif //OXYGEN_XDYNAMICARRAY_H
