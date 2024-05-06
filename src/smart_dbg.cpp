#include <iostream>
#include <unistd.h>
#include <sys/ptrace.h>
#include "include/debugger.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Prg to debug not specified" << std::endl;
    }

    auto program = argv[1];
    int pid = fork();
    // we are in the child process
    if (pid == 0) {
        // we are in the child process
        ptrace(PT_TRACE_ME, 0, nullptr, -1);
        execl(program, program, nullptr);

    } else if (pid  > 0){

        std::cout << "Debugging Started for process " << pid << std::endl;
        debugger dbg{program, pid};

    }
}