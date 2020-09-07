#include <modification_protras.hpp>
#include <dict.cpp>
#include <limits>
#include <string>


namespace bp = boost::python

bool exist(int *check_array, int array_size, int element) {
    for (int i = 0; i < array_size; i++) {
    if (*(check_array + i) == element)
    return true;
  }
  return false;
}

void _ModificationProTraS::run_modification_protras(bp::list &coreset_index, bp::list &substituted_list, bp:dict &py_rep_set, bp::numpy::ndarray &coord) {
    int data_size = coord.shape(0);
    int dim = coord.shape(1);
    float **c_coord = reinterpret_cast<float **>(coord.get_data());

    bp.
    
    int current_coreset_size = bp::len(coreset_index);
    int *whole_list = new int [current_coreset_size];
    int *unsubstituted_list = new int [current_coreset_size];
    int *subs_list = new int;
    int subs_list_size = 0;

    for (int i = 0; i < current_corset_size; i++) {
        *(whole_list + i) = coreset_index[i];
        *(unsubstituted_list + i) = coreset_index[i];
    }

    if (current_coreset_size == 1) {
        *subs_list = *whole_list;
    } else {
        int decrease_size = current_coreset_size;
        while (decrease_size > 0) {
            bool self_point = false;

            // get last element
            int the_index = *unsubstituted_list;
            unsubstituted_list = unsubstituted_list + 1;
            decrease_size--;

            int *current_substituted = new int;
            int *old_substituted = new int;
            int current_substituted_size = 0;

            *current_substituted = the_index;
            current_substituted_size++;
            *old_substituted = the_index;

            float **distance_map;
            for (int i = 0; i < data_size; i++)
            {
                distance_map[i] = new float[data_size - i]{-1.0};
            }

            do
            {
                float min_distance = std::numeric_limits<float>::max();
                int min_coreset_index = -1;

                // 1. find index nearest to the_index
                // find closest x in whole_list compared to current_substituted
                for (int i = 0; i < current_substituted_size; i++) {
                    the_index = *(current_substituted + i);
                    
                    // prim algorithm based
                    for (int j = 0; j < current_coreset_size; j++) {
                        if (*(whole_list + j) != the_index) {
                            float distance = _get_distance(c_coord, distance_map, *(whole_list + j), the_index, dim);
                            if (distance < min_distance) {
                                min_distance = distance;
                                min_coreset_index = *(whole_list + j);
                            }
                        }
                    }

                }

                // check if self_point should be true
                // if min_coreset_index in the current_substituted itself, or it has been substituted before
                if (exist(current_substituted, current_substituted_size, min_coreset_index) || exist(subs_list, subs_list_size, min_coreset_index)) {
                    self_point = true;

                    // substitute every point x in current_substituted
                    for (int i = 0; i < current_substituted_size; i++) {
                        // group_datapoint: list of point index belonging to rep point whose index is x
                        int *group_datapoint = new int;
                        for 
                    }
                }

            } while (/* condition */);
            
        }
    }

}