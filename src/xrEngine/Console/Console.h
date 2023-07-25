// Console.h: interface for the CConsole class.
// 
//////////////////////////////////////////////////////////////////////
#pragma once

class ENGINE_API IConsole_Command;

#define CmdLineSize 256

#ifdef ConsoleMTDisable
#define MT_LOG 0
#else
#define MT_LOG 1
#endif

#ifdef LADCEGAtoCR
#include <imgui_api/imgui.h>
#else
#include <imgui/imgui.h>
#endif


#if MT_LOG
#include "../../xrCore/RingBuffer.h"
#endif

#include "../../Include/xrRender/FactoryPtr.h"
#include "../../Include/xrRender/UIShader.h"




class ENGINE_API CConsole :
	//public IInputReceiver,
	public pureRender,
	public pureFrame,
	public pureScreenResolutionChanged
{
public:
	struct str_pred
	{	
		IC bool operator()(const char* x, const char* y) const
		{
			return (xr_strcmp( x, y ) < 0);
		}
	};
	typedef  xr_map<LPCSTR,IConsole_Command*,str_pred>	vecCMD;
	typedef  vecCMD::iterator							vecCMD_IT;
	typedef  vecCMD::const_iterator						vecCMD_CIT;

private: ////////// NEW ///////////////
	typedef std::map<char, std::pair<ImVec4, char> > CosnoleMark; //new

	ImGuiWindowFlags	mCmdWindowFlags		= 0;
	ImGuiWindowFlags	mTipsWindowFlags	= 0;

	char				mCommandLine[CmdLineSize] = "";
	bool				mShowCMDDbgWnd;
	bool				mShowConsole;
	CosnoleMark			mMarks;

public:
	ImFont*				mConsoleFont;
private:
	ImFont*				mConsoleIconsFont;

	static xr_string	mToCmdLine;
	int					mLastlogSize;
	float				mLastScroll;

	bool				mAutoScroll;

	HKL					mUserKeyboardLayout;

	//actions
	static bool			mNeedClearCommandLine;
	static bool			mShowTipsWindow;
	static bool			mTab;
	bool				mScrollToEnd;
	bool				mEnterSelectedTip;
	//


	ImVec4				getLineColorAndIcon(char&);
	static int			ConsoleInputCallback(ImGuiInputTextCallbackData*);
	void				DrawConsoleBackground();
	void				DrawRect(Frect const& r, u32 color);

	/*				tips			*/		 

	struct ConsoleTip
	{
		xr_string first;
		xr_string second = "";

		int hightlight_start = -1;
		bool isText = false;
	};
	struct TipsSort
	{
		bool operator()(const ConsoleTip& obj1, const ConsoleTip& obj2)
		{
			return obj1.hightlight_start < obj2.hightlight_start;
		}
	};


	typedef  xr_vector<ConsoleTip>					 ConsoleTipsVec;

	//byte				mSelectTipDir;
	byte				mTipLevel;
	bool				mNeedUpdateTipPos; //keyboard scrooling

	xr_string			mTipZeroLevel;		//for 1 level tips

	ConsoleTipsVec		GetTips(int tips_arg = 0);
	void				SelectTip(int tipsSize);
	
	ConsoleTipsVec		mTips;
	static int			mSelectedTipNo; // mSelectedTipNo
	int					mHoveredTipNo = -1; //mHoveredTipNo
	bool				mIgnoreTipsSelect = false;

	static bool			mHistoryMode; // mode for listanija tips bez obraschenija vnimanija na buffer
	static int			mHistoryNo;
	xr_vector<shared_str> mInputHistory; //user input histiory

	int					mMaxHistory = 5; //

	//view lines by type

	bool mViewErrors	= true;
	bool mViewWarnings	= true;
	bool mViewOthers	= true;
	 
	bool mEnableUnderscore = true;
	bool mEnableIcons = true;
	//
	int	mMaxLogLines = 2000;
	//colors
	float mTextBackgroundOpacity = 0.65f;
	float mUnderscoreOpacity = 0.5f;
	//

	FactoryPtr<IUIShader>* mBackgroundShader;


#if MT_LOG
	xr_vector<xrLogger::LogLineStruct> m_log_history;
	xrCriticalSection m_log_history_guard;
#else
	xr_vector<LogFileStruct> m_log_history;
#endif 
public:

	bool isOpen() { return mShowConsole;}

	void initConsoleFont();
	bool OnKeyboardPress(int dik);

	void ToggleCMDDbg() { mShowCMDDbgWnd = !mShowCMDDbgWnd; }

	void ExecuteCommand(const char*, bool = true);
#if MT_LOG
	void AddLogEntry(xrLogger::LogLineStruct line);
#endif
	void ClearLog();

public: /////////////////////////////////////////
					CConsole			();
	virtual			~CConsole			();
	virtual	void	Initialize			();
	virtual void	Destroy				();

	virtual void	OnRender			();
	virtual void 	OnFrame				();
	virtual void 	ImGuiOnFrame		();
	virtual void	OnScreenResolutionChanged();
	string64		ConfigFile;
	vecCMD			Commands;

	void			AddCommand			( IConsole_Command* cc );
	void			RemoveCommand		( IConsole_Command* cc );

	void			Show				(bool usable = true);
	void			Hide				();
	
	void			Execute				( LPCSTR cmd );
	void			ExecuteScript		( LPCSTR str );
	//void			ExecuteCommand		( LPCSTR cmd, bool record_cmd = true );

	bool			GetBool				( LPCSTR cmd ) const;
	float			GetFloat			( LPCSTR cmd, float& min, float& max) const;
	int				GetInteger			( LPCSTR cmd, int& min, int& max) const;
	LPCSTR			GetString			( LPCSTR cmd ) const;
	LPCSTR			GetToken			( LPCSTR cmd ) const;
	xr_token*		GetXRToken			( LPCSTR cmd ) const;
	Fvector			GetFVector			( LPCSTR cmd ) const;
	Fvector*		GetFVectorPtr		( LPCSTR cmd ) const;
	IConsole_Command* GetCommand		( LPCSTR cmd ) const;
}; // class CConsole

ENGINE_API extern CConsole* Console;