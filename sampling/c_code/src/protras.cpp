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

void _ProTraS::run_protras(boost::python::numpy::ndarray &coord, double epsilon, std::string cal_mode)
{
    //Get shape of data
    int data_size = coord.shape(0);
    int dim = coord.shape(1);

    //convert coord to C++ array
    float** c_coord = reinterpret_cast<float**>(coord.get_data());

    //initialize supporting variable
    float **distance_map = new float*[data_size];
    bool **rep = new bool*[data_size];
    float *dis_to_rep = new float[data_size];
    for(int i = 0 ; i < data_size ; i++){
        if (cal_mode == "memory-based"){
            distance_map[i] = new float[data_size - i];
        }
        rep[i] = new bool[data_size];
        dis_to_rep[i] = -1.0;
    }
    
    //initialize points: 0
    int initial_point_index = 0;
    

    //initialize representative
    rep[initial_point_index][initial_point_index] = true;
    for(int i = 0 ; i < data_size ; i++)
    {
        rep[initial_point_index][i] = true
        double distance = get_distance(c_coord, distance_map, initial_point_index, i, cal_mode);
        dis_to_rep[i] = distance;
    }

    //find maximum distance
    double max_length = 0.0;
    for(int i = 0 ; i < data_size ; i++)
    {
        float distance = get_distance(c_coord, distance_map, initial_point_index, x.first, cal_mode);
        if (distance > max_length)
        {
            max_length = distance;
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
        for(int i = 0 ; i < data_size ; i++)
        {
            int rep_size = 0
            if(rep[i][i] == true){ //it is the rep
                int farthest_point_idx = -1;
                double norm_max_dist = 0.0;

                //for each member point of representative
                for(int t = 0 ; t < data_size ; t++)
                {
                    if (rep[i][t] == true){ //it belongs to rep, but not rep
                        rep_size += 1
                        double current_distance = dis_to_rep[t];
                        if (current_distance > norm_max_dist)
                        {
                            norm_max_dist = current_distance;
                            farthest_point_idx = t;
                        }
                        //reset representative after calculating
                        if(i != t){ //if it is not itself
                            rep[i][t] = false
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
        rep[the_lion_king][the_lion_king] = true
        dis_to_rep[the_lion_king] = 0.0;

        //Step 3: Find group for each representative
        //For each DataPoint
        for(int i = 0 ; i < data_size ; i++)
        {
            if (rep[i][i] == false) //not a rep
            {
                double norm_min_dist = std::numeric_limits<double>::max();
                int rep_idx = -1;
                //for each representative
                for(int t = 0 ; t < data_size ; t++)
                {
                    if(rep[t][t] == true) //is a rep
                    { 
                        double distance = get_distance(c_coord, distance_map, i, t, cal_mode);
                        if (distance < norm_min_dist)
                        {
                            norm_min_dist = distance;
                            rep_idx = t
                        }
                    }
                }
                //add index of point to representative
                rep[rep_idx][i] = true
                if (cal_mode == "memory_based")
                {
                    set_distance(distance_map, i, rep_idx, norm_min_dist);
                }
                dis_to_rep[i] = norm_min_dist;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        std::cout << cost << "\n";
    } while (cost > epsilon && coreset_indices.size() < data_size);
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