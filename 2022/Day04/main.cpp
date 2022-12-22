#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <string_view>
#include <algorithm>
#include <utility>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <fstream>

struct Range {
    std::uint32_t x;
    std::uint32_t y;
};

using RangePair = std::pair<Range, Range>;
using RangePairs = std::vector<RangePair>;


std::istream& operator>>(std::istream& stream, Range& range) {
    char delim;
    stream >> range.x >> delim >> range.y;
    return stream;
}

std::istream& operator>>(std::istream& stream, RangePair& range_pair) {
    char delim;
    stream >> range_pair.first >> delim >> range_pair.second;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, Range& range) {
    stream << range.x << "-" <<  range.y;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, RangePair& range_pair) {
    stream << range_pair.first << "," << range_pair.second;
    return stream;
}

std::uint32_t contains(const RangePairs& data) {
    auto contain = [] (const auto& range_pair) {
        const auto [x1, y1] = range_pair.first;
        const auto [x2, y2] = range_pair.second;
        return  (x1 <= x2 and y2 <= y1) or (x2 <= x1 and y1 <= y2);
    };
    auto count = std::ranges::count_if(data, contain);
    return count;
}

std::uint32_t overlaps(const RangePairs& data) {
    auto overlap = [] (const auto& range_pair) {
        const auto [x1, y1] = range_pair.first;
        const auto [x2, y2] = range_pair.second;
        return (x1 <= y2) and (x2 <= y1); 
    };
    auto count = std::ranges::count_if(data, overlap);
    return count;
}

int test_sample_input(){
    RangePairs input_data = {
        {{2,4},{6,8}},
        {{2,3},{4,5}},
        {{5,7},{7,9}},
        {{2,8},{3,7}},
        {{6,6},{4,6}},
        {{2,6},{4,8}}
    };
    assert(contains(input_data) == 2);
    assert(overlaps(input_data) == 4);
    return 0;
}

int parse_and_run(const std::string path){
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }
    RangePairs data(std::istream_iterator<RangePair>{file}, std::istream_iterator<RangePair>{});
    std::cout << "The number of elves that would do completely redundant cleaning is " << contains(data) << '\n';
    std::cout << "The number of elves that would do partially redudant cleaning is " << overlaps(data) << '\n';
    return 0;
}


int main(int argc, char* argv[]) 
{
    if (argc == 1) {
        return test_sample_input();
    } 
    else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
    
    return 0; 
}