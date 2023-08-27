#pragma once
#include <iostream>
#include <unordered_map>
#include <string>

enum class Levels 
{
    jupiter = 1,
    k00_marsh,
    k01_darkscape,
    k02_trucks_cemetery,
    l01_escape,
    l02_garbage,
    l03_agroprom,
    l04_darkvalley,
    l05_bar,
    l06_rostok,
    l07_military,
    l08_yantar,
    l09_deadcity,
    l10_limansk,
    l10_radar,
    l10_red_forest,
    l11_hospital,
    l11_pripyat,
    l12_stancia,
    l12_stancia_2,
    l13_generators,
    pripyat,
    zaton,
    jupiter_underground,
    labx8,
    l03u_agr_underground,
    l04u_labx18,
    l08u_brainlab,
    l10u_bunker,
    l12u_control_monolith,
    l12u_sarcofag,
    l13u_warlab,
    fake_start,
    y04_pole
};

Levels getLevelFromString(const std::string& levelStr) 
{
    static std::unordered_map<std::string, Levels> levelMap = 
    {
        {"jupiter", Levels::jupiter},
        {"k00_marsh", Levels::k00_marsh},
        {"k01_darkscape", Levels::k01_darkscape},
        {"k02_trucks_cemetery", Levels::k02_trucks_cemetery},
        {"l01_escape", Levels::l01_escape},
        {"l02_garbage", Levels::l02_garbage},
        {"l03_agroprom", Levels::l03_agroprom},
        {"l04_darkvalley", Levels::l04_darkvalley},
        {"l05_bar", Levels::l05_bar},
        {"l06_rostok", Levels::l06_rostok},
        {"l07_military", Levels::l07_military},
        {"l08_yantar", Levels::l08_yantar},
        {"l09_deadcity", Levels::l09_deadcity},
        {"l10_limansk", Levels::l10_limansk},
        {"l10_radar", Levels::l10_radar},
        {"l10_red_forest", Levels::l10_red_forest},
        {"l11_hospital", Levels::l11_hospital},
        {"l11_pripyat", Levels::l11_pripyat},
        {"l12_stancia", Levels::l12_stancia},
        {"l12_stancia_2", Levels::l12_stancia_2},
        {"l13_generators", Levels::l13_generators},
        {"pripyat", Levels::pripyat},
        {"zaton", Levels::zaton},
        {"jupiter_underground", Levels::jupiter_underground},
        {"labx8", Levels::labx8},
        {"l03u_agr_underground", Levels::l03u_agr_underground},
        {"l04u_labx18", Levels::l04u_labx18},
        {"l08u_brainlab", Levels::l08u_brainlab},
        {"l10u_bunker", Levels::l10u_bunker},
        {"l12u_control_monolith", Levels::l12u_control_monolith},
        {"l12u_sarcofag", Levels::l12u_sarcofag},
        {"l13u_warlab", Levels::l13u_warlab},
        {"fake_start", Levels::fake_start},
        {"y04_pole", Levels::y04_pole}
    };

    auto it = levelMap.find(levelStr);

    if (it != levelMap.end()) 
    {
        return it->second;
    } 
    else 
    {
        throw std::invalid_argument("Invalid level string");
    }
}