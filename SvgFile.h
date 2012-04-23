#ifndef SVGFILE_H
#define SVGFILE_H

#include "tinyxml2\tinyxml2.h"

namespace GraphGame
{
    class SvgFile
    {
    public:
        SvgFile();
        void saveFile(const char* filename);
    protected:
        tinyxml2::XMLDocument doc;
    };
}

#endif