#include <iostream>
#include <Process.h>
#include <bits/stdc++.h>

Process::Process(int pid, std::queue<int> *burst_times, bool is_cpu, int first_arrvied_in_cpu_queue)
{
    this->pid = pid;
    this->burst_times = burst_times;
    this->remaining_time = burst_times->front();
    this->is_cpu = is_cpu;
    this->first_arrvied_in_cpu_queue = first_arrvied_in_cpu_queue;
}

void Process::print()
{
    // cout << "PID: " << this->pid << endl;
    // cout << "Is CPU: " << this->is_cpu << endl;
    // cout<<"Burst time: " << this->burst_times->front() << endl;
    // cout << "Remaining Time: " << this->remaining_time << endl;
}