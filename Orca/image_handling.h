#pragma once

namespace Orca
{
    class ImageHandling
    {
    public:
        static void initialize();
        static void saveRGBImage(const char *imgBuf, int w, int h, char const *filename);
    };
}