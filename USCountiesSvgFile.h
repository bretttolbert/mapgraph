#ifndef US_COUNTIES_SVG_FILE_H
#define US_COUNTIES_SVG_FILE_H

#include "SvgFile.h"

namespace GraphGame
{
    #define US_COUNTIES_SVG_FILENAME "USA_Counties_with_FIPS_and_names.svg"

    class USCountiesSvgFile : public SvgFile
    {
    public:
        USCountiesSvgFile();
        bool markCountyByFips(int fips, const std::string& fill);
        virtual bool markNode(int id, const std::string& fill);
    private:
    
    };
}
#endif
