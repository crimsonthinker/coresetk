#include<protras.hpp>
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(core)
{
    Py_Initialize();
    boost::python::numpy::initialize();

    boost::python::class_<_ProTraS>("_ProTraS")
        .def("set_eps", &_ProTraS::set_eps)
        .def("run_protras", &_ProTraS::run_protras)
        .def("set_cal_mode", &_ProTraS::set_cal_mode);
}
