#include <string>
#include <unistd.h>
#include "submodules/linenoise/linenoise.h"
#include <vector>
#include <iostream>

class debugger {
    // public members
    public:
        debugger (std::string program_name, pid_t pid)
            : m_prog_name{std::move(program_name)}, m_pid{pid} {}

        void run();

    // private members (default behaviour in c++)
    private:
        void execute_command(std::string);
        void continue_execution();

        std::string m_prog_name;
        pid_t m_pid;
};

void debugger::run() {
    int wait_status;
    int options = 0;
    // waiting for child to finish launching and sending SIGTRAP signal (trace/breakpoint)
    waitpid(m_pid, &wait_status, options);

    // looping until we get the CTR + C from user
    char* line = nullptr;
    while ((line = linenoise("smart_dbg> ")) != nullptr) {
        std::string command(line);
        execute_command(command);
        linenoiseHistoryAdd(line);
        linenoiseFree(line);
    }
}

void debugger::execute_command(std::string command) {
    auto args = split(command, " ");
    std::string command = args[0];

    if (contains(command, "continue") ) {
        continue_execution();
    } else {
        std::cerr << "Unknown command" << std::endl;
    }
}

void debugger::continue_execution() {
    ptrace(PT_CONTINUE, m_pid, nullptr, -1);

    int wait_status;
    int options = 0;
    waitpid(m_pid, &wait_status, options);
}

std::vector<std::string> split(std::string command, std::string deliminter) {
    std::vector<std::string> result;
    size_t pos = 0;
    std::string token;
    while ((pos = command.find(deliminter)) != std::string::npos) {
        token = command.substr(0, pos);
        result.push_back(token);
        command.erase(0, pos + deliminter.length());
    }
    return result;
}

bool contains(std::string command, std::string full_command) {
    if (command.length() > full_command.length()) return false;
    
    auto res = std::mismatch(command.begin(), command.end(), full_command.begin());
    if (res.first == command.end()) return true;
}