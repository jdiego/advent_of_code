#include <string_view>

int test_sample_input(){
    return 0;
}

int parse_and_run(std::string_view path){

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