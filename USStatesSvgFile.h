#ifndef US_STATES_SVG_FILE_H
#define US_STATES_SVG_FILE_H

#include "SvgFile.h"

namespace GraphGame
{
    #define US_STATES_SVG_FILENAME "Map_of_USA_without_state_names.svg"

    class USStatesSvgFile : public SvgFile
    {
    public:
        USStatesSvgFile();
        bool markStateByAbbreviation(const std::string& abbreviation, const std::string& fill);
    private:
    
    };
}
#endif
