#include <iostream>
#include <unordered_map>
#include <string>

class Shell {
public:
    Shell() {
        commandMap["echo"] = "echo is a shell builtin";
        commandMap["exit"] = "exit is a shell builtin";
        commandMap["type"] = "type is a shell builtin";
    }

    void run() {
        while(true) {
            std::cout<<"$ ";

            std::string input;
            std::getline(std::cin, input);

            if(input.empty()) {
                break;
            }


            std::string command = parseCommand(input);

            // exit built it
            if (command == "exit") {
                exit(0);
            }

            if(command=="echo") {
                echoCommand(input);
            }else if(command=="type") {
                typeCommand(input.substr(5));
            }else {
                std::cerr << command << ": command not found\n";
            }
        }
    }

private:

    std::unordered_map<std::string, std::string> commandMap;

    std::string parseCommand(const std::string& input) {
        return input.substr(0, input.find(' '));
    }

    void echoCommand(const std::string& input) {
        std::cout << input.substr(5) << std::endl;
    }

    void typeCommand(const std::string& command) {
        // Check if the command exists in the map and output its type

        if(commandMap.find(command) != commandMap.end()) {
            std::cout << commandMap[command] << std::endl;
        }else {
            std:: cout << command << ": not found" << std::endl;
        }
    }
};



int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  Shell shell;
    shell.run();

    return 0;
}
