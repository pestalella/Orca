#pragma once
#include <random>
#include <functional>

namespace Orca {
    class Random {
        int curHaltonSample;

    public:
        static float uniform01();
        static float uniform(float minval, float maxval);
        static int uniformInt(int minval, int maxval);

        void initHalton();
        Vec2f uniform01Halton();


    };
};
