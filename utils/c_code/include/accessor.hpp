#pragma once

#include <math.h>
#include <measure.hpp>

struct Accessor{
    static double get_distance(double*, double**, int, int, int);
    static void set_distance(double**, int, int, double);
};