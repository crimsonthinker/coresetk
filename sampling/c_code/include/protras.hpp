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
        double epsilon = 0.015;
        std::string cal_mode;
        double _euclide_distance(double*, double*, int);
        double _get_distance(double*, double**, int, int, int);
        void _set_distance(double**, int, int, double);
    public:
        void set_cal_mode(std::string);
        void set_eps(double);
        void run_protras(boost::python::numpy::ndarray&,
        boost::python::list&, boost::python::dict&);
};