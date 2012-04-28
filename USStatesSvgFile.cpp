#include <cstdlib>
#include <iostream>
#include <string>

#include "USStatesSvgFile.h"

namespace GraphGame
{
    extern bool showWarnings;

    USStatesSvgFile::USStatesSvgFile()
    {
        if (doc.LoadFile(US_STATES_SVG_FILENAME) != tinyxml2::XML_SUCCESS)
        {
            std::cerr << "Error: Failed to open " << US_STATES_SVG_FILENAME << std::endl;
            exit(1);
        }
    }

    bool USStatesSvgFile::markStateByAbbreviation(const std::string& abbreviation, const std::string& fill)
    {
        tinyxml2::XMLElement* svg = doc.FirstChildElement("svg");
        tinyxml2::XMLElement* g = svg->FirstChildElement("g");
        tinyxml2::XMLElement* path = g->FirstChildElement("path");
        do
        {
            const char* id = path->Attribute("id");
            if (id == NULL)
            {
                continue;
            }
            else if (strcmp(id, abbreviation.c_str()) == 0)
            {
                path->SetAttribute("fill", fill.c_str());
                return true;
            }
        } while ((path = path->NextSiblingElement()) != NULL);
        return false;
    }
}
