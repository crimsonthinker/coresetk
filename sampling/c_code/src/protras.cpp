#include <protras.hpp>

double _ProTraS::tmp_euclide_distance(Point &p1, Point &p2)
{
    double res = 0.0;
    for (int i = 0; i < p1.size(); i++)
    {
        res += pow(p1[i] - p2[i], 2);
    }
    return sqrt(res);
}

double _ProTraS::get_distance(Coord &c_coord, DistanceMap &distance_map, int d1, int d2, std::string cal_mode)
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
        float distance = tmp_euclide_distance(c_coord[first], c_coord[second]);
        distance_map[first][second] = tmp;
        return distance_map[first][second];
    }
    else
    {
        return tmp_euclide_distance(c_coord[d1], c_coord[d2]);
    }
}

void _ProTraS::set_distance(DistanceMap &distance_map, int d1, int d2, double distance_value)
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
    distance_map[first][second] = distance_value;
}

void _ProTraS::run_protras(boost::python::list &coord, double epsilon, std::string cal_mode)
{
    int data_size = boost::python::len(coord);

    FMatrix distance_map;
    FPoint dis_to_rep = FPoint(-1.0, data_size);
    BMatrix rep = IPoint(-1, data_size);
    Marker coreset_indices = Marker(false,data_size);
    FMatrix c_coord = Coord();

    if (cal_mode == "memory-based"){
        distance_map = DistanceMap(-1.0,data_size);
    }

    //convert boost::python::list to Coord
    for (ssize_t i = 0; i < boost::python::len(coord); i++)
    {

        boost::python::extract<boost::python::list> extractor_point(coord[i]);
        if (extractor_point.check())
        {
            boost::python::list point = extractor_point();
            c_coord[key] = Point();
            for (ssize_t t = 0; t < boost::python::len(l); t++)
            {
                //check key is string
                boost::python::extract<float> extractor_float(l[t]);
                if (extractor_float.check())
                {
                    float f = extractor_float();
                    c_coord[key].push_back(f);
                }
            }
        }
    }
    double max_length = 0.0;

    //initialize variables
    if (cal_mode == "memory-based"){
        for(int i = data_size - 1 ; i >= 0 ; i--){
            distance_map.push_back(Point(-1.0,i));
        }
    }


    //initialize points: 0
    int initial_point_index = 0;
    rep[initial_point_index] = true;
    rep_set[initial_point_index][initial_point_index] = true;
    set_distance(distance_map, initial_point_index, initial_point_index, 0.0);
    coreset_indices[initial_point_index] = true

    //find maximum distance
    for (auto const &x : c_coord)
    {
        float distance = get_distance(c_coord, distance_map, initial_point_index, x.first, cal_mode);
        if (distance > max_length)
        {
            max_length = distance;
        }
    }

    //initialize representative
    for (auto const &x : c_coord)
    {
        rep[x.first] = initial_point_index;
        if (x.first != initial_point_index)
        {
            rep_set[initial_point_index].push_back(x.first);
            double distance = get_distance(c_coord, distance_map, initial_point_index, x.first, cal_mode);
            dis_to_rep[x.first] = distance;
        }
        else
        {
            dis_to_rep[x.first] = 0.0;
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

            cost += (pk / (data_size * max_length));
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        //step 2: add new point to the group
        rep_set[the_lion_king] = IndicesSet();
        rep_set[the_lion_king].push_back(the_lion_king);
        if (cal_mode == "memory-based"){
            set_distance(distance_map, the_lion_king, the_lion_king, 0.0);
        }
        rep[the_lion_king] = the_lion_king;
        dis_to_rep[the_lion_king] = 0.0;

        //step 3: clear group
        for (const auto &x : rep_set)
        {
            rep_set[x.first].clear();
            rep_set[x.first].reserve(data_size);
            rep_set[x.first].push_back(x.first);
        }

        //Step 4: Find group for each representative
        //For each DataPoint
        for (auto const &x : c_coord)
        {
            if (rep_set.find(x.first) == rep_set.end())
            {
                double norm_min_dist = std::numeric_limits<double>::max();
                int rep_idx = -1;
                //for each representative
                for (const auto &r : rep_set)
                {
                    //find distance
                    double distance = get_distance(c_coord, distance_map, x.first, r.first, cal_mode);
                    if (distance < norm_min_dist)
                    {
                        norm_min_dist = distance;
                        rep_idx = r.first;
                    }
                }
                //add index of point to representative
                rep_set[rep_idx].push_back(x.first);
                if (cal_mode == "memory_based")
                {
                    set_distance(distance_map, x.first, rep_idx, norm_min_dist);
                }
                rep[x.first] = rep_idx;
                dis_to_rep[x.first] = norm_min_dist;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        //Step 5: Add new representative - new member of coreset - THE LION KING - to variables
        coreset_indices.push_back(the_lion_king);
        std::cout << cost << "\n";
    } while (cost > epsilon && coreset_indices.size() < data_size);

    //convert to python objects
    //py_rep
    for (auto const &x : rep)
    {
        this->py_rep[x.first] = x.second;
    }
    for (auto const &x : dis_to_rep)
    {
        this->py_dis_to_rep[x.first] = x.second;
    }
    for (auto const &x : rep_set)
    {
        boost::python::list tmp;
        for (auto v : x.second)
        {
            tmp.append(v);
        }
        this->py_rep_set[x.first] = tmp;
    }
    for (auto const &x : coreset_indices)
    {
        this->py_coreset_indices.append(x);
    }
}

    boost::python::list _ProTraS::get_coreset_indices(){
        return this->py_coreset_indices;
    }

    boost::python::dict _ProTraS::get_rep()
    {
        return this->py_rep;
    }

    boost::python::dict _ProTraS::get_dis_to_rep()
    {
        return this->py_dis_to_rep;
    }

    boost::python::dict _ProTraS::get_rep_set()
    {
        return this->py_rep_set;
    }