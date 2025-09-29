#pragma once
#ifndef CHINESE_WALL_H   
#define CHINESE_WALL_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
using namespace std;

struct Operation {
    string type; 
    int object_id;
    string firm;
};

void runChineseWall();

#endif 
