#include <cstdlib>
#include <iostream>
#include <string>

#include "USStatesSvgFile.h"
#include "MapGraph.h"

namespace MapGraph
{
    USStatesSvgFile::USStatesSvgFile(IntegerIdAdjacencyListFile* adjacencyFile_)
        : adjacencyFile(adjacencyFile_)
    {
        std::string filepath = MapGraph::absolutePath(US_STATES_SVG_FILENAME);
        if (doc.LoadFile(filepath.c_str()) != tinyxml2::XML_SUCCESS)
        {
            std::cerr << "Error: Failed to open " << filepath << std::endl;
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

    bool USStatesSvgFile::markNode(int id, const std::string& fill)
    {
        std::string stateAbbreviation = adjacencyFile->nodeIdToString(id);
        if (stateAbbreviation.empty())
        {
            return false;
        }
        else
        {
            return markStateByAbbreviation(stateAbbreviation, fill);
        }
    }
}
