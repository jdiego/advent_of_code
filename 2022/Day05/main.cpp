#include <cassert>
#include <cstdint>
#include <cctype>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

struct Move
{
    size_t count;
    size_t src;
    size_t dst;
    friend auto operator<=>(const Move&, const Move&) = default;

    friend std::istream& operator>>(std::istream& stream, Move& move) {

        std::string line;
        std::getline(stream, line);
        std::sscanf(line.c_str(), "move %zu from %zu to %zu", &move.count, &move.src, &move.dst);
        // zero-base moves.....
        --move.src;
        --move.dst;
        return stream;
    }
};


using Moves = std::vector<Move>;
using Stack = std::vector<char>;
using Stacks = std::vector<Stack>;

void dump_crate(const Stack& crate, int index) {
    std::cout << " Stack " << index + 1 << ": ";
    std::ranges::for_each(crate, [](auto const &it) { std::cout << it << " "; });
    std::cout << '\n';
}

Stacks load_crates(std::istream& stream)
{
    Stacks crates;
    std::string line;
    bool done = false;
    // process input line-by-line
    int count_line = 0;
    while (!done && std::getline(stream, line)) {
        for(std::size_t i = 0; i < line.size(); ++i){
            std::size_t current_stack = (i + 1) / 4;
            if (std::isupper(line[i])) {
                if (current_stack >= crates.size()) { 
                    crates.resize(current_stack + 1);
                }
                crates[current_stack].push_back(line[i]);
            }
            if (std::isdigit(line[i])) {
                done = true;
                break;
            }
        }
        count_line++;
    }
    //
    std::ranges::for_each(crates, std::ranges::reverse);
    // eat the empty-line before the moves
    std::getline(stream, line);
    return crates;
}


void operate_crane_mover_9000(Stacks& stacks, const Moves& move)
{
    //
    std::ranges::for_each(move, [&stacks](const auto& move){
        //
        auto& src = stacks[move.src];
        auto& dst = stacks[move.dst];
        // std::ranges::subrange rng ( src.end() - move.count, src.end());
        // auto reverse_rng = rng | std::views::reverse | std::views::common;
        // dst.insert(dst.end(), reverse_rng.begin(), reverse_rng.end());
        dst.insert(dst.end(),  src.rbegin(), src.rbegin() + move.count);
        src.erase( src.end() - move.count, src.end());

    });
}

void operate_crane_mover_9001(Stacks& stacks, const Moves& move)
{
    //
    std::ranges::for_each(move, [&stacks](const auto& move){
        //
        auto& src = stacks[move.src];
        auto& dst = stacks[move.dst];
        dst.insert(dst.end(), src.end() - move.count, src.end());
        src.erase( src.end() - move.count, src.end());

    });
}

std::string top_crates(const Stacks& crates)
{
    std::string result;
    for (auto& crate : crates) {
        if (crate.size())
            result += crate.back();
    }
    return result;
}

int test_sample_input(){
    std::stringstream stream(R"(    
    [D]    
[N] [C]    
[Z] [M] [P]
 1   2   3 

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2)"
    );

    Stacks test_crates {
        {'Z', 'N'},
        {'M', 'C', 'D'},
        {'P'},
    };
    auto crates = load_crates(stream);
    // std::cout << "State: \n";
    // std::ranges::for_each(crates, [](const auto& v) {
    //     std::cout << " Stack: ";
    //     std::ranges::for_each(v, [](auto const& it) {
    //         std::cout << it << " ";
    //     });
    //     std::cout << '\n';
    // });
    assert(crates.size() == 3);

    assert(std::ranges::equal(crates[0], test_crates[0]));
    assert(std::ranges::equal(crates[1], test_crates[1]));
    assert(std::ranges::equal(crates[2], test_crates[2]));
    
    Moves moves;
    std::ranges::copy(std::views::istream<Move>(stream), std::back_inserter(moves));
    assert(moves.size() == 4);
    auto test_moves = {Move{1, 1, 0}, Move{3, 0, 2}, Move{2, 1, 0}, Move{1, 0, 1}};
    assert(std::ranges::equal(moves, test_moves));
    
    //
    auto crates_copy = crates;
    //
    operate_crane_mover_9000(crates, moves);
    std::string result = top_crates(crates);
    std::cout << "After processing orders using CrateMover9000, the top crates are " << top_crates(crates) << '\n';
    assert(result == "CMZ");
    //
    operate_crane_mover_9001(crates_copy, moves);
    result = top_crates(crates_copy);   
    std::cout << "After processing orders using CrateMover9001, the top crates are " << top_crates(crates_copy) << '\n';
    assert(result == "MCD");



    return 0;
}

int parse_and_run(const std::string path){
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }
    auto crates = load_crates(file);
    Moves moves;
    std::ranges::copy(std::views::istream<Move>(file), std::back_inserter(moves));
    operate_crane_mover_9000(crates, moves);
    std::cout << "After processing orders, the top crates are " << top_crates(crates) << '\n';
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