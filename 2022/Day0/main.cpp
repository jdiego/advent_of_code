#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <string_view>
#include <ranges>
#include <iostream>
#include <fstream>


int test_sample_input(){
    return 0;
}

int parse_and_run(const std::string path){

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