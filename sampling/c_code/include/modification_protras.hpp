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
    public:
        void run_modification_protras(boost::python::list&, boost::python::list&, boost::python::numpy::ndarray&);
};