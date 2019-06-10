#include "Utils.hpp"

#include <cmath>

#define PI 3.14159265358979323846


float cosDecay(float x, float p) {
    if (x<=0) return 1.0;
    if (x>=1) return 0.0;
    return -cos(PI*pow(1-x,p))/2.0 +0.5;
}

float cosCurve(float x, float p) {
    if (x<=-1 || x>=1) return 0.0;
    return cos(pow(fabs(x),p)*PI/2.0);
}