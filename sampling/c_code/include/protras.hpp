#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <boost/python/list.hpp>
#include <boost/python/dict.hpp>
#include <boost/python.hpp>
#include <math.h>
#include <limits>

typedef std::vector<double> Point;
typedef std::vector<int> Indices;
typedef std::set<int> IndicesSet;
typedef std::map<int, int> Rep;
typedef std::map<int, double> DisToRep;
typedef std::map<int, Point> Coord;
typedef std::map<int, std::map<int, double>> DistanceMap;
typedef std::map<int, IndicesSet> RepSet;

class _ProTraS
{
    private:
        boost::python::dict py_rep;
        boost::python::dict py_dis_to_rep;
        boost::python::dict py_rep_set;
        double tmp_euclide_distance(boost::python::list &p1, boost::python::list &p2);
        double get_distance(boost::python::dict &coord, DistanceMap &distance_map, int d1, int d2, std::string cal_mode);
        void set_distance(DistanceMap &distance_map, int d1, int d2, double distance_value);
    public:
        void run_protras(boost::python::dict &coord, double epsilon, std::string cal_mode);
        boost::python::dict get_rep();
        boost::python::dict get_dis_to_rep();
        boost::python::dict get_rep_set();
};