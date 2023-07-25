#pragma once

#include "json.h"
#include "Main.h"

#include <fstream>
#include <iomanip>
#include <initializer_list>
#include <iostream>
#include <string>

namespace JSON
{
    JSON_API void CreateTimerTableJson(const std::string& filename, unsigned id, unsigned start, unsigned end);

	template <typename... Args>
    static void TemplateJSONFile(const std::string& filename, Args&&... args)
    {
        json timerData = json::object({{"key", std::forward<Args>(args)}...});
        std::ofstream outputFile(filename);

        if (!outputFile.is_open())
            return;

        outputFile << std::setw(4) << timerData;
        outputFile.close();
    }
}