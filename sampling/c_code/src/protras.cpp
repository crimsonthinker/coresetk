#include <protras.hpp>

float _ProTraS::_euclide_distance(float *p1, float *p2, int dim)
{
    double res = 0.0;
    for (int i = 0; i < dim; i++)
    {
        res += pow(p1[i] - p2[i], 2);
    }
    return sqrt(res);
}

float _ProTraS::_get_distance(float **c_coord, float **distance_map, int d1, int d2, int dim)
{
    if (this->cal_mode == "memory-based")
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
        if (distance_map[first][second] < 0.0)
        {
            float distance = _euclide_distance(c_coord[d1], c_coord[d2], dim);
            distance_map[first][second] = distance;
        }
        return distance_map[first][second];
    }
    else
    {
        return _euclide_distance(c_coord[d1], c_coord[d2], dim);
    }
}

void _ProTraS::_set_distance(float **distance_map, int d1, int d2, float distance_value)
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

void _ProTraS::set_eps(float eps)
{
    epsilon = eps;
}

void _ProTraS::set_cal_mode(std::string mode)
{
    if (mode != "memory-based" && mode != "ram-based")
    {
        std::cout << "Unknown calculation mode: Choosing default";
        this->cal_mode = "ram-based";
    }
    else
    {
        this->cal_mode = mode;
    }
}

void _ProTraS::run_protras(boost::python::numpy::ndarray &coord,
                           boost::python::list &py_dis_to_rep,
                           boost::python::dict &py_rep_set)
{
    //Get shape of data
    int data_size = coord.shape(0);
    int dim = coord.shape(1);

    //convert coord to C++ matrix
    float **c_coord = reinterpret_cast<float **>(coord.get_data());

    //initialize supporting variable
    float **distance_map;
    if (this->cal_mode == "memory-based")
    {
        distance_map = new float *[data_size];
    }
    bool **rep = new bool *[data_size];
    float *dis_to_rep = new float[data_size];
    for (int i = 0; i < data_size; i++)
    {
        if (this->cal_mode == "memory-based")
        {
            distance_map[i] = new float[data_size - i]{-1.0};
        }
        rep[i] = new bool[data_size];
        dis_to_rep[i] = -1.0;
    }

    //initialize points: 0
    int initial_point_index = 0;

    //initialize representative
    double max_length = 0.0;
    rep[initial_point_index][initial_point_index] = true;
    for (int i = 0; i < data_size; i++)
    {
        rep[initial_point_index][i] = true;
        float distance = _get_distance(c_coord, distance_map, initial_point_index, i, dim);
        dis_to_rep[i] = distance;
        if (distance > max_length)
        {
            max_length = distance;
        }
    }

    /****************************************************************************/
    double cost = 0.0;
    int coreset_size = 0;
    do
    {
        //Step 1: For each representative, find cost and largest distance
        cost = 0.0;
        int the_lion_king = -1;
        double max_wd = 0.0;
        double max_distance = 0.0;
        int largest_cost_rep_idx = 0;
        //for each representative
        for (int i = 0; i < data_size; i++)
        {
            int rep_size = 0;
            if (rep[i][i] == true)
            { //it is the rep
                int farthest_point_idx = -1;
                double norm_max_dist = 0.0;

                //for each member point of representative
                for (int t = 0; t < data_size; t++)
                {
                    if (rep[i][t] == true)
                    { //it belongs to rep
                        rep_size += 1;
                        double current_distance = dis_to_rep[t];
                        if (current_distance > norm_max_dist)
                        {
                            norm_max_dist = current_distance;
                            farthest_point_idx = t;
                        }
                        //reset representative after calculating
                        if (i != t)
                        { //if it is not itself
                            rep[i][t] = false;
                        }
                    }
                }

                //calculate cost
                double pk = rep_size * norm_max_dist;
                if (pk > max_wd)
                {
                    max_distance = norm_max_dist;
                    max_wd = pk;
                    the_lion_king = farthest_point_idx;
                    largest_cost_rep_idx = i;
                }
                cost += (pk / (data_size * max_length));
            }
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        //step 2: add new point as the representative
        rep[the_lion_king][the_lion_king] = true;
        dis_to_rep[the_lion_king] = 0.0;

        //Step 3: Find group for each representative
        //For each DataPoint
        for (int i = 0; i < data_size; i++)
        {
            if (rep[i][i] == false) //not a rep
            {
                double norm_min_dist = std::numeric_limits<double>::max();
                int rep_idx = -1;
                //for each representative
                for (int t = 0; t < data_size; t++)
                {
                    if (rep[t][t] == true) //is a rep
                    {
                        double distance = _get_distance(c_coord, distance_map, i, t, dim);
                        if (distance < norm_min_dist)
                        {
                            norm_min_dist = distance;
                            rep_idx = t;
                        }
                    }
                }
                //add index of point to representative
                rep[rep_idx][i] = true;
                if (cal_mode == "memory_based")
                {
                    _set_distance(distance_map, i, rep_idx, norm_min_dist);
                }
                dis_to_rep[i] = norm_min_dist;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        std::cout << cost << "\n";
        coreset_size += 1;
    } while (cost > epsilon && coreset_size < data_size);

    //parse result back to parameter
    py_dis_to_rep = boost::python::list();
    py_rep_set = boost::python::dict();

    for (int i = 0; i < data_size; i++)
    {
        py_dis_to_rep.append(dis_to_rep[i]);
    }
    for (int x = 0; x < data_size; x++)
    {
        if (rep[x][x] == true)
        { //line of rep
            py_rep_set[x] = boost::python::list();
            boost::python::list holder;
            for (int y = 0; y < data_size; y++)
            {
                if (rep[x][y] == true)
                {
                    holder.append(y);
                }
                py_rep_set[x] = holder;
            }
        }
    }
} 