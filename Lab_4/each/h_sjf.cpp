#include <bits/stdc++.h>
#include <fstream>

using namespace std;

// Initialize current time
int currentTime = 0;

struct performanceMetrics
{
    vector<vector<int>> turnaroundTime;
    vector<vector<int>> waitingTime;
    vector<vector<int>> responseTime;
    vector<vector<float>> penaltyRatio;
    vector<vector<int>> burstTime;
    float avgTt;
    float avgWt;
    float avgRt;
    float avgPr;
    float sysThroughput;

} metrics;

// Helper Functions

void printVector(vector<vector<int>> v)
{
        for (int i = 0; i < v.size(); i++)
        {
            for (int j = 0; j < v[i].size(); j++)
                cout << v[i][j] << " ";
            cout<<"\n";
        }   
}

void printVecToFile(vector<vector<int>> v, ofstream& output)
{   
    for (int i = 0; i < v.size(); i++)
        {
            output<<"{"<<i<<"} : ";
            for (int j = 0; j < v[i].size(); j++)
                output << v[i][j] << " ";
            output<<"\n";
        }
}

void printVecToFile(vector<vector<float>> v, ofstream& output)
{   
    for (int i = 0; i < v.size(); i++)
        {
            output<<"{"<<i<<"} : ";
            for (int j = 0; j < v[i].size(); j++)
                output << v[i][j] << " ";
            output<<"\n";
        }
    
}

float avgVector(vector<int> v)
{
    float avg = 0;
    for (int i = 0; i < v.size(); i++)
        {
            avg += v[i];
        }
    return avg/v.size();
}

float avgVector(vector<vector<int>> v)
{
    float avg = 0;
    for (int i = 0; i < v.size(); i++)
        {
            for (int j = 0; j < v[i].size(); j++)
                avg += v[i][j];
        }
    return avg/v.size();
}

float avgVector(vector<vector<float>> v)
{
    float avg = 0;
    for (int i = 0; i < v.size(); i++)
        {
            for (int j = 0; j < v[i].size(); j++)
                avg += v[i][j];
        }
    return avg/v.size();
}

bool sortVec(const vector<int>& v1, const vector<int>& v2)
{
    return (v1[1] <= currentTime && v1[2] < v2[2]);
}

int main(int argc, char* argv[]) {

    // Declare variables
    ifstream input;
    ofstream output("sjf.txt");
    string line;
    int start = 3, here = 0;
    vector<vector<int>> processes;
    vector<int> proc;
    int sysTt, sysWt, sysPr, sysThroughput;


    // Read test data
    input.open(argv[1]);
    while(!input.eof())
    {
        input >> line;
        if (line[0]!='<')
        {
            proc.push_back(stoi(line));
            if (line == "-1")
            {
                processes.push_back(proc);
                proc.clear();
            }
        }
    }
    input.close();

    int totalJobs = processes.size();
    for (int i = 0; i < totalJobs; i++)
    {
        
        vector<int> tempTt, tempWt, tempRt, tempBt;
        vector<float> tempPr;
        for (int j = 0; j < (processes[i].size() - 1)/2; j++)
        {
            tempTt.push_back(0);
            tempWt.push_back(0);
            tempRt.push_back(-1);
            tempPr.push_back(0);
            tempBt.push_back(0);
        }

        metrics.turnaroundTime.push_back(tempTt);
        metrics.waitingTime.push_back(tempWt);
        metrics.responseTime.push_back(tempRt);
        metrics.penaltyRatio.push_back(tempPr);
        metrics.burstTime.push_back(tempBt);

        // Calculate burst time for every process
        for (int j = 1, k = 0; k < (processes[i].size() - 1)/2; j+=2, k+=1)
        {
            metrics.burstTime[i][k] = processes[i][j];
        }

        tempTt.clear();
        tempWt.clear();
        tempRt.clear();
        tempPr.clear();
        tempBt.clear();
    }

    // // Run the jobs
    vector<vector<int>> readyQueue;

    for (int i = 0; i < processes.size(); i++)
    {
        readyQueue.push_back({i, processes[i][0], processes[i][1], 2} );
    }

    sort(readyQueue.begin(), readyQueue.end(), sortVec);
    while(readyQueue.size()!=0)
    {
        cout<<"--------------------------------SCHEDULING START--------------------------------------\n";
        cout<<"Ready Queue: \n";
        // printVector(readyQueue);
        cout<<"**************************************************************\n"; 
        vector<int> running_process = readyQueue.front();
        int index = running_process[0];
        int arrTime = running_process[1];
        int burstTime = running_process[2];
        int step = running_process[3];
        metrics.responseTime[index][(step/2) - 1] = currentTime - arrTime;

        // If no process can be executed, change current time to lowest arrival time
        if (currentTime < arrTime)
        {
            currentTime += arrTime;
        }

        currentTime += burstTime;

        if(processes[index][step] == -1)
        {
            cout<<"Process "<<index<<" exited. \n";
        }
        // else if(processes[index][step + 1] == -1)
        // {
        //     cout<<"Process "<<index<<" exited. \n";
        // }
        else
        {
            
            // Add next process
            vector<int> next_running = {index, currentTime + processes[index][step], processes[index][step + 1], step + 2};
            readyQueue.push_back(next_running);
        }

        // Calculate Metrics
        metrics.turnaroundTime[index][(step/2) - 1] = currentTime + 1 - arrTime;
        metrics.waitingTime[index][(step/2) - 1] = currentTime + 1 - metrics.burstTime[index][(step/2) - 1] - arrTime;
            if(metrics.waitingTime[index][(step/2) - 1] < 0)
                metrics.waitingTime[index][(step/2) - 1] = 0;
        metrics.penaltyRatio[index][(step/2) - 1] = (float)metrics.burstTime[index][(step/2) - 1]/(metrics.burstTime[index][(step/2) - 1] + metrics.waitingTime[index][(step/2) - 1]);

        readyQueue.erase(readyQueue.begin());
        sort(readyQueue.begin(), readyQueue.end(), sortVec);
    }

    // Calculate Metrics
    metrics.avgTt = avgVector(metrics.turnaroundTime);
    metrics.avgWt = avgVector(metrics.waitingTime);
    metrics.avgRt = avgVector(metrics.responseTime);
    metrics.avgPr = avgVector(metrics.penaltyRatio);
    metrics.sysThroughput = (float)totalJobs/currentTime;

    // Write to file
    output<<"METRICS: \n";
    output<<"Turnaround time for each process : \n";
    printVecToFile(metrics.turnaroundTime, output);
    output<<"Waiting time for each process : \n";
    printVecToFile(metrics.waitingTime, output);
    output<<"Response Time for each process : \n";
    printVecToFile(metrics.responseTime, output);
    output<<"Penalty Ratio for each process : \n";
    printVecToFile(metrics.penaltyRatio, output);
    output<<"Average Turnaround Time = "<<metrics.avgTt<<"\n";
    output<<"Average Waiting Time = "<<metrics.avgWt<<"\n";
    output<<"Average Response Time = "<<metrics.avgRt<<"\n";
    output<<"Average Penalty Ratio = "<<metrics.avgPr<<"\n";
    output<<"System Throughput = "<<metrics.sysThroughput<<"\n";
    output.close();

    // Print in stdout
    cout<<"---------------------------------------------------------------------------------------\n";
    cout<<"METRICS: \n";
    cout<<"Average Turnaround Time = "<<metrics.avgTt<<"\n";
    cout<<"Average Waiting Time = "<<metrics.avgWt<<"\n";
    cout<<"Average Response Time = "<<metrics.avgRt<<"\n";
    cout<<"Average Penalty Ratio = "<<metrics.avgPr<<"\n";
    cout<<"System Throughput = "<<metrics.sysThroughput<<"\n";


   return 0;
}