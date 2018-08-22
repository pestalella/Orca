#pragma once
#include <random>
#include <functional>

namespace Orca {
    class Random {
    public:
        static float uniform01();
        static float uniform(float minval, float maxval);
    };
};
