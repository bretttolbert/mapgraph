#ifndef SVGFILE_H
#define SVGFILE_H

#include <string>

#include "tinyxml2/tinyxml2.h"

namespace MapGraph
{
    class SvgFile
    {
    public:
        SvgFile();
        void saveFile(const char* filename);
        virtual bool markNode(int id, const std::string& fill) = 0;
    protected:
        tinyxml2::XMLDocument doc;
    };
}

#endif
