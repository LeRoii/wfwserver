#ifndef PROJ_MAIN_HPP_
#define PROJ_MAIN_HPP_

#include <map>

#include "enemy_intention.hpp"
#include "kf_kalman.hpp"
#include "topsis_threat.hpp"
#include "proj_tool.hpp"
#include "algo.h"
#include "common.h"

proj_tool::ToolFun math_tool;
std::map<std::string, int> enemyid_map = {
    {"Soldier", 1},
    {"Fav", 2},
    {"Tank", 3},
    {"Armor", 4},
    {"Command", 5},
    {"Rover_para", 6},
    {"Fixuav_para", 7},
    {"person", 1},
    {"vehicle", 4}
};
std::map<int, std::string> intention_map = {
    {0, "attack"},
    {1, "backoff"},
    {2, "defend"},
    {3, "detour"}
};

#endif // PROJ_MAIN_HPP_