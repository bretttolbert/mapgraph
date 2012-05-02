#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <vector>
#include <string>

class StringUtils
{
public:
    static std::vector<std::string> split(const std::string& str, char delim);
    static std::string join(const std::vector<std::string>& tokens, char delim);
    static std::string urldecode(const std::string& str);        
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
    static std::string randomColor();
};

#endif
