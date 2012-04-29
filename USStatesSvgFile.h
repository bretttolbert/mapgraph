#ifndef US_STATES_SVG_FILE_H
#define US_STATES_SVG_FILE_H

#include "SvgFile.h"
#include "IntegerIdAdjacencyListFile.h"

namespace GraphGame
{
    #define US_STATES_SVG_FILENAME "svg/Map_of_USA_without_state_names.svg"

    class USStatesSvgFile : public SvgFile
    {
    public:
        USStatesSvgFile(IntegerIdAdjacencyListFile* adjacencyFile);
        USStatesSvgFile() { }
        bool markStateByAbbreviation(const std::string& abbreviation, const std::string& fill);
        virtual bool markNode(int id, const std::string& fill);
    private:
        IntegerIdAdjacencyListFile* adjacencyFile;
    };
}
#endif
