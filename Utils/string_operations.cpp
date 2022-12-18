#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>

struct Line
{
    std::string buffer;

    operator std::string() const
    {
        return buffer;
    }
};
std::istream& operator>>(std::istream& str, Line& data)
{
    std::getline(str, data.buffer);
    return str;
}
