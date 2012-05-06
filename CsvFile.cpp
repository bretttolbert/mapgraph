#include <iostream>
#include <fstream>
#include <cstdlib>

#include "CsvFile.h"
#include "StringUtils.h"
#include "Utils.h"

namespace GraphGame
{
    CsvFile::CsvFile(const std::string& filename)
    {
        std::ifstream file;
        file.open(filename.c_str(), std::ios::in);
        if (file.is_open())
        {
            int lineNum = 1;
            while (file.good())
            {
                std::string line;
                getline(file,line);
                line = StringUtils::strip(line, "\r\n");
                //convert '\,' escape sequences to '\x2c'
                line = StringUtils::replaceAll(line, "\\,", "\\x2c");
                if (!line.empty())
                {
                    std::vector<std::string> tokens = StringUtils::split(line,',');
                    std::vector<std::string>::iterator it;
                    for (it=tokens.begin(); it!=tokens.end(); ++it)
                    {
                        std::string& token = *it;
                        token = StringUtils::strip(token, " ");
                        //convert '\x2c' to ','
                        token = StringUtils::replaceAll(token, "\\x2c", ",");
                    }
                    if (lineNum == 1)
                    {
                        columnNames = tokens;
                    }
                    else
                    {
                        if (tokens.size() != columnNames.size())
                        {
                            std::cerr << "CSV Parser Error: Line " << lineNum << " Wrong number of columns\n";
                            std::cerr << "Found " << tokens.size() << " columns, expected " << columnNames.size() << "\n";
                            exit(1);
                        }
                        rows.push_back(tokens);
                    }
                    ++lineNum;
                }
            }
            file.close();
        }
        else
        {
            std::cerr << "Error: Failed to open file \"" << filename << "\"\n";
            exit(1);
        }
    }

    std::string CsvFile::toJSON() const
    {
        std::string json;
        json += "{\n\"rows:[\"";
        std::vector<std::vector<std::string> >::const_iterator it;
        for (it=rows.begin(); it!=rows.end(); ++it)
        {
            const std::vector<std::string>& row = *it;
            json += "  {\n";
            for (size_t i=0; i<columnNames.size(); ++i)
            {
                json += "    \"" + columnNames[i] + "\":\"" + row[i] + "\"";
                if (i != columnNames.size()-1)
                {
                    json += ",";
                }
                json += "\n";
            }
            json += "  }";
            if (next(it) != rows.end())
            {
                json += ",";
            }
            json += "\n";
        }
        json += "]\n}\n";
        return json;
    }
}
