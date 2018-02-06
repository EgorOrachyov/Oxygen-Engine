/*
 *  Project: Oxygen Game Engine
 *  Created by Egor Orachyov on 05.02.2018.
 *  Copyright
 *
 *  xVirtualObject is a base class, which
 *  releases functionality for each
 *  programmable object in the engine
 *  ( it involves Load, Update, Close
 *  voids, which can be programmed as an
 *  usual script)
 */

#ifndef OXYGEN_XVIRTUALOBJECT_H
#define OXYGEN_XVIRTUALOBJECT_H

// ----------------------------------------------------------------------
// Virtual Object Class
// ----------------------------------------------------------------------

class xVirtualObject
{
public:

    // ----------------------------------------------------------------------
    // Class Constructor
    // ----------------------------------------------------------------------
    xVirtualObject() {}

    // ----------------------------------------------------------------------
    // Class Destructor
    // ----------------------------------------------------------------------
    ~xVirtualObject() {}

    // ----------------------------------------------------------------------
    // Load function (can be redefined for special behaviour)
    // Will be called after camera's registration in the engine
    // ----------------------------------------------------------------------
    virtual void Load() {}

    // ----------------------------------------------------------------------
    // Update function (can be redefined for special behaviour)
    // Will be called for each main loop cycle
    // ----------------------------------------------------------------------
    virtual void Update() {}

    // ----------------------------------------------------------------------
    // Close function (can be redefined for special behaviour)
    // Will be called, when engine start self-destroying
    // ----------------------------------------------------------------------
    virtual void Close() {}
};

#endif //OXYGEN_XVIRTUALOBJECT_H
