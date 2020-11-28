#ifndef BRAVE2_INPUTHANDLER_H
#define BRAVE2_INPUTHANDLER_H

#include <cstddef>
#include <GLFW/glfw3.h>

/**
 * The inputHandler class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over. (Singleton pattern)
 */
class InputHandler {
    // Input handling
    bool aKeyWasPressed;
    bool keyPressed[348] = {false};
    int modifiers;

    /**
     * The inputHandler's constructor should always be private to prevent direct
     * construction calls with the `new` operator.
     */
    InputHandler() {
        aKeyWasPressed = false;
        modifiers = 0;
        for (bool &k : keyPressed)
            k = false;
    }

    static InputHandler *singleton_;

public:

    /**
     * Singletons should not be cloneable.
     */
    InputHandler(InputHandler &other) = delete;

    /**
     * Singletons should not be assignable.
     */
    void operator=(const InputHandler &) = delete;

    static InputHandler *GetInstance();

    void KeyPress(int key);

    void SetModifiers(int m);

    void KeyRelease(int key);

    bool IsPressed(int key);

    bool IsShiftPressed();

    bool IsControlPressed();

    bool IsAltPressed();

    bool IsSuperPressed();

    bool IsCapsLock();

    bool IsNumLock();

};

#endif //BRAVE2_INPUTHANDLER_H
