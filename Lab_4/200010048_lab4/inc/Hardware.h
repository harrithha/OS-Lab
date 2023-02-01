#ifndef HARDWARE_H
#define HARDWARE_H

#include "Process.h"

class Process;

class HardWare{
    public:
        Process *process;
        HardWare();
        int run();
};

#endif
