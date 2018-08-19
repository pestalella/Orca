#include "stdafx.h"
#include "random.h"

std::default_random_engine generator;
std::uniform_real_distribution<float> uni01dist = std::uniform_real_distribution<float>(0.0f, 1.0f);

float Orca::Random::uniform01()
{
    return uni01dist(generator);
}
