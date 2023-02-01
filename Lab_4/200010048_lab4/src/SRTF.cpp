#include <fstream>
#include <iostream>
#include <utility.h>
#include <Process.h>
#include <Hardware.h>
#include <bits/stdc++.h>

using namespace std;


struct SRTF
{
    bool operator()(const Process* lhs, const Process* rhs) const
    {
        return lhs->remaining_time > rhs->remaining_time;
    }
};


int main(int argc, char const *argv[])
{

    if(argc != 2){
        cout << "Usage: ./SRTF <process_file_path>" << endl;
        return 0;
    }

    string file_path = argv[1];
    unordered_map<int, queue<int>> all_process_dict;

    read_process_file(file_path, all_process_dict);


    int clock=0, total_procs_completed=0, no_of_procs_left=all_process_dict.size();
    unordered_map<string, vector<double>> metrics = {
        {"turnaround_time", vector<double>(no_of_procs_left, 0)},
        {"waiting_time", vector<double>(no_of_procs_left, 0)},
        {"response_time", vector<double>(no_of_procs_left, 0)},
        {"penalty_ratio", vector<double>(no_of_procs_left, 0)},
        {"system_throughput", vector<double>(no_of_procs_left, 0)},
        {"arrival_time", vector<double>(no_of_procs_left, 0)}
    };

    vector<bool> arrived(no_of_procs_left, false);
    priority_queue<Process*, vector<Process*>, SRTF> cpu_ready, io_ready;
    HardWare cpu, io;

    
    while(no_of_procs_left){

        for(auto &process: all_process_dict){
            if(process.second.front() == clock && !arrived[process.first]){
                process.second.pop();
                Process *new_process = new Process(process.first, &process.second, true, clock);
                metrics["arrival_time"][new_process->pid] = clock;
                bool is_preempt = preempt_existing_process(&(cpu.process), &(new_process));
                if(is_preempt){
                    metrics["response_time"][cpu.process->pid] = 0;
                    cpu.process->first_pop = true;
                }
                cpu_ready.push(new_process);
                arrived[process.first] = true;
            }
        }

        if(cpu.process == NULL && !cpu_ready.empty()){
            Process *current_process = cpu_ready.top();
            cpu_ready.pop();
            if(!current_process->first_pop){
                metrics["response_time"][current_process->pid] = clock - current_process->first_arrvied_in_cpu_queue;
                current_process->first_pop = true;
            }
            cpu.process = current_process;
        }

        if(io.process == NULL && !io_ready.empty()){
            Process *current_process = io_ready.top();
            io_ready.pop();
            io.process = current_process;
        }

        if(cpu.process != NULL){
            if(cpu.run()){
                Process* process_in_cpu = cpu.process;
                cpu.process = NULL;
                process_in_cpu->burst_times->pop();
                int next_burst = process_in_cpu->burst_times->front();
                process_in_cpu->remaining_time = next_burst;
                process_in_cpu->is_cpu = false;
                if(next_burst==-1){
                    no_of_procs_left--;
                    metrics["turnaround_time"][process_in_cpu->pid] = clock - process_in_cpu->first_arrvied_in_cpu_queue+1;
                    metrics["penalty_ratio"][process_in_cpu->pid] = (float)metrics["turnaround_time"][process_in_cpu->pid]/(float)process_in_cpu->actual_time;
                    metrics["waiting_time"][process_in_cpu->pid] = metrics["turnaround_time"][process_in_cpu->pid] - process_in_cpu->cpu_burst;
                    total_procs_completed++;
                }
                else{
                    io_ready.push(process_in_cpu);
                    process_in_cpu->print();
                }
            }
        }

        if(io.process != NULL){
            if(io.run()){
                Process* process_in_io = io.process;
                io.process = NULL;
                process_in_io->burst_times->pop();
                int next_burst = process_in_io->burst_times->front();
                process_in_io->remaining_time = next_burst;
                process_in_io->is_cpu = true;
                if(next_burst==-1){
                    no_of_procs_left--;
                    metrics["turnaround_time"][process_in_io->pid] = clock - process_in_io->first_arrvied_in_cpu_queue+1;
                    metrics["penalty_ratio"][process_in_io->pid] = (float)metrics["turnaround_time"][process_in_io->pid]/(float)process_in_io->actual_time;
                    metrics["waiting_time"][process_in_io->pid] = metrics["turnaround_time"][process_in_io->pid] - process_in_io->cpu_burst;
                    total_procs_completed++;
                }
                else{
                    preempt_existing_process(&(cpu.process), &(process_in_io));
                    cpu_ready.push(process_in_io);
                    process_in_io->print();
                }
            }
        }
        clock++;
    }
    metrics["system_throughput"][0] = (float)total_procs_completed/(float)(clock-1);
    print_metrics(metrics, file_path+".strf.csv");

    return 0;
}