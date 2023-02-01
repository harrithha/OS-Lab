#include <iostream>
#include <utility.h>
#include <Process.h>
#include <Hardware.h>
#include <iomanip>
#include <ios>

using namespace std;

void print_metrics(unordered_map<string, vector<double>> &metrics, string output_file_path){

    ofstream output_file(output_file_path);
    double avg_turnaround_time = 0, avg_waiting_time = 0, avg_response_time = 0, avg_penalty_ratio = 0;
    
    for(auto &metric: metrics){
        for(auto &value: metric.second){
            if(metric.first == "turnaround_time") avg_turnaround_time += value;
            else if(metric.first == "waiting_time") avg_waiting_time += value;
            else if(metric.first == "response_time") avg_response_time += value;
            else if(metric.first == "penalty_ratio") avg_penalty_ratio += value;
        }
    }

    output_file<<"arrival_time,turnaround_time,waiting_time,response_time,penalty_ratio"<<endl;
    int no_of_procs = metrics["turnaround_time"].size();
    for(int i=0; i<no_of_procs; i++){
        output_file<<metrics["arrival_time"][i]<<","<<metrics["turnaround_time"][i]<<
                ","<<metrics["waiting_time"][i]<<","<<metrics["response_time"][i]<<","<<metrics["penalty_ratio"][i]<<endl;
    }
    output_file.close();
    cout<<setprecision(5);
    cout<<"<-----------------------------------Metrics----------------------------------------------------->"<<endl;
    cout<<"|Process Number\t|Arrival\t|Turnaround\t|Waiting\t|Response\t|Penalty Ratio\t|"<<endl;
    for(int i=0; i<no_of_procs; i++){
        cout<<"-------------------------------------------------------------------------------------------------"<<endl;
        cout<<"|"<<i+1<<"\t\t|"<<metrics["arrival_time"][i]<<"\t\t|"<<metrics["turnaround_time"][i]<<"\t\t|"<<metrics["waiting_time"][i]<<"\t\t|"<<metrics["response_time"][i]<<"\t\t|"<<metrics["penalty_ratio"][i]<<"\t\t|"<<endl;
    }
    cout<<"-------------------------------------------------------------------------------------------------"<<endl;
    cout<<"|Average\t|"<<"\t\t|"<<avg_turnaround_time/no_of_procs<<"\t\t|"<<avg_waiting_time/no_of_procs<<"\t\t|"<<avg_response_time/no_of_procs<<"\t\t|"<<avg_penalty_ratio/no_of_procs<<"\t\t|"<<endl;
    cout<<"<----------------------------------------------------------------------------------------------->"<<endl;
    cout<<endl;
    cout<<"System Throughput: "<< metrics["system_throughput"][0] <<endl;
    return;
}

void read_process_file(string file_path, unordered_map<int, queue<int>> &all_process_dict){
    fstream process_file;
    process_file.open(file_path, ios::in);

    if (process_file.is_open()){

        string line_in;
        int line_no=0;

        while(getline(process_file, line_in)){

            if(line_in[0] == '<'){
                if(line_in[1] == '/') break;
                else continue;
            }

            stringstream ss(line_in);

            string temp;

            while (ss >> temp) {
                int input_int = stoi(temp);
                if(all_process_dict.find(line_no) == all_process_dict.end()){
                    all_process_dict[line_no] = queue<int>();
                }
                all_process_dict[line_no].push(input_int);

            }
            line_no++;
        }
        process_file.close();
    }
    return;
}


bool preempt_existing_process(Process **cpu_process, Process **new_process){
    if(*cpu_process != NULL){
        if((*cpu_process)->remaining_time > (*new_process)->remaining_time){
            Process *temp = *cpu_process;
            *cpu_process = *new_process;
            *new_process = temp;
            return true;
        }
    }
    return false;
}