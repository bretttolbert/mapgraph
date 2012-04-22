#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <vector>
#include <string>

class StringUtils
{
public:
    static std::vector<std::string> split(const std::string& str, char delim);
    static bool iequals(std::string s1, std::string s2);
    class CaseInsensitiveComparator 
    {
    public:
        CaseInsensitiveComparator(const std::string& strToMatch);
        bool operator() (std::string s);
        void setMatchTarget(const std::string& strToMatch);
    private:
        std::string matchTarget;
    };
};

#endif