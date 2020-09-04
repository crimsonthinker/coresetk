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
#include <protras.hpp>

class _ModificationProTraS : private _ProTraS
{
    private:
        boost::python::list py_dis_to_rep;
        boost::python::dict py_rep_set;
        boost::python::list py_coreset_indices;
        double tmp_euclide_distance(Point&, Point&);
        double get_distance(Coord&, DistanceMap&, int, int, std::string);
        void set_distance(DistanceMap &, int, int, double);
    public:
        void run_protras(boost::python::list&, double, std::string);
        boost::python::list get_coreset_indices();
        boost::python::dict get_rep();
        boost::python::dict get_dis_to_rep();
        boost::python::dict get_rep_set();
};