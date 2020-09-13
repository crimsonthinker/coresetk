#include <accessor.hpp>
#include <iostream>

double Accessor::get_distance(double* c_coord, double **distance_map, int d1, int d2, int dim)
{
    int first = -1;
    int second = -1;
    if (d1 <= d2)
    {
        first = d1;
        second = d2 - d1;
    }
    else
    {
        first = d2;
        second = d1 - d2;
    }
    if (distance_map[first][second] < 0.0){
        distance_map[first][second] = Measure::euclide(c_coord + (d1 * dim), c_coord + (d2 * dim), dim);
    } 
    return distance_map[first][second];
}

void Accessor::set_distance(double **distance_map, int d1, int d2, double distance_value)
{
    int first = -1;
    int second = -1;
    if (d1 <= d2)
    {
        first = d1;
        second = d2 - d1;
    }
    else
    {
        first = d2;
        second = d1 - d2;
    }
    distance_map[first][second] = distance_value;
}