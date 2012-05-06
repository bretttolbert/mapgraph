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
    /**
     * Strips leading and trailing characters matching charsToStrip
     * E.g. strip whitespace: myString = strip(myString, " \r\n");
     */
    static std::string strip(const std::string& str, const std::string& charsToStrip);

    /**
     * Replace all occurences of a with b
     */ 
    static std::string replaceAll(const std::string& str, const std::string& a, const std::string& b);

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
