#include <cassert>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>
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

std::uint64_t prioritize_items(const std::vector<std::string>& rucksacks) {
    auto view = [](std::string rucksack) -> std::uint64_t {
        auto midpoint = rucksack.begin() + rucksack.length()/2;
        std::ranges::subrange lower(rucksack.begin(), midpoint);
        std::ranges::subrange upper(midpoint, rucksack.end());
        // Sort the lower and upper parts.
        std::ranges::sort(lower);
        std::ranges::sort(upper);
        //
        std::vector<char> items;
        std::ranges::set_intersection(lower, upper, std::back_inserter(items));
        return (std::isupper(items[0])) ? 27 + items[0] - 'A' : 1 + items[0] - 'a';
    };
    auto priorities = rucksacks | std::views::transform(view) | std::views::common;
    return std::accumulate(priorities.begin(), priorities.end(), 0);
}

std::uint64_t prioritize_badge (const auto& rucksacks) {
    std::vector<char> tmp;
    std::ranges::set_intersection(rucksacks[0], rucksacks[1], std::back_inserter(tmp)); 
    // Determine the common items between the common items and the third rucksack.
    std::vector<char> out;
    std::ranges::set_intersection(tmp, rucksacks[2], std::back_inserter(out));
    // Calculate priority and add to the total.
    return (std::isupper(out[0])) ? (27 + out[0] - 'A') :(1 + out[0] - 'a');
}

std::uint64_t prioritize_badges (std::vector<std::string> rucksacks) {
    std::ranges::for_each(rucksacks, std::ranges::sort);
    auto const chunks = rucksacks | std::views::chunk(3);
    auto badges = chunks | std::views::transform([](const auto& chunk) -> std::uint64_t {
        return prioritize_badge(chunk);
    });
    return std::accumulate(badges.begin(), badges.end(), 0);
}

int test_sample_input() {

    std::vector<std::string> test_data {
        "vJrwpWtwJgWrhcsFMMfFFhFp",
        "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL",
        "PmmdzqPrVvPwwTWBwg",
        "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn",
        "ttgJtRGJQctTZtZT",
        "CrZsJsPPZsGzwwsLwLmpwMDw",
    };
    assert(prioritize_items(test_data) == 157);
    assert(prioritize_badges(test_data) == 70);
    return 0;
}

int parse_and_run(const std::string path){
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }
    std::vector<std::string> rucksacks(std::istream_iterator<Line>{file}, std::istream_iterator<Line>{});
    std::cout << "The sum of the priorities of items  is " << prioritize_items(rucksacks) << '\n';
    std::cout << "The sum of the priorities of badges is " << prioritize_badges(std::move(rucksacks)) << '\n';
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
