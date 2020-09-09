#include <modification_protras.hpp>
#include <merge_sort.cpp>
#include <datapoint.cpp>
#include <dict.cpp>
#include <limits>
#include <string>
#include <cmath>

namespace bp = boost::python;

int exist(int *check_array, int array_size, int element)
{
    for (int i = 0; i < array_size; i++)
    {
        if (*(check_array + i) == element)
            return i;
    }
    return -1;
}

template <class T>
void delete_value(T* arr, int arr_size, T value) {
    bool found_delete = false;

    for (int i = 0; i < arr_size; i++) {
      if arr[i] == value)
        found_delete = true;

      if (found_delete && i < arr_size)
        *(arr + i) = *(arr + (i+1));   
    }
  }

float *_ModificationProTraS::_make_vector(float **c_coord, int d1, int d2, int dim)
{
    float *new_vector = new float[dim];
    for (int i = 0; i < dim; i++)
    {
        new_vector[i] = c_coord[d2][i] - c_coord[d1][i];
    }
    return new_vector;
}

float _ModificationProTraS::_cosine_angle(float *first_vector, float *second_vector, int dim)
{
    float numerator, first_denominator, second_denominator = 0.0;

    for (int i = 0; i < dim; i++)
    {
        numerator += first_vector[i] * second_vector[i] first_denominator += first_vector[i] * first_vector[i] second_denominator += second_vector[i] * second_vector[i]
    }

    return numerator / (sqrt(first_denominator) * sqrt(second_denominator));
}

float _ModificationProTraS::_get_cosine(float **c_coord, float **distance_map, int d1, int d2, int d3, int dim)
{
    float *first_vector = _make_vector(c_coord, d2, d1, dim);
    float *second_vector = _make_vector(c_coord, d2, d3, dim);

    return _cosine_angle(first_vector, second_vector, dim);
}

void _ModificationProTraS::run_modification_protras(bp
                                                    : dict &py_rep_set, bp::dict &coreset_index, bp::list &substituted_list, bp::numpy::ndarray &coord)
{
    int data_size = coord.shape(0);
    int dim = coord.shape(1);
    float **c_coord = reinterpret_cast<float **>(coord.get_data());

    bp::list rep_keys = bp::list(py_rep_set.keys());
    int representative_size = bp::len(rep_keys);
    Dict<int> representative;

    for (int i = 0; i < representative_size; i++)
    {
        int coreset_group_size = bp::len(py_rep_set[rep_keys[i]]) int *rep_group = new int;
        int *rep_group_index = new int;
        for (int j = 0; j < coreset_group_size; j++)
        {
            *(rep_group + j) = py_rep_set[rep_keys[i][j]
            *(rep_group_index + j) = coreset_index[rep_keys[i][j]
        }

        representative.append(i, coreset_group_size, rep_group, rep_group_index);
    }

    int *whole_list = new int[current_coreset_size];
    int *unsubstituted_list = new int[current_coreset_size];
    int *subs_list = new int;
    int subs_list_size = 0;
    int unsubs_list_size = representative_size;

    for (int i = 0; i < representative_size; i++)
    {
        *(whole_list + i) = rep_keys[i];
        *(unsubstituted_list + i) = rep_keys[i];
    }

    if (current_coreset_size == 1)
    {
        *subs_list = *whole_list;
    }
    else
    {
        int decrease_size = current_coreset_size;
        while (decrease_size > 0)
        {
            bool self_point = false;

            // get first element
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
                for (int i = 0; i < current_substituted_size; i++)
                {
                    the_index = *(current_substituted + i);

                    // prim algorithm based
                    for (int j = 0; j < current_coreset_size; j++)
                    {
                        if (*(whole_list + j) != the_index)
                        {
                            float distance = _get_distance(c_coord, distance_map, *(whole_list + j), the_index, dim);
                            if (distance < min_distance)
                            {
                                min_distance = distance;
                                min_coreset_index = *(whole_list + j);
                            }
                        }
                    }
                }

                // check if self_point should be true
                // if min_coreset_index in the current_substituted itself, or it has been substituted before
                if (exist(current_substituted, current_substituted_size, min_coreset_index) != -1|| exist(subs_list, subs_list_size, min_coreset_index) != -1)
                {
                    self_point = true;

                    // substitute every point x in current_substituted
                    for (int i = 0; i < current_substituted_size; i++)
                    {
                        // group_datapoint_cosine: list of point index belonging to rep point whose index is x
                        _Datapoint<float> *group_datapoint_cosine = new _Datapoint<float>;
                        int *current_rep = representative.get(current_substituted[i]);
                        int new_size = 0

                            for (int j = 0; j < current_rep.value_size; j++)
                        {
                            if (*(current.index + j) != current_substituted[i])
                            {
                                _Datapoint<float> tmp_point;
                                tmp_point.index = *(current.index + j)
                                                       tmp_point.value = _get_cosine(c_coord, distance_map, min_coreset_index, current_substituted[i], group_datapoint_cosine[j], dim);
                                *(group_datapoint_cosine + j) = tmp_point;
                                new_size++;
                            }
                        }
                        // this array is sorted from smallest to largest
                        merge_sort(group_datapoint_cosine, 0, new_size - 1)

                        // get the index
                        int percentage_cosine = round(this->percentage * (new_size - 1));
                        // get threshold value from index
                        float percentage_cosine_value = group_datapoint_cosine[percentage_cosine].value;

                        // get datapoint that form a cosine value larger than threshold
                        // the larger the percentage, the larger the threshold value, the more compact cluster is
                        int compact_size = 0;
                        _Datapoint<float> *group_datapoint_distance_compact = new _Datapoint<float>;
                        // there is other points
                        // Using percentage to get the point that is nth close to rep
                        for (int j = 0; j < new_size; j++)
                        {
                            if (group_datapoint_cosine[j].value >= percentage_cosine_value)
                            {
                                _Datapoint<float> tmp_point;
                                tmp_point.index = group_datapoint_cosine[j].index;
                                tmp_point.value = _get_distance(c_coord, distance_map, current_substituted[i], tmp_point.index, dim);
                                *(group_datapoint_distance_compact + j) = tmp_point;
                                compact_size++;
                            }
                        }

                        merge_sort(group_datapoint_distance_compact, 0, compact_size - 1, false);
                        int order = round((1 - this->percentage) * (compact_size + 1));
                        int new_coreset_index = group_datapoint_distance_compact[order].index;

                        // update representative
                        if (new_coreset_index != -1) {
                            int *the_list = representative.get_value(current_substituted[i]);
                            representative.append(new_coreset_index, representative.get_value_size(current_substituted[i]), the+list);
                            representative.delete_key(current_substituted[i]);

                            // update whole_list
                            int whole_list_index = exist(whole_list, representative_size, current_substituted[i]);
                            whole_list[whole_list_index] = new_coreset_index;

                            // update current_substittued
                            current_substituted[i] = new_coreset_index;
                        }
                    }
                } else {
                    *(current_substituted + current_substituted_size) = min_coreset_index;
                    *(old_substituted + current_substituted_size) = min_coreset_index;
                    current_substituted_size++
                }
            } while (!self_point);

            // add current_substituted to substituted_list, remove them from unsubstituted_lsit
            for (int i = 0; i < current_substituted_size; i++) {
                substituted_list.append(current_substituted[i]);
                if (exist(old_substituted, current_substituted_size, unsubstituted_list[i]) != -1) {
                    delete_value(unsubstituted_list, unsubs_list_size, unsubstituted_list[i]);
                    unsubs_list_size--;
                }
            }
        }
    }
}