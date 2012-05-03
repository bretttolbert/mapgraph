#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>

#include "../GraphGame.h"
#include "../CsvAdjacencyListFile.h"
#include "../USCountiesAdjacencyListFile.h"
#include "../IntegerIdAdjacencyListFile.h"
#include "../StringUtils.h"
#include "../Utils.h"

#define DEBUG 0

namespace GraphGame
{
    void die(const std::string& errorMsg)
    {
    #if !DEBUG
        std::cout << "Content-Type: text/plain;charset=us-ascii\n\n";
    #endif
        std::cout << "Error: " << errorMsg << std::endl;
        exit(1);
    }

    void runCgi(const std::string& queryStringOverride)
    {
    #if DEBUG
        std::cout << "Content-Type: text/plain;charset=us-ascii\n\n";
    #endif
        std::string rawQuery;
        if (!queryStringOverride.empty())
        {
            rawQuery = queryStringOverride;
        }
        else if (!getenv("QUERY_STRING"))
        {
            die("failed to get query string");
        }
        else
        {
            rawQuery = getenv("QUERY_STRING");
        }
    #if DEBUG
        std::cout << "query: " << rawQuery << std::endl;
    #endif
        std::vector<std::string> rawParams = StringUtils::split(rawQuery, '&');
        std::map<std::string,std::string> params;
        std::vector<std::string>::const_iterator it;
        for (it=rawParams.begin(); it!=rawParams.end(); ++it)
        {
            std::vector<std::string> paramTokens = StringUtils::split(*it, '=');
            if (paramTokens.size() != 2)
            {
                die("parser: paramsTokens.size() != 2\n");
            }
            params[paramTokens[0]] = StringUtils::urldecode(paramTokens[1]);
        }
    #if DEBUG
        std::cout << "params:\n";
        std::map<std::string,std::string>::const_iterator param_it;
        for (param_it=params.begin(); param_it!=params.end(); ++param_it)
        {
            std::cout << param_it->first << "=\"" << param_it->second << "\"\n";
        }
    #endif

        if (!params.count("action"))
        {
            die("missing required parameter \"action\"\n");
        }

        if (params["action"] == "cli")
        {
            std::cout << "Content-Type: text/html;charset=us-ascii\n\n";
            std::cout << "<!DOCTYPE html>\n";
            std::cout << "<html>\n";
            std::cout << "<head>\n";
            std::cout << "<title>graphgame</title>\n";
            std::cout << "</head>\n";
            std::cout << "<body>\n";
            std::cout << "<textarea rows=\"30\" cols=\"80\">";
            if (system(NULL))
            {
                std::string cmd = "../graphgame ";
                cmd += params["args"] + " > out.txt 2>&1";
        #if DEBUG
                std::cout << "cmd: " << cmd << std::endl;
        #endif
                system(cmd.c_str());
                std::ifstream file;
                file.open("out.txt", std::ios::in);
                if (file.is_open())
                {
                    while (file.good())
                    {
                        std::string line;
                        getline(file, line);
                        std::cout << line << "\n";
                    }
                }
                else
                {
                    die("Failed to open out.txt\n");
                }
            }
            else
            {
                die("Processor is not available.\n");
            }
            std::cout << "</textarea>\n";
            //std::cout << "<br>\n";
            std::cout << "<img src=\"output/output.svg\" width=\"" << (555*2) << "\" height=\"" << (352*2) << "\" />\n";
            std::cout << "</body>\n";
            std::cout << "</html>\n";
        }
        else if (params["action"] == "getNodes")
        {
            if (!params.count("af"))
            {
                die("Missing required parameter \"af\"");
            }

            IntegerIdAdjacencyListFile* af;
            if (params["af"] == "us-states")
            {
                af = new CsvAdjacencyListFile(GraphGame::absolutePath("48US.txt"));
            }
            else if (params["af"] == "us-counties")
            {
                af = new USCountiesAdjacencyListFile();
            }
            else
            {
                die("Unrecognized adjacency file preset");
            }

            std::cout << "Content-Type: application/json;charset=us-ascii\n\n";
            std::cout << "{\n";

            const IntegerIdAdjacencyListFile::NodeIdToNodeStringMap& nodeIdToNodeStringMap
                = af->getNodeIdToNodeStringMap();
            IntegerIdAdjacencyListFile::NodeIdToNodeStringMap::const_iterator it;
            std::cout << "  \"nodes\":[";
            for (it=nodeIdToNodeStringMap.begin(); it!=nodeIdToNodeStringMap.end(); ++it)
            {
                std::cout << "{\n";
                std::cout << "    \"nodeId\":" << it->first << ",\n";
                std::cout << "    \"nodeString\":" << "\"" << it->second << "\",\n";
                std::cout << "    \"neighbors\":[";
                const std::set<int>& neighbors = af->getNeighbors(it->first);
                std::set<int>::const_iterator jt;
                for (jt=neighbors.begin(); jt!=neighbors.end(); ++jt)
                {
                    std::cout << *jt;
                    if (next(jt) != neighbors.end())
                    {
                        std::cout << ",";
                    }
                }
                std::cout << "]\n";
                std::cout << "  }";
                if (next(it) != nodeIdToNodeStringMap.end())
                {
                    std::cout << ",";
                }
            }
            std::cout << "]";
            std::cout << "\n}\n";
        }
        else if (params["action"] == "getAdjacencyList")
        {
        }
        else
        {
            die("Unrecognized action");
        }
    }
}

int main(int argc, char** argv)
{
    std::string queryStringOverride; //for debugging
    if (argc > 1)
    {
        queryStringOverride = argv[1];
    }
    GraphGame::runCgi(queryStringOverride);
    return 0;
}
