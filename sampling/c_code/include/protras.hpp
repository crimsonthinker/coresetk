#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <boost/python/list.hpp>
#include <boost/python/dict.hpp>
#include <boost/python.hpp>
#include <math.h>
#include <limits>
#include <boost/python/numpy.hpp>
#include <measure.hpp>
#include <accessor.hpp>
#include <stdlib.h>
#include <time.h>

class _ProTraS{
    private:
        double cost = 0.0;
        double max_length = 0.0;
    protected:
        //attribute
        int data_size = 0; //mainly used for deleting
        int rep_size = 0;
        int dim = 0;
        std::string cal_mode = "memory-based"; //or ram-based
        double epsilon = 0.015;
        bool **rep_table = NULL;
        double *rep_dist = NULL;
        double **distance_map = NULL;
        //function
        void rep_grouping(double *);
        void find_new_rep();
        void check_signal();
    public:
        //function
        ~_ProTraS();
        void set_cal_mode(std::string);
        void set_epsilon(double);
        virtual void run(const boost::python::numpy::ndarray&);
        boost::python::list get_rep_list();
        boost::python::list get_rep_of_point_list();
        boost::python::list get_rep_dist_list();
};