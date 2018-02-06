/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 31.01.2018.
 * Copyright
 *
 * Realisation of functions defined in the file
 * xInput.h. Go there to find more information
 * and interface specifications
 */

#include "xEngine.h"

int is_cursor_in_window;
double mouse_x_position;
double mouse_y_position;
double mouse_delta_x;
double mouse_delta_y;
double scroll_delta_x;
double scroll_delta_y;
int mouse_button;
int mouse_button_action;

unsigned int pressed_char;
int pressed_char_mods;
int keyboard_key;
int keyboard_key_action;

static void m_cursorPositionCallback(GLFWwindow * window, double x_position, double y_position)
{
    mouse_delta_x = mouse_x_position - x_position;
    mouse_delta_y = mouse_y_position - y_position;

    mouse_x_position = x_position;
    mouse_y_position = y_position;
}

static void m_cursorEnterCallback(GLFWwindow * window, int is_entered)
{
    is_cursor_in_window = is_entered;
}

static void m_mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    mouse_button = button;
    mouse_button_action = action;
}

static void m_scrollCallback(GLFWwindow * window, double xDelta, double yDelta)
{
    scroll_delta_x = xDelta;
    scroll_delta_y = yDelta;
}

static void m_characterCallback(GLFWwindow * window, unsigned int key, int mods)
{
    pressed_char = key;
    pressed_char_mods = mods;
}

static void m_keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    keyboard_key = key;
    keyboard_key_action = action;
}

xInput::xInput(GLFWwindow * window_descriptor)
{
    m_window = window_descriptor;

    mouse_x_position = 0.0;
    mouse_y_position = 0.0;
    mouse_delta_x = 0.0;
    mouse_delta_y = 0.0;
    scroll_delta_x = 0.0;
    scroll_delta_y = 0.0;

    mouse_button = -1;
    mouse_button_action = -1;

    pressed_char = 0;
    pressed_char_mods = -1;

    keyboard_key = -1;
    keyboard_key_action = -1;

    m_isGamePadAvailable = false;
    m_inaccuracy = 0.0;

    // Set necessary modes (sticky to prevent lose of info in cycles)
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    // Callback sets functions
    glfwSetCursorPosCallback(m_window, m_cursorPositionCallback);
    glfwSetCursorEnterCallback(m_window, m_cursorEnterCallback);
    glfwSetMouseButtonCallback(m_window, m_mouseButtonCallback);
    glfwSetScrollCallback(m_window, m_scrollCallback);

    glfwSetCharModsCallback(m_window, m_characterCallback);
    glfwSetKeyCallback(m_window, m_keyCallback);

    // First update (to get info before main loop)
    m_isGamePadAvailable = glfwJoystickPresent(GLFW_JOYSTICK_1);
    if (m_isGamePadAvailable)
    {
        const float * axis = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &m_axisCount);
        for(int i = 0; i < m_axisCount; i++) {
            m_delta_axis[i] = axis[i] - m_axis[i];
        }
        memcpy(m_axis, axis, sizeof(float) * m_axisCount);

        const unsigned char * buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &m_buttonCount);
        memcpy(m_previous_buttons, m_buttons, sizeof(unsigned char) * m_buttonCount);
        memcpy(m_buttons, buttons, sizeof(unsigned char) * m_buttonCount);
    }
}

xInput::~xInput()
{

}

unsigned char xInput::GetPressedChar()
{
    return pressed_char;
}

int xInput::GetPressedCharMods()
{
    return pressed_char_mods;
}

bool xInput::IsKeyPressed(int key)
{
    if (keyboard_key == key && keyboard_key_action == GLFW_PRESS) {
        return true;
    } else {
        return false;
    }
}

bool xInput::IsKeyRepeated(int key)
{
    if (keyboard_key == key && keyboard_key_action == GLFW_REPEAT) {
        return true;
    } else {
        return false;
    }
}

bool xInput::IsKeyReleased(int key)
{
    if (keyboard_key == key && keyboard_key_action == GLFW_RELEASE) {
        return true;
    } else {
        return false;
    }
}

bool xInput::IsMouseButtonPressed(char button)
{
    if (mouse_button == button && mouse_button_action == GLFW_PRESS) {
        return true;
    } else {
        return false;
    }
}

bool xInput::IsMouseButtonReleased(char button)
{
    if (mouse_button == button && mouse_button_action == GLFW_RELEASE) {
        return true;
    } else {
        return false;
    }
}

double xInput::GetScrollX()
{
    return scroll_delta_x;
}

double xInput::GetScrollY()
{
    return  scroll_delta_y;
}

void xInput::HideCursor()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void xInput::ShowCursor()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

int xInput::IsCursorInWindow()
{
    return is_cursor_in_window;
}

double xInput::GetPositionX()
{
    return mouse_x_position;
}

double xInput::GetPositionY()
{
    return mouse_y_position;
}

double xInput::GetDeltaX()
{
    return mouse_delta_x;
}

double xInput::GetDeltaY()
{
    return mouse_delta_y;
}

void xInput::SetGamePadAxisInaccuracy(float inaccuracy)
{
    m_inaccuracy = inaccuracy;
}

int xInput::IsGamePadAvailable()
{
    return m_isGamePadAvailable;
}

bool xInput::IsPressedGamePadButton(char button)
{
    if (m_isGamePadAvailable && GLFW_PRESS == m_buttons[button]) {
        return true;
    } else {
        return false;
    }
}

bool xInput::IsWasPressedGamePadButton(char button)
{
    if (m_isGamePadAvailable && GLFW_PRESS == m_previous_buttons[button]) {
        return true;
    } else {
        return false;
    }
}

bool xInput::IsReleasedGamePadButton(char button)
{
    if (m_isGamePadAvailable && GLFW_RELEASE == m_buttons[button] && GLFW_PRESS == m_previous_buttons[button]) {
        return true;
    } else {
        return false;
    }
}

float xInput::GetGamePadAxisAngle(char axis)
{
    if (m_isGamePadAvailable) {
        return (abs(m_axis[axis]) > m_inaccuracy ? m_axis[axis] : 0);
    } else {
        return 0.0;
    }
}

float xInput::GetGamePadDeltaAxisAngle(char axis)
{
    if (m_isGamePadAvailable) {
        return m_delta_axis[axis];
    } else {
        return 0.0;
    }
}

const char * xInput::GetGamePadName()
{
    if (m_isGamePadAvailable) {
        return glfwGetJoystickName(GLFW_JOYSTICK_1);
    }
}

void xInput::Update()
{
    //mouse_button = -1;
    //mouse_button_action = -1;

    mouse_delta_x = 0.0;
    mouse_delta_y = 0.0;

    pressed_char = 0;
    pressed_char_mods = -1;

    keyboard_key = -1;
    keyboard_key_action = -1;

    m_isGamePadAvailable = glfwJoystickPresent(GLFW_JOYSTICK_1);
    if (m_isGamePadAvailable)
    {
        const float * axis = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &m_axisCount);
        for(int i = 0; i < m_axisCount; i++) {
            m_delta_axis[i] = axis[i] - m_axis[i];
        }
        memcpy(m_axis, axis, sizeof(float) * m_axisCount);

        const unsigned char * buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &m_buttonCount);
        memcpy(m_previous_buttons, m_buttons, sizeof(unsigned char) * m_buttonCount);
        memcpy(m_buttons, buttons, sizeof(unsigned char) * m_buttonCount);
    }
}


