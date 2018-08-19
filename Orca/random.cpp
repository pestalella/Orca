#include "stdafx.h"
#include "random.h"

std::default_random_engine generator;
std::uniform_real_distribution<float> uni01dist = std::uniform_real_distribution<float>(0.0f, 1.0f);
std::uniform_real_distribution<float> unim1p1dist = std::uniform_real_distribution<float>(-1.0f, 1.0f);

float Orca::Random::uniform01()
{
    return uni01dist(generator);
}

float Orca::Random::uniform(float minval, float maxval)
{
    std::uniform_real_distribution<float> uniformDist(minval, maxval);
    return uniformDist(generator);
}
