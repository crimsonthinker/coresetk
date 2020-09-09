#include <iostream>
#include <string>
using namespace std;

// @open source
// see https://www.softwaretestinghelp.com/merge-sort/

template <class T>
void merge(T *, int, int, int);

template <class T>
void merge_sort(T *arr, int low, int high, bool asc = true)
{
    int mid;
    if (low < high)
    {
        // divide the array at mid and sort independently using merge sort
        mid = (low + high) / 2;
        merge_sort(arr, low, mid, asc);
        merge_sort(arr, mid + 1, high, asc);
        // merge or conquer sorted arrays
        merge(arr, low, high, mid, asc);
    }
}
// Merge sort
template <typename T>
void merge(T *arr, int low, int high, int mid, bool asc)
{
    std::string arr_type = typeid(arr).name();
    bool is_datapoint_type = (hihi.find("_Datapoint") >= 0 && hihi.find("_Datapoint") < arr_type.length()) ? true : false;

    int i, j, k;
    T c[5000];
    i = low;
    k = low;
    j = mid + 1;

    while (i <= mid && j <= high)
    {
        if (asc)
        {
            if (is_datapoint_type)
            {
                if (arr[i].value < arr[j].value)
                {
                    c[k] = arr[i];
                    k++;
                    i++;
                }
                else
                {
                    c[k] = arr[j];
                    k++;
                    j++;
                }
            }
            else
            {
                if (arr[i] < arr[j])
                {
                    c[k] = arr[i];
                    k++;
                    i++;
                }
                else
                {
                    c[k] = arr[j];
                    k++;
                    j++;
                }
            }
        }
        else
        {
            if (is_datapoint_type)
            {
                if (arr[i].value > arr[j].value)
                {
                    c[k] = arr[i];
                    k++;
                    i++;
                }
                else
                {
                    c[k] = arr[j];
                    k++;
                    j++;
                }
            }
            else
            {
                if (arr[i] > arr[j])
                {
                    c[k] = arr[i];
                    k++;
                    i++;
                }
                else
                {
                    c[k] = arr[j];
                    k++;
                    j++;
                }
            }
        }
    }
    while (i <= mid)
    {
        c[k] = arr[i];
        k++;
        i++;
    }
    while (j <= high)
    {
        c[k] = arr[j];
        k++;
        j++;
    }
    for (i = low; i < k; i++)
    {
        arr[i] = c[i];
    }
}