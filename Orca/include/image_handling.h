#pragma once

namespace Orca
{
    class ImageHandling
    {
    public:
        static void initialize();
        static void saveRGBImage(const unsigned char *imgBuf, int w, int h, char const *filename);
        static void saveLImage(const unsigned char * imgBuf, int w, int h, char const * filename);
    };
}