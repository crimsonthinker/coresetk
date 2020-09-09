template <class T>
struct _Datapoint {
    int index;
    T value;
};

// int _Datapoint::_find(int index) {
//     for (int i = 0; i < this->size; i++) {
//         if (this->index_list[i] == index)
//             return i;
//     }
//     return -1;
// }

// template <class T>
// void _Datapoint::_append(int index, T value) {
//     if (_find(index) == -1) {
//         *(this->index_list + this->size) = index;
//         *(this->value_list + this->size) = value;
//         this->size++;
//     } else
//         std::cout << "Error: Duplicate index" << std::endl;
// }

// template <class T>
// void _Datapoint::_update(int index, T value) {
//     int position = _find(index);
//     if (position != -1) {
//         *(this->value_list + position) = value;
//         this->size++;
//     } else {
//         _append(index, value);
//     }
// }

// void _Datapoint::_sort() {

// }