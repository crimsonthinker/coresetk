#include<protras.hpp>
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(core)
{
    Py_Initialize();
    boost::python::numpy::initialize();

    boost::python::class_<_ProTraS>("_ProTraS")
        .def("run_protras", &_ProTraS::run_protras)
        .def("get_rep", &_ProTraS::get_rep)
        .def("get_dis_to_rep", &_ProTraS::get_dis_to_rep)
        .def("get_rep_set", &_ProTraS::get_rep_set);
}
