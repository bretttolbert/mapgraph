#include <cstdlib>
#include <iostream>
#include <string>

#include "USCountiesSvgFile.h"
#include "USCountiesAdjacencyListFile.h" //for fipsToString
#include "StringUtils.h"

namespace GraphGame
{
    extern bool showWarnings;

    USCountiesSvgFile::USCountiesSvgFile()
    {
        if (doc.LoadFile(US_COUNTIES_SVG_FILENAME) != tinyxml2::XML_SUCCESS)
        {
            std::cerr << "Error: Failed to open " << US_COUNTIES_SVG_FILENAME << std::endl;
            exit(1);
        }
    }

    bool USCountiesSvgFile::markCountyByFips(int fips, const std::string& fill)
    {
        tinyxml2::XMLElement* svg = doc.FirstChildElement("svg");
        tinyxml2::XMLElement* path = svg->FirstChildElement("path");
        std::string fipsStr = USCountiesAdjacencyListFile::fipsToString(fips);
        do
        {
            const char* countyFips = path->Attribute("id");
            //const char* countyName = path->Attribute("inkscape:label");
            if (countyFips == NULL)
            {
                continue;
            }
            else if (fipsStr == countyFips)
            {
                std::string style = path->Attribute("style");
                std::vector<std::string> tokens = StringUtils::split(style, ';');
                std::vector<std::string>::iterator it;
                for (it = tokens.begin(); it != tokens.end(); ++it)
                {
                    std::vector<std::string> subtokens = StringUtils::split(*it, ':');
                    if (subtokens[0] == "fill")
                    {
                        *it = std::string("fill:") + fill;
                    }
                }
                style = StringUtils::join(tokens, ';');
                path->SetAttribute("style", style.c_str());
                return true;
            }
        } while ((path = path->NextSiblingElement()) != NULL);
        return false;
    }

    bool USCountiesSvgFile::markNode(int id, const std::string& fill)
    {
        return markCountyByFips(id, fill);
    }
}
