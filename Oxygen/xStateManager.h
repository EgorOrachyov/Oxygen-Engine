/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 30.01.2018.
 * Copyright
 *
 * xStateManager releases class for
 * powerful engine main loop control by
 * using simple states. Developer can
 * create his own states, define unique
 * behaviour and add them to the manager,
 * which will check current state for
 * each cycle and does some important
 * actions (loading, updating, rendering
 * viewer's setting, closing)
 *
 * Warning: manager has only one active
 * state, therefore engine can be controlled
 * just by current (active) state
 */

#ifndef OXYGEN_XSTATEMANAGER_H
#define OXYGEN_XSTATEMANAGER_H

// ----------------------------------------------------------------------
// State Manager Class
// ----------------------------------------------------------------------

class xStateManager
{
public:

    // ----------------------------------------------------------------------
    // Class Constructor
    // ----------------------------------------------------------------------
    xStateManager();

    // ----------------------------------------------------------------------
    // Class Destructor
    // ----------------------------------------------------------------------
    ~xStateManager();

    // ----------------------------------------------------------------------
    // Add a new state to the list and (if flag is_to_change = true) sets
    // this new state as the current state (active)
    // ----------------------------------------------------------------------
    void AddState(xState * state, bool is_to_change = false);

    // ----------------------------------------------------------------------
    // Removes the state by pointer from the manager with its data
    // ----------------------------------------------------------------------
    void RemoveState(xState * state);

    // ----------------------------------------------------------------------
    // Removes the state by unique id from the manager with its data
    // ----------------------------------------------------------------------
    void RemoveState(unsigned long state_id);

    // ----------------------------------------------------------------------
    // Sets state with with id = state_id as the current state (active)
    // ----------------------------------------------------------------------
    void ChangeState(unsigned long state_id);

    // ----------------------------------------------------------------------
    // Returns true, if current state has been changed
    // ----------------------------------------------------------------------
    bool IsStateChanged();

    // ----------------------------------------------------------------------
    // Sets manager's values to the default positions (should be called
    // for each main loop iteration)
    // ----------------------------------------------------------------------
    void SetToDefault();

    // ----------------------------------------------------------------------
    // Returns pointer to the current (active) state
    // ----------------------------------------------------------------------
    xState * GetCurrentState();

private:

    xLinkedList<xState> * m_states;     // Linked list with states
    xState * m_currentState;            // Pointer to the current (active) state
    bool m_stateChanged;                // Flag, which indicates, that state has been changed
};


#endif //OXYGEN_XSTATEMANAGER_H
