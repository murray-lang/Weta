//
// Created by murray on 5/01/17.
//
#include <math.h>

// Workaround for missing library function in ESP-IDF sdk
// see: https://github.com/espressif/esp-idf/issues/83

double __ieee754_remainder(double x, double y)
{
    return x - y * floor(x/y);
}

