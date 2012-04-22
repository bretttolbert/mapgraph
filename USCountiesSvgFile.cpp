#include <cstdlib>
#include <iostream>
#include <string>

#include "USCountiesSvgFile.h"
#include "StringUtils.h"

UsCountiesSvgFile::UsCountiesSvgFile()
{
    if (doc.LoadFile(USCOUNTIES_SVGFILE_FILENAME) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "failed to open " << USCOUNTIES_SVGFILE_FILENAME << std::endl;
        exit(1);
    }
}

bool UsCountiesSvgFile::markCountyByName(const std::string& name, const std::string& fill)
{
    tinyxml2::XMLElement* svg = doc.FirstChildElement("svg");
    tinyxml2::XMLElement* path = svg->FirstChildElement("path");

    do
    {
        const char* countyGeoid = path->Attribute("id");
        const char* countyName = path->Attribute("inkscape:label");
        if (countyName == NULL)
        {
            continue;
        }
        if (strcmp(countyName, name.c_str()) == 0)
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

