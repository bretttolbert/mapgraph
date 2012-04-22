#ifndef USCOUNTIES_SVGFILE_H
#define USCOUNTIES_SVGFILE_H

#include "SvgFile.h"

#define USCOUNTIES_SVGFILE_FILENAME "USA_Counties_with_FIPS_and_names.svg"

class USCountiesSvgFile : public SvgFile
{
public:
    USCountiesSvgFile();
    bool markCountyByFips(int fips, const std::string& fill);
private:
    
};

#endif