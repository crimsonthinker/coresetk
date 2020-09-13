#include <protras.hpp>
#include <iostream>
#include <fstream>

_ProTraS::~_ProTraS(){
    if (rep_table){
        for(int i = 0 ; i < data_size ; i++)
            delete[] rep_table[i];
        delete[] rep_table;
    }

    if(distance_map){
        for(int i = 0 ; i < data_size ; i++)
            delete[] distance_map[i];
        delete[] distance_map;
    }

    if(rep_dist)
        delete[] rep_dist;
}

void _ProTraS::set_eps(double eps){
    epsilon = eps;
}

void _ProTraS::set_cal_mode(std::string mode){
    if(mode != "memory-based" && mode != "ram-based")
        // BOOST_LOG_TRIVIAL(warning) << "Unknown calculation mode: Choosing default";
        cal_mode = "ram-based";
    else
        cal_mode = mode;
    
}

void _ProTraS::rep_grouping(double *c_coord){
    for(int i = 0 ; i < data_size ; i++){
        if (rep_table[i][i] == false) //not a rep
        {
            double norm_min_dist = std::numeric_limits<double>::max();
            int rep_idx = -1;
            //for each representative
            for(int t = 0 ; t < data_size ; t++){
                if(rep_table[t][t] == true){ //it is a rep 
                    double distance;
                    if(cal_mode =="memory-based")
                        distance = Accessor::get_distance(c_coord, distance_map, t, i, dim);
                    else
                        distance = Measure::euclide(c_coord + (t * dim), c_coord + (i * dim), dim);
                    if (distance < norm_min_dist){
                        norm_min_dist = distance;
                        rep_idx = t;
                    }
                }
                check_signal();
            }
            //add index of point to representative
            rep_table[rep_idx][i] = true;
            if (cal_mode == "memory_based")
                Accessor::set_distance(distance_map, i, rep_idx, norm_min_dist);
            rep_dist[i] = norm_min_dist;
        }
    }
}

void _ProTraS::find_new_rep(){
    //Find cost and largest distance
    cost = 0.0;
    int the_lion_king = -1;
    double max_wd = 0.0;
    double max_distance = 0.0;
    int largest_cost_rep_idx = 0;
    //for each representative
    for(int i = 0 ; i < data_size ; i++){
        int rep_set_size = 0;
        if(rep_table[i][i] == true){ //it is the rep
            int farthest_point_idx = -1;
            double norm_max_dist = 0.0;

            //for each member point of representative
            for(int t = 0 ; t < data_size ; t++)
            {
                if (rep_table[i][t] == true){ //it belongs to rep
                    rep_set_size += 1;
                    double current_distance = rep_dist[t];
                    if (current_distance > norm_max_dist){
                        norm_max_dist = current_distance;
                        farthest_point_idx = t;
                    }
                    //reset representative after calculating
                    if(i != t) //if it is not itself
                        rep_table[i][t] = false;
                }
                check_signal();
            }

            //calculate cost
            double pk = rep_set_size * norm_max_dist;
            if (pk > max_wd){
                max_distance = norm_max_dist;
                max_wd = pk;
                the_lion_king = farthest_point_idx;
                largest_cost_rep_idx = i;
            }
            cost += (pk / (data_size * max_length));
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    rep_table[the_lion_king][the_lion_king] = true;
    rep_dist[the_lion_king] = 0.0;
}

void _ProTraS::check_signal(){
    if(PyErr_CheckSignals() == -1)
        exit(1);
}

void _ProTraS::run(const boost::python::numpy::ndarray &numpy)
{
     //check data dimension
    int data_dim = numpy.get_nd();
    if (data_dim != 2)
        return;

    //Get shape of data
    data_size = numpy.shape(0);
    dim = numpy.shape(1);
    if(dim != 2)
        return;

    //convert coord to C++ 1d pointer
    double* c_coord = reinterpret_cast<double*>(numpy.get_data());

    //check pointer before running protras
    if (rep_table){
        for(int i = 0 ; i < data_size ; i++)
            delete[] rep_table[i];
        delete[] rep_table;
    }
    rep_table = NULL;

    if(rep_dist)
        delete[] rep_dist;
    rep_dist = NULL;

    if(cal_mode == "memory_based"){
        if(distance_map){
            for(int i = 0 ; i < data_size ; i++)
                delete[] distance_map[i];
            delete[] distance_map;
        }
        distance_map = NULL;
    }
    
    //initialize supporting variable
    if(cal_mode == "memory-based")
        distance_map = new double*[data_size];
    rep_table = new bool*[data_size];
    rep_dist = new double[data_size];
    for(int i = 0 ; i < data_size ; i++){
        rep_table[i] = new bool[data_size];
        for(int t = 0 ; t < data_size ; t++)
            rep_table[i][t] = false;
        rep_dist[i] = -1.0;
        if(cal_mode == "memory-based"){
            distance_map[i] = new double[data_size - i];
            for(int t = 0 ; t < data_size - i ; t++)
                distance_map[i][t] = -1.0;
        }
    }
    
    //initialize random point
    srand(time(NULL));
    int initial_point_index = rand() % data_size;
    

    //initialize representative
    max_length = 0.0;
    rep_table[initial_point_index][initial_point_index] = true;
    for(int i = 0 ; i < data_size ; i++){
        rep_table[initial_point_index][i] = true;
        double distance;
        if(cal_mode =="memory-based")
            distance = Accessor::get_distance(c_coord, distance_map, initial_point_index, i, dim);
        else
            distance = Measure::euclide(c_coord + (initial_point_index * dim), c_coord + (i * dim), dim);
        rep_dist[i] = distance;
        if (distance > max_length)
            max_length = distance;
    }

    /****************************************************************************/
    cost = 0.0;
    rep_size = data_size > 0 ? 1 : 0;

    //catch interrupt signal
    check_signal();
    do{
        //step 0: check signal
        check_signal();

        //step 1: Find new rep
        find_new_rep();

        //Step 2: Find group for each representative
        rep_grouping(c_coord);

        rep_size += 1;
    } while (cost > epsilon && rep_size < data_size);
}

boost::python::list _ProTraS::get_rep_list(){
    //return list of rep after running protras
    boost::python::list result;
    if(rep_size)
        for(int i = 0 ; i < data_size ; i++)
            if(rep_table[i][i] == true)
                result.append(i);
    return result;
}

boost::python::list _ProTraS::get_rep_of_point_list(){
    boost::python::list result;
    if(rep_size)
        for(int i = 0 ; i < data_size ; i++) //for each point
            for(int t = 0 ; t < data_size ; t++) //for each rep
                if(rep_table[t][i] == true)
                    result.append(t);
    return result;
}

boost::python::list _ProTraS::get_rep_dist_list(){
    boost::python::list result;
    if(rep_size)
        for(int i = 0 ; i < data_size ; i++)
            result.append(rep_dist[i]);
    return result;
}