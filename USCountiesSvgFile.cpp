#include <cstdlib>
#include <iostream>
#include <string>

#include "USCountiesSvgFile.h"
#include "USCountiesAdjacencyList.h" //for fipsToString
#include "StringUtils.h"

USCountiesSvgFile::USCountiesSvgFile()
{
    if (doc.LoadFile(USCOUNTIES_SVGFILE_FILENAME) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "failed to open " << USCOUNTIES_SVGFILE_FILENAME << std::endl;
        exit(1);
    }
}

bool USCountiesSvgFile::markCountyByFips(int fips, const std::string& fill)
{
    tinyxml2::XMLElement* svg = doc.FirstChildElement("svg");
    tinyxml2::XMLElement* path = svg->FirstChildElement("path");
    const char* fipsStr = USCountiesAdjacencyList::fipsToString(fips).c_str();

    do
    {
        const char* countyFips = path->Attribute("id");
        //const char* countyName = path->Attribute("inkscape:label");
        if (countyFips == NULL)
        {
            continue;
        }
        if (strcmp(countyFips, fipsStr) == 0)
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