#include "stdafx.h"

#include "algebra.h"
#include "random.h"

namespace Orca {

    std::default_random_engine generator;
    std::uniform_real_distribution<float> uni01dist = std::uniform_real_distribution<float>(0.0f, 1.0f);
    std::uniform_real_distribution<float> unim1p1dist = std::uniform_real_distribution<float>(-1.0f, 1.0f);

    float Random::uniform01()
    {
        return uni01dist(generator);
    }

    float Random::uniform(float minval, float maxval)
    {
        std::uniform_real_distribution<float> uniformDist(minval, maxval);
        return uniformDist(generator);
    }

    int Random::uniformInt(int minval, int maxval)
    {
        std::uniform_int_distribution<int> uniformDist(minval, maxval);
        return uniformDist(generator);
    }

    void Random::initHalton()
    {
        curHaltonSample = 0;
    }

    Vec2f Random::uniform01Halton()
    {
        static const int curSample = 0;
        const int haltonBaseX = 2;
        const int haltonBaseY = 3;
        Vec2f result;

        // x axis
        result.x = 0.0f;
        {
            float denominator = float(haltonBaseX);
            size_t n = curSample;
            while (n > 0)
            {
                size_t multiplier = n % haltonBaseX;
                result.x += float(multiplier) / denominator;
                n = n / haltonBaseX;
                denominator *= haltonBaseX;
            }
        }

        // y axis
        result.y = 0.0f;
        {
            float denominator = float(haltonBaseY);
            size_t n = curSample;
            while (n > 0)
            {
                size_t multiplier = n % haltonBaseY;
                result.y += float(multiplier) / denominator;
                n = n / haltonBaseY;
                denominator *= haltonBaseY;
            }
        }
        curHaltonSample++;
        return result;
    }
}