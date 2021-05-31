//
//  ReadTrades.h
//  FRE7831-Homework2
//
//  Created by denizkuraal on 02/13/2021.
//

#ifndef ReadTrades_h
#define ReadTrades_h


#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class ReadTrades
{
    public:
    //Class for reading line by line from csv file
        std::string_view operator[](std::size_t index) const
        {
            return std::string_view(&m_line[m_data[index] + 1], m_data[index + 1] -  (m_data[index] + 1));
        }
        std::size_t size() const
        {
            return m_data.size() - 1;
        }
        void readNextRow(std::istream& str)
        {
            std::getline(str, m_line);

            m_data.clear();
            m_data.emplace_back(-1);
            std::string::size_type pos = 0;
            while((pos = m_line.find(',', pos)) != std::string::npos)
            {
                m_data.emplace_back(pos);
                ++pos;
            }
            // This checks for a trailing comma with no data after it.
            pos   = m_line.size();
            m_data.emplace_back(pos);
        }
    private:
        std::string         m_line;
        std::vector<int>    m_data;
};

std::istream& operator>>(std::istream& str, ReadTrades& data)
{
    data.readNextRow(str);
    return str;
}

   

#endif /* ReadTrades_h */










/*SOURCE: https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c?page=1&tab=votes&newreg=b6aaab35a9aa4d1ab1be92ad8f44c527

*/
