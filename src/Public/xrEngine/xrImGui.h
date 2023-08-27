#pragma once

class ENGINE_API xrImGui
{
public:
    using RenderFunc = fastdelegate::FastDelegate<void()>;
    using KeyFunc = fastdelegate::FastDelegate<bool(int)>;
    using MouseFunc = fastdelegate::FastDelegate<bool(int, int)>;

    static void Init();
    static void FrameStart();
    static void PreRender();
    static void Render();
    static void AfterRender();
    static bool InputEnabled();
    static void AddToRender(RenderFunc func);
    static void AddKeyPressCallback(KeyFunc func);
    static void AddKeyReleaseCallback(KeyFunc func);
    static void AddKeyHoldCallback(KeyFunc func);
    static void AddMouseMoveCallback(MouseFunc func);
    static void AddMouseWheelCallback(KeyFunc func);
    static void AddKeyToHoldCallback(int key); // С перерывами возвращает KeyPress коллбек пока клавиша зажата

    static void EnableInputOnFrame();
    static bool KeyPress(int key);
    static bool KeyRelease(int key);
    static bool KeyHold(int key);
    static bool MouseMove(int dx, int dy);
    static bool MouseWheel(int direction);

private:
    static void StopHolding();
};