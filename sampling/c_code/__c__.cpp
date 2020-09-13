#include<protras.hpp>
#include<modification_protras.hpp>

BOOST_PYTHON_MODULE(__c__)
{
    Py_Initialize();
    boost::python::numpy::initialize();

    boost::python::class_<_ProTraS>("_ProTraS")
        .def("set_eps", &_ProTraS::set_eps)
        .def("run", &_ProTraS::run)
        .def("set_cal_mode", &_ProTraS::set_cal_mode)
        .def("get_rep_list", &_ProTraS::get_rep_list)
        .def("get_rep_of_point_list", &_ProTraS::get_rep_of_point_list)
        .def("get_rep_dist_list", &_ProTraS::get_rep_dist_list);

    boost::python::class_<_ModificationProTraS>("_ModificationProtraS")
        .def("set_percentage", &_ModificationProTraS::set_percentage)
        .def("run", &_ModificationProTraS::run);
}
