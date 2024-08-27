#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage

    while(true) {
        std::cout << "$ ";

        std::string input;
        std::getline(std::cin, input);

        if(input.empty()) {
            break;
        }
    // exit built it
        if (input == "exit 0") {
            exit(0);
        }

    //echo built in

        if(input.substr(0,4)=="echo") {
            std::cout<<input.substr(5)<<std::endl;
        }else {
            std::cerr <<input << ": command not found\n";
        }

    }
    return 0;
}
