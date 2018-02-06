/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 30.01.2018.
 * Copyright
 *
 * xState releases the class State for fast,
 * efficient and transparent control of the
 * engine working and of the resources allocation
 */

#ifndef OXYGEN_XSTATE_H
#define OXYGEN_XSTATE_H

// ----------------------------------------------------------------------
// Viewer Setup Structure (the virtual viewer setup settings structure
// ----------------------------------------------------------------------

struct xViewerSetup
{
    xVector3 * position;        //
    xVector3 * at;              //
    xVector3 * up;              //

    GLdouble front;             //
    GLdouble back;              //
    GLdouble angle;             //

    xViewerSetup()
    {
        // Memory allocation
        position = new xVector3;
        at = new xVector3;
        up = new xVector3;

        // Standrad frustum pyramid settings
        front = 0.0001;
        back = 10000.0;
        angle = 45.0;

        // Standard virtual viewer position
        position->Set(0.0, 0.0, 5.0);
        at->Set(0.0, 0.0, 0.0);
        up->Set(0.0, 1.0, 0.0);
    }
};

// ----------------------------------------------------------------------
// State class
// ----------------------------------------------------------------------

class xState
{
public:

    // ----------------------------------------------------------------------
    // Class constructor
    // ----------------------------------------------------------------------
    xState(unsigned long id = 0);

    // ----------------------------------------------------------------------
    // Allows to the state complete some pre-processing operations
    // (in the time of the creating of the state)
    // ----------------------------------------------------------------------
    virtual void Load();

    // ----------------------------------------------------------------------
    // Allows to the state complete some post-processing operations
    // (in the time of deleting of the state)
    // ----------------------------------------------------------------------
    virtual void Close();

    // ----------------------------------------------------------------------
    // Returns the params of the setting of the view for current framee
    // ----------------------------------------------------------------------
    virtual void RequestViewer(xVirtualCamera * camera);

    // ----------------------------------------------------------------------
    // Updates state according to the elapsed time
    // ----------------------------------------------------------------------
    virtual void Update(double elapsed);

    // ----------------------------------------------------------------------
    // Renders state using all updated settings
    // ----------------------------------------------------------------------
    virtual void Render();

    // ----------------------------------------------------------------------
    // Returns unique ID of the state
    // ----------------------------------------------------------------------
    unsigned long GetID();

private:

    unsigned long m_id;         // Unique ID for each state select needed one
};


#endif //OXYGEN_XSTATE_H
