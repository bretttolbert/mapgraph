#include "SvgFile.h"

namespace MapGraph
{
    SvgFile::SvgFile()
    {
    }

    void SvgFile::saveFile(const char* filename)
    {
        doc.SaveFile(filename);
    }
}