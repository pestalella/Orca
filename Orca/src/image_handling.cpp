#include "stdafx.h"
#include "image_handling.h"

#include <iostream>

#ifdef _UNICODE
#undef _UNICODE
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#define _UNICODE
#endif 

void Orca::ImageHandling::initialize()
{
    ilInit();
    iluInit();
}

void Orca::ImageHandling::saveRGBImage(const char *imgBuf, int w, int h, char const *filename) {
    ILuint ImageName;
    ilGenImages(1, &ImageName);
    //        Now bind this image name so that DevIL performs all subsequent operations on this image:
    ilBindImage(ImageName);
    ILboolean result = ilTexImage(w, h, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, const_cast<char *>(imgBuf));
    if (result) {
        ilEnable(IL_FILE_OVERWRITE);
        result = ilSave(IL_PNG, filename);
        if (!result) {
            std::cout << "ilSave returned false. Something went wrong." << std::endl;
        }
    } else {
        std::cout << "ilTexImage returned false. Something went wrong." << std::endl;
    }
}
