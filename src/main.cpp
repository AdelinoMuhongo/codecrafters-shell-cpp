#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <sstream>

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
            std::string path = findPath((command));
            if(!path.empty()) {
                std::cout << command << " is " << path << std::endl;

            }else{
                std:: cout << command << ": not found" << std::endl;
            }
        }
    }
    std::string findPath(const std::string &command) {
        std::string path_env = std::getenv("PATH");
        std::stringstream ss(path_env);
        std::string path;

        while(std::getline(ss, path, ':')) {
            std::string full_path = path + '/' + command;

            if(std::filesystem::exists(full_path) && std::filesystem::is_regular_file(full_path)) {
                return full_path;
            }
        }
        return "";
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
