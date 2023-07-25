#pragma once
#include "../xr_level_controller.h"

class UIHint;
class CScriptXmlInit;
class CUIWindow;

#include "../xrGameAPI/UI/UIWindowAlloc.inl"
using namespace UIWinAlloc;

#include "UIMessages.h"
#include "../../xrServerEntities/script_export_space.h"
#include "uiabstract.h"

class CUIWindow : public CUISimpleWindow
{
public:
    CUIWindow();
    virtual ~CUIWindow();

    ////////////////////////////////////
    // ������ � ��������� � ������������� ������
    virtual void AttachChild(CUIWindow* pChild);
    virtual void DetachChild(CUIWindow* pChild);
    virtual bool IsChild(CUIWindow* pChild) const;
    virtual void DetachAll();
    int GetChildNum() { return m_ChildWndList.size(); }

    void SetParent(CUIWindow* pNewParent);
    CUIWindow* GetParent() const { return m_pParentWnd; }

    // �������� ���� ������ �������� ������
    CUIWindow* GetTop()
    {
        if (m_pParentWnd == NULL)
            return this;
        else
            return m_pParentWnd->GetTop();
    }

    CUIWindow* GetCurrentMouseHandler();
    CUIWindow* GetChildMouseHandler();

    virtual bool OnKeyboardAction(int dik, EUIMessages keyboard_action);
    virtual bool OnKeyboardHold(int dik);

    virtual bool OnMouseAction(float x, float y, EUIMessages mouse_action);
    virtual void OnMouseMove();
    virtual void OnMouseScroll(float iDirection);
    virtual bool OnDbClick();
    virtual bool OnMouseDown(int mouse_btn);
    virtual void OnMouseUp(int mouse_btn);

    virtual void OnFocusReceive();
    virtual void OnFocusLost();

    // Alundaio
    /*
    void			DisableHint			();
    void			EnableHint			();
    void			SetHintDelay		(u32 val);
    u32				GetHintDelay		();
    void			DrawHintWnd			();
    void			RemoveHint			();
    void			SetHintText			(LPCSTR text);
    LPCSTR			GetHintText			();
    void			SetHintWnd			(UIHint* hintWnd) { m_pHint = hintWnd; };
    UIHint*			GetHintWnd			() { return m_pHint; };
    */
    //-Alundaio

    // ���������/���������� ���� �����
    // ��������� ���������� �������� ����� �������������
    void SetCapture(CUIWindow* pChildWindow, bool capture_status);
    CUIWindow* GetMouseCapturer() { return m_pMouseCapturer; }

    // ������, �������� ������������ ���������,
    // ���� NULL, �� ���� �� GetParent()
    void SetMessageTarget(CUIWindow* pWindow) { m_pMessageTarget = pWindow; }
    CUIWindow* GetMessageTarget();

    void SetKeyboardCapture(CUIWindow* pChildWindow, bool capture_status);

    // ��������� ��������� �� �������������� ������������ �������������
    // �-��� ������ ����������������
    // pWnd - ��������� �� ����, ������� ������� ���������
    // pData - ��������� �� �������������� ������, ������� ����� ������������
    virtual void SendMessage(CUIWindow* pWnd, s16 msg, void* pData = NULL);

    virtual void Enable(bool status) { m_bIsEnabled = status; }
    bool IsEnabled() { return m_bIsEnabled; }

    // ������/�������� ���� � ��� �������� ����
    virtual void Show(bool status)
    {
        SetVisible(status);
        Enable(status);
    }

    IC bool IsShown() { return GetVisible(); }
    void ShowChildren(bool show);

    // ���������� ����������
    IC void GetAbsoluteRect(Frect& r);
    IC void GetAbsolutePos(Fvector2& p)
    {
        Frect abs;
        GetAbsoluteRect(abs);
        p.set(abs.x1, abs.y1);
    }

    void SetWndRect_script(Frect rect) { CUISimpleWindow::SetWndRect(rect); }
    void SetWndPos_script(Fvector2 pos) { CUISimpleWindow::SetWndPos(pos); }
    void SetWndSize_script(Fvector2 size) { CUISimpleWindow::SetWndSize(size); }

    // ���������� ����
    virtual void Draw();
    virtual void Draw(float x, float y);
    // ���������� ���� ����������������
    virtual void Update();

    void SetPPMode();
    void ResetPPMode();
    IC bool GetPPMode() { return m_bPP; };
    // ��� �������� ���� � �������� � �������� ���������
    virtual void Reset();
    void ResetAll();

    DEF_UILIST(WINDOW_LIST, CUIWindow*);
    WINDOW_LIST& GetChildWndList() { return m_ChildWndList; }

    IC bool IsAutoDelete() { return m_bAutoDelete; }
    IC void SetAutoDelete(bool auto_delete) { m_bAutoDelete = auto_delete; }

    // Name of the window
    const shared_str WindowName() const { return m_windowName; }
    void SetWindowName(LPCSTR wn) { m_windowName = wn; }
    LPCSTR WindowName_script() { return m_windowName.c_str(); }
    CUIWindow* FindChild(const shared_str name);

    IC bool CursorOverWindow() const { return m_bCursorOverWindow; }
    IC u32 FocusReceiveTime() const { return m_dwFocusReceiveTime; }

    IC bool GetCustomDraw() const { return m_bCustomDraw; }
    IC void SetCustomDraw(bool b) { m_bCustomDraw = b; }

protected:
    IC void SafeRemoveChild(CUIWindow* child)
    {
        WINDOW_LIST_it it = std::find(m_ChildWndList.begin(), m_ChildWndList.end(), child);
        if (it != m_ChildWndList.end())
            m_ChildWndList.erase(it);
    };

    shared_str m_windowName;
    // ������ �������� ����
    WINDOW_LIST m_ChildWndList;

    // ��������� �� ������������ ����
    CUIWindow* m_pParentWnd;

    // �������� ���� �������, ��������� ���� ����
    CUIWindow* m_pMouseCapturer;

    // �������� ���� �������, ��������� ���� ����������
    CUIWindow* m_pKeyboardCapturer;

    // ���� ���� ���������
    CUIWindow* m_pMessageTarget;

    // ��������� ������� �����
    Fvector2 cursor_pos;

    // ����� �������� ����� �����
    // ��� ����������� DoubleClick
    u32 m_dwLastClickTime;
    u32 m_dwFocusReceiveTime;

    // ���� ��������������� �������� �� ����� ������ �����������
    bool m_bAutoDelete;

    bool m_bPP;
    bool m_bIsEnabled;

    // ���� ������ ��� �����
    bool m_bCursorOverWindow;
    bool m_bCustomDraw;

#ifdef DEBUG
    int m_dbg_id;
#endif

public:
    DECLARE_SCRIPT_REGISTER_FUNCTION
};

bool fit_in_rect(CUIWindow* w, Frect const& vis_rect, float border = 0.0f, float dx16pos = 0.0f);
