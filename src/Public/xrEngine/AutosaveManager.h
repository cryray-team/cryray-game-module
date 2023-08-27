//-' OldSerpskiStalker

#pragma once
#include "stdafx.h"

namespace crAutosaveManager
{
    class ENGINE_API AutosaveManager //-' Сам класс
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
        bool bAutosaveStart; //-' Старт таймера, если закрыто меню

    public:
        int iAutosave; //-' Основной int, который хранит выданное значение

        bool bAutosaveConsole; //-' Срабатывает когда происходит сейв по таймеру, по умолчанию false
        bool bOneIntStartAutosave; //-' ...

        static int iSettingsIntervalAutosave; //-' Интервал автосохранения

        AutosaveManager() 
        {
            this->iAutosave = 0;
            this->iStartTimerAutosave = 0;
            this->bAutosaveStart = false;
            this->bOneIntStartAutosave = false; //-' Переключается 1 раз, на этапе создания CRenderDevice::FrameMove()
            this->bAutosaveConsole = false;
        }

        virtual ~AutosaveManager()
        {

        }

    public:
        static void InitAutosaveManager(); //-' Выделение памяти
        static void DeleteAutosaveManager(); //-' Чистка памяти

        inline void SetValueiStartTimerAutosave(int i) { this->iStartTimerAutosave = i; };
        inline int GetValueiStartTimerAutosave() { return this->iStartTimerAutosave; };

        inline bool GetbTimerAutosaveStart() { return this->bAutosaveStart; };
        inline void SetbTimerAutosaveStart(bool b) { this->bAutosaveStart = b; };
	};
}

extern ENGINE_API crAutosaveManager::AutosaveManager* pAutosaveManager;