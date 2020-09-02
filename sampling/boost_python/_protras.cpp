#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/dict.hpp>
#include <math.h>
#include <limits>

typedef std::vector<double> Point;
typedef std::vector<int> Indices;
typedef std::set<int> IndicesSet;
typedef std::map<int, int> Rep;
typedef std::map<int, double> DisToRep;
typedef std::map<int, Point> Coord;
typedef std::map<int, std::map<int, double> &> DistanceMap;
typedef std::map<int, IndicesSet> RepSet;

class _ProTraS
{
private:
    boost::python::dict py_rep;
    boost::python::dict py_dis_to_rep;
    boost::python::dict py_rep_set;

    double tmp_euclide_distance(boost::python::list &p1, boost::python::list &p2)
    {
        double res = 0.0;
        for (int i = 0; i < boost::python::len(p1); i++)
        {
            //check key is string
            boost::python::extract<double> extractor_1(p1[i]);
            boost::python::extract<double> extractor_2(p1[i]);
            if (extractor_1.check() && extractor_2.check()){
                double v1 = extractor_1();
                double v2 = extractor_2();
                res += pow(v1 - v2, 2);
            }
        }
        return sqrt(res);
    }

    double get_distance(boost::python::dict &coord, DistanceMap &distance_map, int d1, int d2, std::string cal_mode)
    {
        if (cal_mode == "memory-based")
        {
            int first = -1;
            int second = -1;
            if (d1 <= d2)
            {
                first = d1;
                second = d2 - d1;
            }
            else
            {
                first = d2;
                second = d1 - d2;
            }
            if (distance_map.find(first) == distance_map.end())
            {
                distance_map[first] = std::map<int, double>();
            }
            boost::python::extract<boost::python::list> extractor_1(coord[first]);
            boost::python::extract<boost::python::list> extractor_2(coord[second]);
            if (extractor_1.check() && extractor_2.check()){
                boost::python::list l1 = extractor_1();
                boost::python::list l2 = extractor_2();
                distance_map[first][second] = tmp_euclide_distance(l1,l2);
            }
            return distance_map[first][second];
        }
        else
        {
            boost::python::extract<boost::python::list> extractor_1(coord[d1]);
            boost::python::extract<boost::python::list> extractor_2(coord[d2]);
            if (extractor_1.check() && extractor_2.check()){
                boost::python::list l1 = extractor_1();
                boost::python::list l2 = extractor_2();
                return tmp_euclide_distance(l1, l2);
            }
            return 0.0;
            
        }
    }

    void set_distance(DistanceMap &distance_map, int d1, int d2, double distance_value)
    {
        int first = -1;
        int second = -1;
        if (d1 <= d2)
        {
            first = d1;
            second = d2 - d1;
        }
        else
        {
            first = d2;
            second = d1 - d2;
        }
        if (distance_map.find(first) == distance_map.end())
        {
            //initialize
            distance_map[first] = std::map<int, double>();
        }
        distance_map[first][second] = distance_value;
    }

public:
    void run_protras(boost::python::dict &coord, double epsilon, std::string cal_mode)
    {
        DistanceMap distance_map;
        DisToRep dis_to_rep;
        Rep rep;
        Indices coreset_indices;

        RepSet rep_set;
        //initialize points: 0
        int initial_point_index = 0;

        rep[initial_point_index] = initial_point_index;

        rep_set[initial_point_index];
        rep_set[initial_point_index].insert(initial_point_index);

        distance_map[initial_point_index] = std::map<int, double>();
        distance_map[initial_point_index][initial_point_index] = 0.0;

        coreset_indices.push_back(initial_point_index);
        int data_size = boost::python::len(coord);
        double max_length = 0.0;

        boost::python::list keys = boost::python::list(coord.keys());
        //find maximum distance
        int target_idx = 0;
        for (ssize_t i = 0; i < boost::python::len(keys); i++)
        {
            //check key is string
            boost::python::extract<int> extractor(keys[i]);
            if (extractor.check())
            {
                int key = extractor();
                float distance = get_distance(coord, distance_map, initial_point_index, key, cal_mode);
                if (distance > max_length)
                {
                    max_length = distance;
                }
            }
        }

        //initialize representative
        for (ssize_t i = 0; i < boost::python::len(keys); i++)
        {
            //check key is string
            boost::python::extract<int> extractor(keys[i]);
            if (extractor.check())
            {
                int key = extractor();
                rep[key] = initial_point_index;
                if (key != initial_point_index)
                {
                    rep_set[initial_point_index].insert(key);
                    double distance = get_distance(coord, distance_map, initial_point_index, key, cal_mode);
                    dis_to_rep[key] = distance;
                }
                else
                {
                    dis_to_rep[key] = 0.0;
                }
            }
        }

        /****************************************************************************/
        double cost = 0.0;
        do
        {
            //Step 1: For each representative, find cost and largest distance
            cost = 0.0;
            int the_lion_king = -1;
            double max_wd = 0.0;
            double max_distance = 0.0;
            int largest_cost_rep_idx = 0;
            //for each representative
            for (auto const &x : rep_set)
            {
                int farthest_point_idx = -1;
                double norm_max_dist = 0.0;

                //for each index of representative set
                for (const int &idx : x.second)
                {
                    double current_distance = dis_to_rep[idx];
                    if (current_distance > norm_max_dist)
                    {
                        norm_max_dist = current_distance;
                        farthest_point_idx = idx;
                    }
                }

                double pk = x.second.size() * norm_max_dist;
                if (pk > max_wd)
                {
                    max_distance = norm_max_dist;
                    max_wd = pk;
                    the_lion_king = farthest_point_idx;
                    largest_cost_rep_idx = x.first;
                }

                cost += (pk / data_size * max_length);
            }
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            //step 2: add new point to the group
            rep_set[the_lion_king] = IndicesSet();
            rep_set[the_lion_king].insert(the_lion_king);
            distance_map[the_lion_king] = std::map<int, double>();
            distance_map[the_lion_king][the_lion_king] = 0.0;
            rep[the_lion_king] = the_lion_king;
            dis_to_rep[the_lion_king] = 0.0;

            //step 3: clear group
            for (const auto &x : rep_set)
            {
                rep_set[x.first].clear();
                rep_set[x.first].insert(x.first);
            }

            //Step 4: Find group for each representative
            //For each DataPoint
            boost::python::list keys = boost::python::list(coord.keys());
            for (ssize_t i = 0; i < boost::python::len(keys); i++)
            {
                //check key is int
                boost::python::extract<int> extractor(keys[i]);
                if (extractor.check())
                {
                    int key = extractor();
                    if (rep_set.find(key) == rep_set.end())
                    {
                        double norm_min_dist = std::numeric_limits<double>::max();
                        int rep_idx = -1;
                        //for each representative
                        for (const auto &r : rep_set)
                        {
                            //find distance
                            double distance = get_distance(coord, distance_map, key, r.first, cal_mode);
                            if (distance < norm_min_dist)
                            {
                                norm_min_dist = distance;
                                rep_idx = r.first;
                            }
                        }
                        //add index of point to representative
                        rep_set[rep_idx].insert(key);
                        if (cal_mode == "memory_based")
                        {
                            set_distance(distance_map, key, rep_idx, norm_min_dist);
                        }
                        rep[key] = rep_idx;
                        dis_to_rep[target_idx] = norm_min_dist;
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////////////////////////
            //Step 5: Add new representative - new member of coreset - THE LION KING - to variables
            coreset_indices.push_back(the_lion_king);
        } while (cost > epsilon && coreset_indices.size() < data_size);

        //convert to python objects
        //py_rep
        for (auto const& x: rep){
            this->py_rep[x.first] = x.second;
        }
        for (auto const& x: dis_to_rep){
            this->py_dis_to_rep[x.first] = x.second;
        }
        for(auto const& x: rep_set){
            boost::python::list tmp;
            for(auto v : x.second){
                tmp.append(v);
            }
            this->py_rep_set[x.first] = tmp;
        }
    }

    boost::python::dict get_rep(){
        return this->py_rep;
    }

    boost::python::dict get_dis_to_rep(){
        return this->py_dis_to_rep;
    }

    boost::python::dict get_rep_set(){
        return this->py_rep_set;
    }
};

BOOST_PYTHON_MODULE(protras_function)
{
    Py_Initialize();

    boost::python::class_<_ProTraS>("_ProTraS")
        .def("run_protras", &_ProTraS::run_protras)
        .def("get_rep", &_ProTraS::get_rep)
        .def("get_dis_to_rep", &_ProTraS::get_dis_to_rep)
        .def("get_rep_set", &_ProTraS::get_rep_set);
}
