#pragma once

#include <iostream>
#include <vector>
#include <math.h>


class Interpolator
{
public:
    int type_ = 0;

    Interpolator(int type):
        type_(type)
    {
    }
    
    static const int CATMULL_ROM = 0;

    virtual std::pair<int, int> operator()(float t, std::pair<int, int> point_0, std::pair<int, int> point_1, std::pair<int, int> point_2, std::pair<int, int> point_3);
};