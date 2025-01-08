#ifndef SAL_INPUT_H
#define SAL_INPUT_H

#define INPUT_KEYBUFFER_SIZE 256

struct InputButton {
    bool down;
    bool pressed;
    bool released;
};

class Input {
public:
    void Clear();
    void SetKey(int key, bool isDown, bool wasDown);

    bool KeyDown(int key) const;
    bool KeyPressed(int key) const;
    bool KeyReleased(int key) const;
private:
    InputButton m_keys[INPUT_KEYBUFFER_SIZE];
};

#endif
