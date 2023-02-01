#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <bits/stdc++.h>

class Process{
    public:
        int pid;
        int remaining_time;
        std::queue<int> *burst_times;
        bool is_cpu;
        int cpu_burst=0;
        int first_arrvied_in_cpu_queue;
        bool first_pop = false;
        int actual_time = 0;
    
        Process(int pid, std::queue<int> *burst_times, bool is_cpu, int first_arrvied_in_cpu_queue);
        void print();
};

#endif
