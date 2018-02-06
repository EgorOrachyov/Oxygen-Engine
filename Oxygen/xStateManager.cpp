/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 30.01.2018.
 * Copyright
 *
 * Realisation of functions defined in the file
 * xStateManager.h. Go there to find more information
 * and interface specifications
 */

#include "xEngine.h"

xStateManager::xStateManager()
{
    m_states = new xLinkedList<xState>;
    m_currentState = NULL;
    m_stateChanged = false;
}

xStateManager::~xStateManager()
{
    if (m_currentState != NULL) {
        m_currentState->Close();
    }

    SAFE_DELETE(m_states);
}

void xStateManager::AddState(xState * state, bool to_change)
{
    m_states->Add(state);

    if (!to_change) {
        return;
    }

    // if there was current state we should close it correctly
    if (m_currentState != NULL) {
        m_currentState->Close();
    }

    // Load new current state
    m_currentState = m_states->GetLast();
    m_currentState->Load();
}

void xStateManager::RemoveState(xState * state)
{
    m_states->Remove(state);
}

void xStateManager::RemoveState(unsigned long state_id)
{
    xState * tmp;
    m_states->Iterate(true);

    // Find the state with needed id
    while(m_states->Iterate() != NULL)
    {
        tmp = m_states->GetCurrent();

        if (tmp->GetID() == state_id) {
            m_states->Remove(tmp);

            break;
        }
    }
}

void xStateManager::ChangeState(unsigned long state_id)
{
    // Iterates throw the list to find needed state to change
    m_states->Iterate (true);
    while(m_states->Iterate() != NULL)
    {
        if (m_states->GetCurrent()->GetID() == state_id)
        {
            // Close previous state
            if (m_currentState != NULL)
                m_currentState->Close();

            // Sets new state as currend and loads it
            m_currentState = m_states->GetCurrent();
            m_currentState->Load();

            // Sets the flag to indicate, tha state has been changed
            m_stateChanged = true;

            break;
        }
    }
}

bool xStateManager::IsStateChanged()
{
    return m_stateChanged;
}

void xStateManager::SetToDefault()
{
    m_stateChanged = false;
}

xState * xStateManager::GetCurrentState()
{
    return  m_currentState;
}