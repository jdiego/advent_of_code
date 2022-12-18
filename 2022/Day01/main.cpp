#include <cstdint>
#include <vector>
#include <string>
#include <string_view>
#include <ranges>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>

struct Line : public std::string {
    using std::string::string;
};

std::istream& operator>>(std::istream& stream, Line& line)
{
    std::getline(stream, line);
    return stream;
}


auto max_calories(const std::vector<std::string>& data, std::string delimiter){
    auto by_elf = data | 
        // 1. split the input by-elf
        std::views::lazy_split(delimiter) |
        // 2. sum up the numbers, so we have a total for each elf
        std::views::transform([](const auto& elf) -> std::uint64_t {
            // 3. convet each calorie string into a number
            auto to_number = [](const auto& in) { return std::stoull(in); };
            auto rng = elf | std::views::transform(to_number) | std::views::common; 
            return std::accumulate(rng.begin(), rng.end(), 0);
        });
    return by_elf;
}

auto top_one(const std::vector<std::string>& data, std::string delimiter = std::string{}){
    auto by_elf = max_calories(data, delimiter);
    // pick the elf with the highest total
    auto it = std::ranges::max_element(by_elf);
    return *it;
}

auto top_three(const std::vector<std::string>& data, std::string delimiter = std::string{}) {
    auto by_elf = max_calories(data, delimiter);
    std::vector<uint64_t> top_elfs(3);
    // sort the elfs by calories
    std::ranges::partial_sort_copy(by_elf, top_elfs, std::greater<>{});
    // pick the top three elf with the most calories
    return std::accumulate(top_elfs.begin(), top_elfs.end(), 0);

}

int parse_and_run(std::string_view path) {
    std::fstream file(path.data());
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }
    std::vector<std::string> data(std::istream_iterator<Line>{file}, std::istream_iterator<Line>{});
    std::cout << "The elf with the maximum number of calorie is carrying " << top_one(data) << " calories worth of food.\n";
    std::cout << "The top 3 elfs are carrying " << top_three(data) << " calories worth of food.\n";
    return 0;
}

int test_sample_input() {
    std::vector<std::string> test_input{
        "1000", "2000", "3000", "",
        "4000", "",
        "5000", "6000", "",
        "7000", "8000", "9000", "",
        "10000"
    };
    auto top_elf = top_one(test_input);
    assert(top_elf == 24000);
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
