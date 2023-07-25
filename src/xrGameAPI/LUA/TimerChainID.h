///////////////////////////////////////////////////////////////////////////
//	File 		: CryRayTimerID.h
//	Author		: OldSerpskiStalker
//	Description : The main file of the timer call
//  Version     : 0.1c
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <memory>
#include <functional>
#include <random>
#include <iostream>
#include <fstream>

#include "CryRayTimerID.h"
using namespace TimerID;

#include "../xrThreads/ThreadsID.h"
using namespace ThreadManagerID;

#include "../xrJSONLib/template.h"

namespace CRTimers
{
    static std::mutex scriptExecutionMutex;
    static std::atomic<bool> scriptExecuting(false);

    static void CreateTimerTable(unsigned id, unsigned start, unsigned end)
    {
        JSON::CreateTimerTableJson("gamedata\\scripts\\json\\timer.json", id, start, end);
    }

    static void TimerChainID(unsigned id, const LPCSTR name_script, const LPCSTR name_function, unsigned start, unsigned end)
    {
        if (name_script[0] == '\0' || name_function[0] == '\0')
        {
            Msg("! [%s] -> name_script is empty or name_function is empty", __FUNCTION__);
            return;
        }
        else if (id < 0)
        {
            Msg("! [%s] ->Timer id cannot be less than zero", __FUNCTION__);
            return;
        }

        Msg("~ [%s]: ID = %i", __FUNCTION__, id);

        CreateTimerTable(id, start, end);

        // Create a new ID and assign it to a dynamically allocated integer using std::make_shared
        std::shared_ptr<unsigned> idPtr = std::make_shared<unsigned>(id);

        // Create a format string with the desired substitution
        char scriptFunction[256];
        sprintf_s(scriptFunction, sizeof(scriptFunction), "%s.%s", name_script, name_function);
        Msg("~ Call scriptFunction: %s", scriptFunction);

        // Проверяем, выполняется ли уже скрипт
        if (scriptExecuting.load())
        {
            // Скрипт уже выполняется, отменяем вызов
            Msg("~ Script is already executing. Aborting TimerChainID.");
            return;
        }

        // Устанавливаем флаг выполнения скрипта
        scriptExecuting.store(true);

        // Используем мьютекс для защиты доступа к скрипту
        std::unique_lock<std::mutex> lock(scriptExecutionMutex, std::try_to_lock);
        if (!lock.owns_lock())
        {
            // Другой поток уже выполняет скрипт, отменяем вызов
            Msg("~ Script is already being executed by another thread. Aborting TimerChainID.");
            // Сбрасываем флаг выполнения скрипта
            scriptExecuting.store(false);
            return;
        }

        // Используйте лямбда-функцию для передачи ID и scriptFunction в callback1
        auto callback = [scriptFunction]() 
        {
            luabind::functor<void> m_functor1;

            if (ai().script_engine().functor(scriptFunction, m_functor1))
                m_functor1();
        };

        auto startTimerTask = [idPtr = std::move(idPtr), callback, start, end]() 
        {
            StartTimer(std::move(idPtr), callback, start, end);
        };

        ThreadPool::enqueue(startTimerTask);

        // Очистка содержимого массива scriptFunction
        Msg("~ Clear scriptFunction: %s", scriptFunction);
        memset(scriptFunction, 0, sizeof(scriptFunction));

        // Сбрасываем флаг выполнения скрипта
        scriptExecuting.store(false);
    }
} // namespace TimerID