#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>

#include "../StringUtils.h"

#define DEBUG 1

int main(int argc, char** argv)
{
    std::cout << "Content-Type: text/plain;charset=us-ascii\n\n";
    if (!getenv("QUERY_STRING"))
    {
        std::cout << "Error: failed to get QUERY_STRING\n";
        exit(1);
    }
    std::string rawQuery = getenv("QUERY_STRING");
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
            std::cout << "Parser Error: paramsTokens.size() != 2\n";
            exit(1);
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
            std::cerr << "Error: Failed to open out.txt\n";
        }
    }
    else
    {
        std::cout << "Error: Processor is not available.\n";
    }
    return 0;
}
