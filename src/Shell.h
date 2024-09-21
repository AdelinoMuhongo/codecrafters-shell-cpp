//
// Created by delin on 21/09/2024.
//

#ifndef SHELL_H
#define SHELL_H




#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>

class Shell {
public:
    Shell();
    void run();

private:
    std::unordered_map<std::string, std::string> commandMap;
    std::string parseCommand(const std::string& input);
    void echoCommand(const std::string& input);
    void typeCommand(const std::string& command);
    void pwdCommand();
    void cdCommand(const std::string& path);
    std::string findPath(const std::string& command);
    void runExternalCommand(const std::string& command, const std::string& input);
    std::vector<std::string> splitInput(const std::string& input);
};



#endif //SHELL_H
