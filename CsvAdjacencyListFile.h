#ifndef CSV_ADAJACENCY_LIST_FILE_H
#define CSV_ADAJACENCY_LIST_FILE_H

#include "IntegerIdAdjacencyListFile.h"

namespace GraphGame
{
    class CsvAdjacencyListFile : public IntegerIdAdjacencyListFile
    {
    public:
        CsvAdjacencyListFile(const std::string& filename);
    private:
    };
}
#endif