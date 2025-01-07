#include "Input.h"


void Input::Clear() {
    for (int i = 0; i < INPUT_KEYBUFFER_SIZE; i++) {
        m_keys[i].pressed  = false;
        m_keys[i].released = false;
    }
}

void Input::SetKey(int key, bool isDown, bool wasDown) {
    if (key < 0 || key >= INPUT_KEYBUFFER_SIZE) {
        return;
    }

    m_keys[key].down     = isDown;
    m_keys[key].pressed  = isDown && !wasDown;
    m_keys[key].released = !isDown && wasDown;
}

bool Input::KeyDown(int key) const {
    if (key < 0 || key >= INPUT_KEYBUFFER_SIZE) {
        return false;
    }

    return m_keys[key].down;
}

bool Input::KeyPressed(int key) const {
    if (key < 0 || key >= INPUT_KEYBUFFER_SIZE) {
        return false;
    }

    return m_keys[key].pressed;
}

bool Input::KeyReleased(int key) const {
    if (key < 0 || key >= INPUT_KEYBUFFER_SIZE) {
        return false;
    }

    return m_keys[key].released;
}
