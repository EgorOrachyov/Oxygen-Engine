/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 25.01.2018.
 * Copyright
 *
 * xLinkedList provides functionality for engine
 * modules by usage of Double-Connected Linked List
 * structure (uses in xResourceManager.h ...)
 */

#ifndef OXYGEN_XLINKEDLIST_H
#define OXYGEN_XLINKEDLIST_H

#include "xEngine.h"

// ----------------------------------------------------------------------
// Linked List template class
// ----------------------------------------------------------------------

template <class Type> class xLinkedList
{
public:

    // ----------------------------------------------------------------------
    // Element Structure
    // ----------------------------------------------------------------------
    struct Element
    {
        Type * data;    // Pointer to element data
        Element * next; // Next element in the list
        Element * prev; // Previous element in the list

        // ----------------------------------------------------------------------
        // The element structure constructor
        // ----------------------------------------------------------------------
        Element(Type * element)
        {
            data = element;
            next = prev = NULL;
        }

        // ----------------------------------------------------------------------
        // The element structure destructor
        // ----------------------------------------------------------------------
        ~Element()
        {
            SAFE_DELETE(data);

            if (next)
                next->prev = prev;
            if (prev)
                prev->next = next;
        }
    };

    // ----------------------------------------------------------------------
    // The linked list class constructor
    // ----------------------------------------------------------------------
    xLinkedList()
    {
        m_first = m_last = m_iterate = m_temp = NULL;
        m_totalElements = 0;
    }

    // ----------------------------------------------------------------------
    // The linked list class destructor
    // ----------------------------------------------------------------------
    ~xLinkedList()
    {
        Empty();
    }

    // ----------------------------------------------------------------------
    // Adds element to the end of the list
    // ----------------------------------------------------------------------
    Type * Add(Type * element)
    {
        if (element == NULL)
            return NULL;

        if (m_first == NULL)
        {
            m_first = new Element(element);
            m_last = m_first;
        }
        else
        {
            m_last->next = new Element(element);
            m_last->next->prev = m_last;
            m_last = m_last->next;
        }

        m_totalElements += 1;

        return m_last->data;
    }

    // ----------------------------------------------------------------------
    // Adds the element before the nextElement
    // ----------------------------------------------------------------------
    Type * InsertBefore(Type * element, Element * nextElement)
    {
        m_temp = nextElement->prev;

        m_totalElements += 1;

        if (m_temp == NULL)
        {
            m_first = new Element(element);
            m_first->next = nextElement;
            nextElement->prev = m_first;

            return m_first->data;
        }
        else
        {
            m_temp->next = new Element(element);
            m_temp->next->prev = m_temp;
            m_temp->next->next = nextElement;
            nextElement->prev = m_temp->next;

            return m_temp->next->data;
        }
    }

    // ----------------------------------------------------------------------
    // Deletes the element from the list and all its data
    // ----------------------------------------------------------------------
    void Remove(Type * element)
    {
        m_temp = m_first;
        while (m_temp != NULL)
        {
            if (m_temp->data == element)
            {
                if (m_temp == m_first)
                {
                    m_first = m_first->next;
                    if (m_first)
                        m_first->prev = NULL;
                }
                if (m_temp == m_last)
                {
                    m_last = m_last->prev;
                    if (m_last)
                        m_last->next = NULL;
                }

                SAFE_DELETE(m_temp);

                element = NULL;

                m_totalElements -= 1;

                return;
            }

            m_temp = m_temp->next;
        }
    }

    // ----------------------------------------------------------------------
    // Deletes all the elements and their data
    // ----------------------------------------------------------------------
    void Empty()
    {
        while (m_last != NULL)
        {
            m_temp = m_last;
            m_last = m_last->prev;
            SAFE_DELETE(m_temp);
        }
        m_first = m_last = m_iterate = m_temp = NULL;
        m_totalElements = 0;
    }

    // ----------------------------------------------------------------------
    // Deletes all the elements with their pointers.
    // Warning: it does not deletes data of elements
    // ----------------------------------------------------------------------
    void ClearPointers()
    {
        while (m_last != NULL)
        {
            m_temp = m_last;
            m_temp->data = NULL;
            m_last = m_last->prev;
            SAFE_DELETE(m_temp);
        }
        m_first = m_last = m_iterate = m_temp = NULL;
        m_totalElements = 0;
    }

    // ----------------------------------------------------------------------
    // Deletes element from the list with its pointer
    // Warning: it does not deletes data of the element
    // ----------------------------------------------------------------------
    void ClearPointer(Type * element)
    {
        m_temp = m_first;
        while (m_temp != NULL)
        {
            if (m_temp->data == element)
            {
                if (m_temp == m_first)
                {
                    m_first = m_first->next;
                    if (m_first)
                        m_first->prev = NULL;
                }
                if (m_temp == m_last)
                {
                    m_last = m_last->prev;
                    if (m_last)
                        m_last->next = NULL;
                }

                m_temp->data = NULL;

                SAFE_DELETE( m_temp );

                element = NULL;

                m_totalElements -= 1;

                return;
            }

            m_temp = m_temp->next;
        }
    }

    // ----------------------------------------------------------------------
    // Iteration of the each element in the list
    // ----------------------------------------------------------------------
    Type * Iterate(bool restart = false)
    {
        if (restart)
            m_iterate = NULL;
        else
        {
            if (m_iterate == NULL)
                m_iterate = m_first;
            else
                m_iterate = m_iterate->next;
        }

        if (m_iterate == NULL)
            return NULL;
        else
            return m_iterate->data;
    }

    // ----------------------------------------------------------------------
    // Returns iterated element in this moment
    // ----------------------------------------------------------------------
    Type * GetCurrent()
    {
        if (m_iterate)
            return m_iterate->data;
        else
            return NULL;
    }

    // ----------------------------------------------------------------------
    // Returns the first element of the linked list
    // ----------------------------------------------------------------------
    Type * GetFirst()
    {
        if (m_first)
            return m_first->data;
        else
            return NULL;
    }

    // ----------------------------------------------------------------------
    // Returns the last element of the linked list
    // ----------------------------------------------------------------------
    Type * GetLast()
    {
        if (m_last)
            return m_last->data;
        else
            return NULL;
    }

    // ----------------------------------------------------------------------
    // Returns the element which goes after that (as a param)
    // ----------------------------------------------------------------------
    Type * GetNext(Type * element)
    {
        m_temp = m_first;
        while (m_temp != NULL)
        {
            if (m_temp->data == element)
            {
                if (m_temp->next == NULL)
                    return NULL;
                else
                    return m_temp->next->data;
            }

            m_temp = m_temp->next;
        }

        return NULL;
    }

    // ----------------------------------------------------------------------
    // Returns random element of the list
    // ----------------------------------------------------------------------
    Type * GetRandom()
    {
        if (m_totalElements == 0)
            return NULL;
        else if (m_totalElements == 1)
            return m_first->data;

        unsigned long element = rand() * m_totalElements / RAND_MAX;

        m_temp = m_first;
        for(unsigned long e = 0; e < element; e++) {
            m_temp = m_temp->next;
        }

        return m_temp->data;
    }

    // ----------------------------------------------------------------------
    // Returns completed element of the list with
    // pointers to the previous and the next elements
    // ----------------------------------------------------------------------
    Element * GetCompleteElement(Type * element)
    {
        m_temp = m_first;
        while (m_temp != NULL)
        {
            if (m_temp->data == element)
                return m_temp;

            m_temp = m_temp->next;
        }

        return NULL;
    }

    // ----------------------------------------------------------------------
    // Returns the number of elements in the linked list
    // ----------------------------------------------------------------------
    unsigned long GetTotalElements()
    {
        return m_totalElements;
    }

private:

    Element * m_first;                  // First element in the linked list
    Element * m_last;                   // Last element in the linked list
    Element * m_iterate;                // Used for iterating the linked list
    Element * m_temp;                   // Used for temporary storage in various operations

    unsigned long m_totalElements;      // Total number of elements in the linked list
};


#endif //OXYGEN_XLINKEDLIST_H
