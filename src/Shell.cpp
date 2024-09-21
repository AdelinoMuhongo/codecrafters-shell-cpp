//
// Created by delin on 21/09/2024.
//

#include "Shell.h"
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

Shell::Shell() {
    commandMap["echo"] = "echo is a shell builtin";
    commandMap["exit"] = "exit is a shell builtin";
    commandMap["type"] = "type is a shell builtin";
    commandMap["pwd"] = "pwd is a shell builtin";
    commandMap["cd"] = "cd is a shell builtin";
}

void Shell::run() {
    while (true) {
        std::cout << "$ ";
        std::cout.flush();
        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) {
            continue;
        }

        std::string command = parseCommand(input);

        if (command == "exit") {
            exit(0);
        } else if (command == "echo") {
            echoCommand(input);
        } else if (command == "type") {
            typeCommand(input.substr(5));
        } else if (command == "pwd") {
            pwdCommand();
        } else if (command == "cd") {
            cdCommand(input.substr(3));
        } else {
            runExternalCommand(command, input);
        }
    }
}

std::string Shell::parseCommand(const std::string& input) {
    return input.substr(0, input.find(' '));
}

void Shell::echoCommand(const std::string& input) {
    std::cout << input.substr(5) << std::endl;
    std::cout.flush();
}

void Shell::typeCommand(const std::string& command) {
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
    std::cout.flush();
}

void Shell::pwdCommand() {
    std::cout << std::filesystem::current_path().string() << std::endl;
    std::cout.flush();
}

void Shell::cdCommand(const std::string& path) {

    std::string trimmedPath = path;
    trimmedPath.erase(0, trimmedPath.find_first_not_of(' '));
    trimmedPath.erase(trimmedPath.find_last_not_of(' ') + 1);

    if (trimmedPath == "~" || trimmedPath.substr(0, 2) == "~/") {
        const char* homeDir = std::getenv("HOME");
        if (homeDir == nullptr) {
            std::cerr << "cd: HOME environment variable is not set" << std::endl;
            return;
        }
        trimmedPath.replace(0, 1, homeDir);
    }

    if (std::filesystem::exists(trimmedPath) && std::filesystem::is_directory(trimmedPath)) {
        std::filesystem::current_path(trimmedPath);
    } else {
        std::cerr << "cd: " << trimmedPath << ": No such file or directory" << std::endl;
        std::cerr.flush();

    }
}

std::string Shell::findPath(const std::string& command) {
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

void Shell::runExternalCommand(const std::string& command, const std::string& input) {
    std::vector<std::string> args = splitInput(input);

    std::vector<char*> execArgs;
    for (auto& arg : args) {
        execArgs.push_back(arg.data());
    }
    execArgs.push_back(nullptr);

    std::string commandPath = findPath(command);

    if (commandPath.empty()) {
        std::cerr << command << ": command not found\n";
        std::cerr.flush();
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

std::vector<std::string> Shell::splitInput(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}
