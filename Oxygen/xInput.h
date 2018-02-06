/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 31.01.2018.
 * Copyright
 *
 * xInput release wrapper class for
 * free glut input, which provides functions
 * for working with data got from keyboard,
 * mouse and game pad.
 */

#ifndef OXYGEN_XINPUT_H
#define OXYGEN_XINPUT_H

#include "GLFW/glfw3.h"

#define MAX_GAMEPAD_BUTTON_NUMBER 20
#define MAX_GAMEPAD_AXIS_NUMBER 10

#define GAMEPAD_SQUARE 0
#define GAMEPAD_XCROSS 1
#define GAMEPAD_CIRCLE 2
#define GAMEPAD_TRIANGLE 3
#define GAMEPAD_L1 4
#define GAMEPAD_R1 5
#define GAMEPAD_L2 6
#define GAMEPAD_R2 7
#define GAMEPAD_SHARE 8
#define GAMEPAD_OPTIONS 9
#define GAMEPAD_L3 10
#define GAMEPAD_R3 11
#define GAMEPAD_PS 12
#define GAMEPAD_TRACKPAD 13
#define GAMEPAD_UP 14
#define GAMEPAD_RIGHT 15
#define GAMEPAD_DOWN 16
#define GAMEPAD_LEFT 17
#define GAMEPAD_L3_X_AXIS 0
#define GAMEPAD_L3_Y_AXIS 1
#define GAMEPAD_R3_X_AXIS 2
#define GAMEPAD_R3_Y_AXIS 3
#define GAMEPAD_L2_AXIS 4
#define GAMEPAD_R2_AXIS 5

#define MOUSE_BUTTON_LEFT GLFW_MOUSE_BUTTON_LEFT
#define MOUSE_BUTTON_RIGHT GLFW_MOUSE_BUTTON_RIGHT

extern int is_cursor_in_window;
extern double mouse_x_position;
extern double mouse_y_position;
extern double mouse_delta_x;
extern double mouse_delta_y;
extern double scroll_delta_x;
extern double scroll_delta_y;
extern int mouse_button;
extern int mouse_button_action;

extern unsigned int pressed_char;
extern int pressed_char_mods;
extern int keyboard_key;
extern int keyboard_key_action;

static void m_cursorPositionCallback(GLFWwindow * window, double x_position, double y_position);
static void m_cursorEnterCallback(GLFWwindow * window, int is_entered);
static void m_mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
static void m_scrollCallback(GLFWwindow * window, double xDelta, double yDelta);

static void m_characterCallback(GLFWwindow * window, unsigned int key, int mods);
static void m_keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);

// ----------------------------------------------------------------------
// Input Wrapper Class
// ----------------------------------------------------------------------

class xInput
{
public:

    // ----------------------------------------------------------------------
    // Class Constructor from window descriptor
    // ----------------------------------------------------------------------
    xInput(GLFWwindow * window_descriptor);

    // ----------------------------------------------------------------------
    // Class Destructor
    // ----------------------------------------------------------------------s
    ~xInput();

    // ----------------------------------------------------------------------
    // Keyboard input functions
    // ----------------------------------------------------------------------
    unsigned char GetPressedChar();
    int GetPressedCharMods();
    bool IsKeyPressed(int key);
    bool IsKeyRepeated(int key);
    bool IsKeyReleased(int key);

    // ----------------------------------------------------------------------
    // Mouse (touch pad, cursor) input functions
    // ----------------------------------------------------------------------
    bool IsMouseButtonPressed(char button);
    bool IsMouseButtonReleased(char button);
    void HideCursor();
    void ShowCursor();
    int IsCursorInWindow();
    double GetPositionX();
    double GetPositionY();
    double GetDeltaX();
    double GetDeltaY();
    double GetScrollX();
    double GetScrollY();

    // ----------------------------------------------------------------------
    // Joystick / Game Pad input functions
    // ----------------------------------------------------------------------
    void SetGamePadAxisInaccuracy(float inaccuracy);
    int IsGamePadAvailable();
    bool IsPressedGamePadButton(char button);
    bool IsWasPressedGamePadButton(char button);
    bool IsReleasedGamePadButton(char button);
    float GetGamePadAxisAngle(char axis);
    float GetGamePadDeltaAxisAngle(char axis);
    const char * GetGamePadName();

    // ----------------------------------------------------------------------
    // Update function (should be called for each main loop step)
    // ----------------------------------------------------------------------
    void Update();

private:

    GLFWwindow * m_window;              // Window descriptor
    int m_isGamePadAvailable;           // Is GamePad available
    int m_axisCount;                    // Game Pad axis count
    int m_buttonCount;                  // Game Pad button count
    float m_inaccuracy;                 // Inaccuracy for Game Pad sticks

    unsigned char m_buttons[MAX_GAMEPAD_BUTTON_NUMBER];
    unsigned char m_previous_buttons[MAX_GAMEPAD_BUTTON_NUMBER];
    float m_axis[MAX_GAMEPAD_AXIS_NUMBER];
    float m_delta_axis[MAX_GAMEPAD_AXIS_NUMBER];

};


#endif //OXYGEN_XINPUT_H
