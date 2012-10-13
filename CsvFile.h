#ifndef CSV_FILE_H
#define CSV_FILE_H

#include <vector>
#include <string>

namespace MapGraph
{
    /**
     * For standard CSV (Comma Separated Values) files.
     * Expects the first row to be the column names and successive rows
     * to be the column values. Parses a CSV file and makes the values
     * accessible by column name and row number.
     */
    class CsvFile
    {
    public:
        CsvFile(const std::string& filename);
        std::string toJSON() const;
    private:
        std::vector<std::string> columnNames;
        std::vector<std::vector<std::string> > rows;
    };
}

#endif
