#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <boost/python/list.hpp>
#include <boost/python/dict.hpp>
#include <boost/python.hpp>
#include <math.h>
#include <limits>
#include <boost/python/numpy.hpp>
class _ProTraS
{
    private:
        float epsilon = 0.015;
        std::string cal_mode;
        float _euclide_distance(float*, float*, int);
        float _get_distance(float**, float**, int, int, int);
        void _set_distance(float**, int, int, float);
    public:
        void set_cal_mode(std::string);
        void set_eps(float);
        void run_protras(boost::python::numpy::ndarray&,
        boost::python::list&, boost::python::dict&);
};