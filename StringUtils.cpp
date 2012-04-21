#include "StringUtils.h"
#include <sstream>
#include <algorithm>

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

bool StringUtils::iequals(std::string s1, std::string s2)
{
	std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
	return (s1 == s2);
}

StringUtils::CaseInsensitiveComparator::CaseInsensitiveComparator(const std::string& strToMatch) 
	: matchTarget(strToMatch) 
{ 
	std::transform(matchTarget.begin(), matchTarget.end(), matchTarget.begin(), ::tolower);
}

bool StringUtils::CaseInsensitiveComparator::operator() (std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return (s == matchTarget);
}