#include "SvgFile.h"

SvgFile::SvgFile()
{
}

void SvgFile::saveFile(const char* filename)
{
    doc.SaveFile(filename);
}