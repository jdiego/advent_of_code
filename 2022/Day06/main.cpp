#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_set>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>


std::size_t find_packet_marker(const std::string& datastream, std::size_t marker_sizer = 4) {
    for(std::size_t i = marker_sizer - 1; i < datastream.size(); ++i) {
        bool inserted = false;
        std::unordered_set<std::string::value_type> marker;

        for(std::size_t j = 0; j < marker_sizer; ++j) {
            std::tie(std::ignore, inserted) = marker.insert(datastream[i - j]);
            if(inserted == false) break; 
        }
        //
        if (marker.size() == marker_sizer){
            return i + 1;
        }
    }
    return 0;
}

int test_sample_input(){
    
    std::vector<std::pair<std::string, std::size_t>> input_data =  {
        {"mjqjpqmgbljsphdztnvjfqwrcgsmlb", 7},
        {"bvwbjplbgvbhsrlpgdmjqwftvncz", 5},
        {"nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 10},
        {"zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 11}
    };

    std::ranges::for_each(input_data, [](const auto& data){
        auto at = find_packet_marker(data.first);
        std::cout << "We need to process " 
                << at 
                << " characters before the first start-of-packet marker.\n";
        //
        assert(at == data.second);
    });

    // PART II
    std::vector<std::pair<std::string, std::size_t>> input_data2 =  {
        {"mjqjpqmgbljsphdztnvjfqwrcgsmlb", 19},
        {"bvwbjplbgvbhsrlpgdmjqwftvncz", 23},
        {"nppdvjthqldpwncqszvftbrmjlhg", 23},
        {"nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 29},
        {"zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 26}
    };

    std::ranges::for_each(input_data2, [](const auto& data){
        auto at = find_packet_marker(data.first, 14);
        std::cout << "We need to process " 
                << at 
                << " characters before the first start-of-packet marker.\n";
        //
        assert(at == data.second);
    });


    return 0;
}

int parse_and_run(const std::string path){
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::string input_data;
    getline(file, input_data);
    std::cout << "We need to process " 
            << find_packet_marker(input_data) 
            << " characters before the first start-of-packet marker.\n";
    
    std::cout << "We need to process " 
            << find_packet_marker(input_data, 14) 
            << " characters before the first start-of-packet marker.\n";
    

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