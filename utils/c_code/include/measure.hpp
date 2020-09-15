#pragma once

#include <math.h>

struct Measure{
    static double euclide(double*, double*, int);
    static double manhattan(double*, double*, int);
    static double cosine_value_from_points(double*, double*, double*,  int);
};