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
#include <accessor.hpp>
#include <utility>

class _ModificationProTraS: protected _ProTraS{
    private:
        double percentage;
    public:
        void set_percentage(double);
        void run(const boost::python::numpy::ndarray&);
};