#include <iostream>
#include <Process.h>
#include <Hardware.h>

HardWare::HardWare(){
    this->process = NULL;
}

int HardWare::run(){
    // cout<<"--------------------------------------------------\n";
    // if(process->is_cpu) cout<<"Running CPU process"<<endl;
    // else cout<<"Running IO process"<<endl;
            
    // cout << "Running process " << this->process->pid << endl;
    // cout << "Remaining time: " << this->process->remaining_time << endl;
    this->process->remaining_time--;
    this->process->actual_time++;
    if(this->process->is_cpu) this->process->cpu_burst++;
    if(this->process->remaining_time <= 0){
        // cout << "Part of process" << this->process->pid << " finished" << endl;
        return 1;
    }
    return 0;
}
