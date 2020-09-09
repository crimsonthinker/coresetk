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
        float percentage = 0.5;
        float* _make_vector(float **, int, int, int);
        float _get_cosine(float **, float **, int, int, int, int);
        float _cosine_angle(float *, float *, int);
    
    public:
        void run_modification_protras(boost::python::list&, boost::python::list&, boost::python::numpy::ndarray&);
};

