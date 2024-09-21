#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

class Shell {
public:
    Shell() {
        commandMap["echo"] = "echo is a shell builtin";
        commandMap["exit"] = "exit is a shell builtin";
        commandMap["type"] = "type is a shell builtin";
        commandMap["pwd"] = "pwd is a shell builtin";
        commandMap["cd"] = "cd is a shell builtin";

    }

    void run() {
        while (true) {
            std::cout << "$ ";
            std::cout.flush();
            std::string input;
            std::getline(std::cin, input);

            if (input.empty()) {
                break;
            }

            std::string command = parseCommand(input);

            if (command == "exit") {
                exit(0);
            } else if (command == "echo") {
                echoCommand(input);
            } else if (command == "type") {
                typeCommand(input.substr(5));
            }else if (command=="pwd") {
                pwdCommand();
                std::cout.flush();
            }else if(command=="cd") {
                cdCommand(input.substr((3)));
            }

            else {
                runExternalCommand(command, input);
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
        if (commandMap.find(command) != commandMap.end()) {
            std::cout << commandMap[command] << std::endl;
        } else {
            std::string path = findPath(command);
            if (!path.empty()) {
                std::cout << command << " is " << path << std::endl;
            } else {
                std::cout << command << ": not found" << std::endl;
            }
        }
    }



    std::string findPath(const std::string& command) {
        std::string path_env = std::getenv("PATH");
        std::stringstream ss(path_env);
        std::string path;

        while (std::getline(ss, path, ':')) {
            std::string full_path = path + '/' + command;
            if (std::filesystem::exists(full_path) && std::filesystem::is_regular_file(full_path)) {
                return full_path;
            }
        }
        return "";
    }

    void runExternalCommand(const std::string& command, const std::string& input) {
        std::vector<std::string> args = splitInput(input);

        std::vector<char*> execArgs;
        for (auto& arg : args) {
            execArgs.push_back(arg.data());
        }
        execArgs.push_back(nullptr);

        std::string commandPath = findPath(command);

        if (commandPath.empty()) {
            std::cerr << command << ": command not found\n";
            return;
        }

        pid_t pid = fork();

        if (pid == 0) {
            execvp(commandPath.c_str(), execArgs.data());
            std::cerr << "Failed to execute " << command << std::endl;
            exit(1);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        } else {
            std::cerr << "Failed to create a new process\n";
        }
    }

    std::vector<std::string> splitInput(const std::string& input) {
        std::istringstream iss(input);
        std::vector<std::string> tokens;
        std::string token;

        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    void pwdCommand() {
        std::cout<<std::filesystem::current_path().string()<<std::endl;
    }

    void cdCommand(const std::string& path) {
        std::string trimmedPath = path;
        trimmedPath.erase(0, trimmedPath.find_first_not_of(' '));
        trimmedPath.erase(trimmedPath.find_last_not_of(' ') + 1);

        if (std::filesystem::exists(trimmedPath) && std::filesystem::is_directory(trimmedPath)) {
            std::filesystem::current_path(trimmedPath);
        } else {
            std::cerr << "cd: " << trimmedPath << ": No such file or directory" << std::endl;
            std::cerr.flush();
        }
    }

};

int main() {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    Shell shell;
    shell.run();
    return 0;
}
