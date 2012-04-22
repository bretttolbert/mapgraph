#ifndef USCOUNTIES_SVGFILE_H
#define USCOUNTIES_SVGFILE_H

#include "SvgFile.h"

#define USCOUNTIES_SVGFILE_FILENAME "USA_Counties_with_FIPS_and_names.svg"

class UsCountiesSvgFile : public SvgFile
{
public:
    UsCountiesSvgFile();
    bool markCountyByName(const std::string& name, const std::string& fill);
private:
    
};

#endif