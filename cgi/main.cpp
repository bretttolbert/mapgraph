#include <iostream>
#include <fstream>
#include <cstdlib>

int main(int argc, char** argv)
{
    std::cout << "Content-Type: text/plain;charset=us-ascii\n\n";
    if (!getenv("QUERY_STRING"))
    {
        std::cout << "Error: failed to get QUERY_STRING\n";
        exit(1);
    }
    std::string query = getenv("QUERY_STRING");
    std::cout << "query: " << query << "<br>";
    if (system(NULL))
    {
        int result = system("graphgame -m neighbors -n1 \"madison, al\" -af preset us-counties > out.txt");
        std::ifstream file;
        file.open("out.txt", std::ios::in);
        while (file.good())
        {
            std::string line;
            getline(file, line);
            std::cout << line << "\n";
        }
    }
    else
    {
        std::cout << "Error: Processor is not available.<br>\n";
    }
    return 0;
}
