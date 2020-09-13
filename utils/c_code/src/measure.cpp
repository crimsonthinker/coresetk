#include <measure.hpp>

double Measure::euclide(double* p1, double* p2, int dim){
    double res = 0.0;
    for (int i = 0; i < dim; i++)
    {
        res += pow(p1[i] - p2[i], 2);
    }
    return sqrt(res);
}

double Measure::manhattan(double* p1, double* p2, int dim){
    double res = 0.0;
    for (int i = 0; i < dim; i++)
    {
        res += abs(p1[i] - p2[i]);
    }
    return res;
}

double Measure::cosine_value_from_points(double *b, double *a, double *c, int dim){
    double numerator = 0.0;
    double first_denominator = 0.0;
    double second_denominator = 0.0;
    for(int i = 0 ; i < dim ; i++){
        double p1 = b[i] - a[i];
        double p2 = c[i] - a[i];
        numerator += p1 * p2;
        first_denominator += p1 * p1;
        second_denominator += p2 * p2;
    }
    return numerator / (sqrt(first_denominator) * sqrt(second_denominator));
}