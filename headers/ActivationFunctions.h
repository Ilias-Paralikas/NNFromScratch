#include <tgmath.h>
//sigmoids
float ActivationFunction(float x)
{
    return (1 / (1 + exp(-x)));
}
//sigmoid prime
float ActivationPrime(float x)
{
    float temp = ActivationFunction(x);
    return temp * (1 - temp);
}
