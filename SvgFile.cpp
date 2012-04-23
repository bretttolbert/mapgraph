#include "SvgFile.h"

namespace GraphGame
{
    SvgFile::SvgFile()
    {
    }

    void SvgFile::saveFile(const char* filename)
    {
        doc.SaveFile(filename);
    }
}