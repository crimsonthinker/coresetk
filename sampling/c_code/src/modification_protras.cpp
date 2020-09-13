#include <modification_protras.hpp>

void _ModificationProTraS::set_percentage(double per){
    if (per < 0.0 || per > 1.0)
        std::cout << "Invalid percentage. Percentage not change\n";
    else
        percentage = per;
}

void _ModificationProTraS::run(const boost::python::numpy::ndarray& numpy){
    //run protras first
    _ProTraS::run(numpy);

    //convert coord to C++ 1d pointer
    double* c_coord = reinterpret_cast<double*>(numpy.get_data());

    //begin performing modification
    std::set<int> whole_list;
    std::set<int> unshifted_list;
    std::set<int> shifted_list;

    //filtering using mean
    double mean_rep_size = percentage * (data_size / rep_size);
    for(int i = 0 ; i < data_size ; i++){
        int count = 0;
        if(rep_table[i][i] == true){
            for(int t = 0 ; t < data_size ; t++)
                if(rep_table[i][t] == true)
                    count += 1;
            if (count < mean_rep_size) //disqualified
                for(int t = 0 ; t < data_size ; t++)
                    rep_table[i][t] = false;
            else{
                whole_list.insert(i);
                unshifted_list.insert(i);
            }
        }
        check_signal();
    }

    //shifting
    if(whole_list.size() == 1)
        shifted_list = whole_list;
    else
        while(unshifted_list.size() > 0){
            bool self_point = false;
            //get an element
            auto it = unshifted_list.begin();
            int the_index = *it;
            unshifted_list.erase(it);
            std::set<int> current_shifted_list;
            current_shifted_list.insert(the_index);
            check_signal();
            do{
                double min_dist = std::numeric_limits<double>::max();
                int min_coreset_index = -1;
                //1. find index nearest to the_index
                for(auto& cfl : current_shifted_list){
                    the_index = cfl;
                    //prim based
                    for(auto& wl : whole_list){
                        if(wl != the_index){
                            double distance;
                            if(cal_mode =="memory-based")
                                distance = Accessor::get_distance(c_coord, distance_map, wl, the_index, dim);
                            else
                                distance = Measure::euclide(c_coord + (wl * dim), c_coord + (the_index * dim), dim);
                            if(distance < min_dist){
                                min_dist = distance;
                                min_coreset_index = wl;
                            }
                        }
                    }
                    check_signal();
                }

                //2. check if self_point should be true
                if(current_shifted_list.find(min_coreset_index) != current_shifted_list.end() ||
                shifted_list.find(min_coreset_index) != shifted_list.end()){
                    self_point = true;
                    //shifted every representative in current_shifted_list
                    for(auto& cfl : current_shifted_list){
                        //calculate cosine value for each member of rep group (except itself)
                        std::vector<std::pair<int,double>> member_cosine_value;
                        for(int i = 0 ; i < data_size ; i++)
                            if(rep_table[cfl][i] == true && cfl != i){ //it is a memeber in rep set
                                double cosine_value = Measure::cosine_value_from_points(c_coord + (min_coreset_index * dim), 
                                c_coord + (cfl * dim), c_coord + (i * dim), dim);
                                member_cosine_value.push_back(std::make_pair(i,cosine_value));
                            }

                        //sort vector
                        std::sort(member_cosine_value.begin(), member_cosine_value.end(),
                        [](const std::pair<int,double> &a, const std::pair<int,double> &b) -> bool{
                            return a.second < b.second;
                        });
                        //choose substituted point based on percentage
                        int percentage_index = (int) (percentage * (member_cosine_value.size() - 1));
                        //get cosine value from percentage_index, for filtering purpose
                        double percentage_cosine_value = member_cosine_value[percentage_index].second;
                        //count members whose cosine is larger than percentage_cosine_value
                        int larger_count = 0;
                        for(auto& mcv : member_cosine_value){
                            if(mcv.second >= percentage_cosine_value)
                                larger_count += 1;
                        }
                        //get order
                        int order = (int) ((1 - percentage) * (larger_count - 1) > 0.0 ? (1 - percentage) * (larger_count - 1) : -1.0);
                        if(order < 0){ //larger count is 0 -> no points -> move to other rep
                            continue;
                        }
                        int count = 0;
                        int new_coreset_index = -1;
                        for(int i = member_cosine_value.size() - 1 ; i >= 0 ; i--){
                            if(order == count){
                                new_coreset_index = member_cosine_value[i].first;
                                break;
                            }else if(member_cosine_value[i].second >= percentage_cosine_value)
                                count += 1;
                        }
                        //apply old rep list on new rep
                        rep_table[new_coreset_index][new_coreset_index] = true;
                        rep_table[cfl][cfl] = false;
                        //swapping address
                        bool* &holder = rep_table[new_coreset_index];
                        rep_table[new_coreset_index] = rep_table[cfl];
                        rep_table[cfl] = holder;
                        //replace rep in whole_list
                        whole_list.erase(whole_list.find(cfl), whole_list.end());
                        whole_list.insert(new_coreset_index);
                        //replace rep in current_shifted_list
                        current_shifted_list.erase(current_shifted_list.find(cfl), current_shifted_list.end());
                        current_shifted_list.insert(new_coreset_index);
                        check_signal();
                    }
                }else{
                    current_shifted_list.insert(min_coreset_index);
                }
            }while(!self_point);

            //merge shifted_list with current_shifted_list
            //remove element from unshifted_list with current_shifted_list
            for(auto cfl : current_shifted_list){
                shifted_list.insert(cfl);
                unshifted_list.erase(unshifted_list.find(cfl));
            }
        }
    //update rep_table and dis_to_rep
    rep_grouping(c_coord);
}