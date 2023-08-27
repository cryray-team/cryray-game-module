//-' OldSerpskiStalker

#pragma once
#include "stdafx.h"

namespace crAutosaveManager
{
    class ENGINE_API AutosaveManager //-' ��� �����
	{
    private:
        void* operator new(size_t size) //-' new
        {
            void* p = malloc(size);

            if (IsDebuggerPresent())
                Msg("~ [%s]: Allocated %zu bytes @ %p\n", __FUNCTION__, size, p);

            return p;
        }

        void operator delete(void* ptr) //-' delete
        {
            if (IsDebuggerPresent())
                Msg("~ [%s]: Freeing %p\n", __FUNCTION__, ptr);

            free(ptr);
        }

    private:
        int iStartTimerAutosave; //-'
        bool bAutosaveStart; //-' ����� �������, ���� ������� ����

    public:
        int iAutosave; //-' �������� int, ������� ������ �������� ��������

        bool bAutosaveConsole; //-' ����������� ����� ���������� ���� �� �������, �� ��������� false
        bool bOneIntStartAutosave; //-' ...

        static int iSettingsIntervalAutosave; //-' �������� ��������������

        AutosaveManager() 
        {
            this->iAutosave = 0;
            this->iStartTimerAutosave = 0;
            this->bAutosaveStart = false;
            this->bOneIntStartAutosave = false; //-' ������������� 1 ���, �� ����� �������� CRenderDevice::FrameMove()
            this->bAutosaveConsole = false;
        }

        virtual ~AutosaveManager()
        {

        }

    public:
        static void InitAutosaveManager(); //-' ��������� ������
        static void DeleteAutosaveManager(); //-' ������ ������

        inline void SetValueiStartTimerAutosave(int i) { this->iStartTimerAutosave = i; };
        inline int GetValueiStartTimerAutosave() { return this->iStartTimerAutosave; };

        inline bool GetbTimerAutosaveStart() { return this->bAutosaveStart; };
        inline void SetbTimerAutosaveStart(bool b) { this->bAutosaveStart = b; };
	};
}

extern ENGINE_API crAutosaveManager::AutosaveManager* pAutosaveManager;