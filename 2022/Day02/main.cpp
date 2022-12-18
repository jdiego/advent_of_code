#include <cassert>
#include <vector>
#include <string_view>
#include <ranges>
#include <iostream>
#include <fstream>
#include <numeric>
#include <iomanip>


using Round = std::pair<char, char>;

int score(int me, int oponent) {
    
    //The score for a single  round is the score for the shape you selected 
    // (1 for Rock, 2 for Paper, and 3 for Scissors) plus 
    int score = me + 1;
    // the score for the outcome of the round:
    //  (0 if you lost, 3 if the round was a draw, and 6 if you won).
    // 0 - rock; 1 - paper; 2 - scissor
    // (4 + 0 - 2) % 3 = 0 lose
    // (4 + 0 - 0) % 3 = 1 draw
    // (4 + 1 - 0) % 3 = 2 win
    score += 3*(( 4 + me - oponent) % 3);
    return score; 
}
auto follow_strategy_guide(const std::vector<Round>& data) {
    auto doit = [](auto& round) -> std::uint64_t {
        // A for Rock (0), B for Paper (1), and C for Scissors (2).
        int oponent = round.first - 'A';
        // X for Rock(0), Y for Paper (1), and Z (2) for Scissors.
        int me = round.second - 'X';
        return score(me, oponent);
    };
    auto scored = data | std::views::transform(doit) | std::views::common;
    return std::accumulate(scored.begin(), scored.end(), 0);
}

auto match_result(const std::vector<Round>& data){
    auto doit = [](auto& round) -> int {
        // A for Rock (0), B for Paper (1), and C for Scissors (2).
        int oponent = round.first - 'A';
        // The second column says how the round needs to end: 
        // X means you need to lose (0), 
        // Y means you need to end the round in a draw (1), 
        // and Z means you need to win. Good luck!
        int outcome = round.second - 'X';
        // Figuring out what shape we need to choose so the round 
        // ends as indicated by the outcome.
        int me = (2 + oponent + outcome) % 3;
        return score(me, oponent);
    };
    auto scored = data | std::views::transform(doit) | std::views::common;
    return std::accumulate(scored.begin(), scored.end(), 0);
}
int test_sample_input() {
    std::vector<Round> test_input {
        {'A','Y'},
        {'B','X'},
        {'C','Z'}
    };
    assert(follow_strategy_guide(test_input) == 15);
    assert(match_result(test_input) == 12);
    return 0;
}

int parse_and_run(const std::string path){
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    Round round;
    std::vector<Round> rounds;
    while (file >> round.first >> round.second) {
        rounds.push_back(round);
    }
    std::cout << "The total score while following the strategy guide is " << follow_strategy_guide(rounds) << "\n";
    std::cout << "The total score while following the proper strategy guide is " << match_result(rounds) << "\n";

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