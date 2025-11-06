#pragma once
#include <GLFW/glfw3.h>

namespace smpl
{
    namespace Key 
    {
        enum class Code 
        {
            Unknown = -1,

            // Letters
            A = GLFW_KEY_A,
            B = GLFW_KEY_B,
            C = GLFW_KEY_C,
            D = GLFW_KEY_D,
            E = GLFW_KEY_E,
            F = GLFW_KEY_F,
            G = GLFW_KEY_G,
            H = GLFW_KEY_H,
            I = GLFW_KEY_I,
            J = GLFW_KEY_J,
            K = GLFW_KEY_K,
            L = GLFW_KEY_L,
            M = GLFW_KEY_M,
            N = GLFW_KEY_N,
            O = GLFW_KEY_O,
            P = GLFW_KEY_P,
            Q = GLFW_KEY_Q,
            R = GLFW_KEY_R,
            S = GLFW_KEY_S,
            T = GLFW_KEY_T,
            U = GLFW_KEY_U,
            V = GLFW_KEY_V,
            W = GLFW_KEY_W,
            X = GLFW_KEY_X,
            Y = GLFW_KEY_Y,
            Z = GLFW_KEY_Z,

            // Numbers
            Num0 = GLFW_KEY_0,
            Num1 = GLFW_KEY_1,
            Num2 = GLFW_KEY_2,
            Num3 = GLFW_KEY_3,
            Num4 = GLFW_KEY_4,
            Num5 = GLFW_KEY_5,
            Num6 = GLFW_KEY_6,
            Num7 = GLFW_KEY_7,
            Num8 = GLFW_KEY_8,
            Num9 = GLFW_KEY_9,

            // Function keys
            Escape      = GLFW_KEY_ESCAPE,
            Enter       = GLFW_KEY_ENTER,
            Tab         = GLFW_KEY_TAB,
            Backspace   = GLFW_KEY_BACKSPACE,
            Insert      = GLFW_KEY_INSERT,
            Delete      = GLFW_KEY_DELETE,
            Right       = GLFW_KEY_RIGHT,
            Left        = GLFW_KEY_LEFT,
            Down        = GLFW_KEY_DOWN,
            Up          = GLFW_KEY_UP,
            PageUp      = GLFW_KEY_PAGE_UP,
            PageDown    = GLFW_KEY_PAGE_DOWN,
            Home        = GLFW_KEY_HOME,
            End         = GLFW_KEY_END,
            CapsLock    = GLFW_KEY_CAPS_LOCK,
            ScrollLock  = GLFW_KEY_SCROLL_LOCK,
            Pause       = GLFW_KEY_PAUSE,
            F1          = GLFW_KEY_F1,
            F2          = GLFW_KEY_F2,
            F3          = GLFW_KEY_F3,
            F4          = GLFW_KEY_F4,
            F5          = GLFW_KEY_F5,
            F6          = GLFW_KEY_F6,
            F7          = GLFW_KEY_F7,
            F8          = GLFW_KEY_F8,
            F9          = GLFW_KEY_F9,
            F10         = GLFW_KEY_F10,
            F11         = GLFW_KEY_F11,
            F12         = GLFW_KEY_F12,
            PrintScreen = GLFW_KEY_PRINT_SCREEN,

            // Optional
            Space       = GLFW_KEY_SPACE,
            KeypadAdd   = GLFW_KEY_KP_ADD,
            KeypadEnter = GLFW_KEY_KP_ENTER,
            LShift      = GLFW_KEY_LEFT_SHIFT,
            RShift      = GLFW_KEY_RIGHT_SHIFT,
            LCtrl       = GLFW_KEY_LEFT_CONTROL,
            RCtrl       = GLFW_KEY_RIGHT_CONTROL,
            LAlt        = GLFW_KEY_LEFT_ALT,
            RAlt        = GLFW_KEY_RIGHT_ALT,
            LWin        = GLFW_KEY_LEFT_SUPER,
            RWin        = GLFW_KEY_RIGHT_SUPER,
            Equal       = GLFW_KEY_EQUAL,
            Minus       = GLFW_KEY_MINUS,
        };

        inline bool operator==(int glfwKey, Code key) 
        {
            return glfwKey == static_cast<int>(key);
        }

        inline bool operator==(Code key, int glfwKey) 
        {
            return static_cast<int>(key) == glfwKey;
        }
    }
}