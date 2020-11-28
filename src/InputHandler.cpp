#include "InputHandler.h"

InputHandler *InputHandler::singleton_ = nullptr;;

InputHandler *InputHandler::GetInstance() {
    if (singleton_ == nullptr) {
        singleton_ = new InputHandler();
    }

    return singleton_;
}

void InputHandler::KeyPress(int key) {
    keyPressed[key] = true;
}

void InputHandler::SetModifiers(int m) {
    modifiers = m;
}

void InputHandler::KeyRelease(int key) {
    keyPressed[key] = false;
}

bool InputHandler::IsPressed(int key) {
    return keyPressed[key];
}

bool InputHandler::IsShiftPressed() {
    return modifiers & GLFW_MOD_SHIFT;
}

bool InputHandler::IsControlPressed() {
    return modifiers & GLFW_MOD_CONTROL;
}

bool InputHandler::IsAltPressed() {
    return modifiers & GLFW_MOD_ALT;
}

bool InputHandler::IsSuperPressed() {
    return modifiers & GLFW_MOD_SUPER;
}

bool InputHandler::IsCapsLock() {
    return modifiers & GLFW_MOD_CAPS_LOCK;
}

bool InputHandler::IsNumLock() {
    return modifiers & GLFW_MOD_NUM_LOCK;
}