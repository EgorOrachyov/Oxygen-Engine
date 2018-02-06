/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 30.01.2018.
 * Copyright
 *
 * Implementation of the methods defined in
 * the xState.h. Warning: all methods are virtual,
 * therefore you can re-define it by using your
 * own functionality (saving the main and common
 * features)
 */

#include "xEngine.h"

xState::xState(unsigned long id)
{
    m_id = id;
}

void xState::Load()
{

}

void xState::Close()
{

}

void xState::RequestViewer(xVirtualCamera * camera)
{

}

void xState::Update(double elapsed)
{

}

void xState::Render()
{

}

unsigned long xState::GetID()
{
    return m_id;
}