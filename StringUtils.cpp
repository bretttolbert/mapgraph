#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "StringUtils.h"

std::vector<std::string> StringUtils::split(const std::string& str, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss(str);
    std::string token;
    while(std::getline(ss, token, delim)) 
    {
        elems.push_back(token);
    }
    return elems;
}

std::string StringUtils::join(const std::vector<std::string>& tokens, char delim)
{
    std::string output;
    std::vector<std::string>::const_iterator it;
    for (it = tokens.begin(); it != tokens.end(); ++it)
    {
        output += *it;
        std::vector<std::string>::const_iterator jt = it;
        if (++jt != tokens.end())
        {
            output += delim;
        }
    }
    return output;
}

std::string StringUtils::urldecode(const std::string& str)
{
    std::string result;
    std::string::const_iterator it;
    for (it=str.begin(); it!=str.end(); ++it)
    {
        if (*it == '+')
        {
            result += ' ';
        }
        else if (*it == '%')
        {
            std::string hexCode;
            std::string::const_iterator next = it;
            if (++next != str.end() && *next != '%')
            {
                hexCode += *(++it);
                if (++it != str.end())
                {
                    hexCode += *it;
                    std::stringstream ss;
                    ss << std::hex << hexCode;
                    unsigned int x;
                    ss >> x;
                    result += (char)x;
                }
            }
        }
        else
        {
            result += *it;
        }
    }
    return result;
}

bool StringUtils::iequals(std::string s1, std::string s2)
{
    std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
    return (s1 == s2);
}

std::string StringUtils::strip(const std::string& str, const std::string& charsToStrip)
{
    std::string result;
    size_t goodPos = str.find_first_not_of(charsToStrip);
    if (goodPos != std::string::npos)
    {
        result = str.substr(goodPos);
        std::reverse(result.begin(), result.end());
        goodPos = result.find_first_not_of(charsToStrip);
        if (goodPos != std::string::npos)
        {
            result = result.substr(goodPos);
        }
        std::reverse(result.begin(), result.end());
    }
    return result;
}

std::string StringUtils::replaceAll(const std::string& str, const std::string& a, const std::string& b)
{
    std::string result = str;
    size_t pos = 0;
    while((pos = result.find(a, pos)) != std::string::npos) 
    {
        result.replace(pos, a.length(), b);
        pos += b.length();
    }
    return result;
}

StringUtils::CaseInsensitiveComparator::CaseInsensitiveComparator(const std::string& strToMatch)
{
    setMatchTarget(strToMatch);
}

bool StringUtils::CaseInsensitiveComparator::operator() (std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return (s == matchTarget);
}

void StringUtils::CaseInsensitiveComparator::setMatchTarget(const std::string& strToMatch)
{
    matchTarget = strToMatch;
    std::transform(matchTarget.begin(), matchTarget.end(), matchTarget.begin(), ::tolower);
}

std::string StringUtils::randomColor()
{
    std::ostringstream oss;
    oss << '#' << std::hex << std::setfill('0')
        << std::setw(2) << (rand() % 256)
        << std::setw(2) << (rand() % 256)
        << std::setw(2) << (rand() % 256);
    return oss.str();
}
