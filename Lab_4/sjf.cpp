#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;

// Initialize current time
int currentTime = 0;

// Helper Functions
int total_burst_time(vector<int> v)
{
  int sum = 0;
  for (int i = 1; i < v.size() - 1; i++)
  {
    sum += v[i];
  }
  return sum;
}

int total_cpu_burst_time(vector<int> v)
{
  int sum = 0;
  for (int i = 1; i < v.size(); i+=2)
  {
    sum += v[i];
  }
  return sum;
}

bool sortVec(const vector<int> &v1, const vector<int> &v2)
{
  return (v1[2] < v2[2]);
}

void selectArrivedProcesses(vector<vector<int>> &processQueue)
{
  vector<vector<int>> beforeCurrTime;
  vector<vector<int>> afterCurrTime;

  for (int i = 0; i < processQueue.size(); i++)
  {
    if (processQueue[i][1] > currentTime)
    {

      afterCurrTime.push_back(processQueue[i]);
    }
    else
    {
      beforeCurrTime.push_back(processQueue[i]);
    }
  }
  sort(beforeCurrTime.begin(), beforeCurrTime.end(), sortVec);
  sort(afterCurrTime.begin(), afterCurrTime.end(), sortVec);
  processQueue.clear();
  copy(beforeCurrTime.begin(), beforeCurrTime.end(), back_inserter(processQueue));
  copy(afterCurrTime.begin(), afterCurrTime.end(), back_inserter(processQueue));
}

int main(int argc, char* argv[])
{
    // Declare variables
    ifstream input;
    string line;
    vector<vector<int>> processes;
    vector<int> proc;
    int sysTt, sysWt, sysPr, sysThroughput;
    vector<vector<int>> cpuQueue;
    vector<vector<int>> ioQueue;
    map<int, int> exitTime;
    map<int, vector<vector<int>>> info;
    map<int, vector<float>> processAnalytics;
    map<int, int> cpuBurstTime;
    map<int, int> responseTimes;

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

    for (int i = 0; i < processes.size(); i++)
    {
        cpuQueue.push_back({i, processes[i][0], processes[i][1], 2, total_burst_time(processes[i])});
        cpuBurstTime[i] = total_cpu_burst_time(processes[i]);
    }

    selectArrivedProcesses(cpuQueue);

    int complete = 0;

    // Run queue until all processes are completed
    while(complete != processes.size())
    {
        if(cpuQueue.size() > 0)
        {
            vector<int> currCpuRunning = cpuQueue.front();
            int index = currCpuRunning[0];
            int arrTime = currCpuRunning[1];
            int burstTime = currCpuRunning[2];
            int step = currCpuRunning[3];
            int totalBurstTime = currCpuRunning[4];
            if(arrTime <= currentTime)
            {
                if(responseTimes.find(index) == responseTimes.end())
                {
                    responseTimes[index] = currentTime - processes[index][0];
                }

                if(burstTime == 0)
                {
                    if(processes[index][step] == -1)
                    {
                        exitTime[index] = currentTime;
                        cout << index + 1 << " exited current time " << currentTime << "\n";
                        complete += 1;
                    }
                    else
                    {
                        vector<int> newIo = {index, currentTime, processes[index][step], step + 1, totalBurstTime - processes[index][step - 1]};
                        ioQueue.push_back(newIo);
                    }

                    cpuQueue.erase(cpuQueue.begin());
                    selectArrivedProcesses(cpuQueue);
                    if (cpuQueue.size() > 0)
                    {
                        if (responseTimes.find(index) == responseTimes.end())
                        {
                            responseTimes[cpuQueue[0][0]] = currentTime - processes[cpuQueue[0][0]][0];
                        }
                        cpuQueue[0][2] -= 1;
                    }
                }
                else
                {
                    cpuQueue[0][2] -= 1;
                }
            }
        }
        if (ioQueue.size() > 0)
        {
            vector<int> currIoRunning = ioQueue.front();
            int ioIndex = currIoRunning[0];
            int ioArrTime = currIoRunning[1];
            int ioBurstTime = currIoRunning[2];
            int ioStep = currIoRunning[3];
            int ioTotalBurstTime = currIoRunning[4];

            if (ioArrTime <= currentTime)
            {
                if (ioBurstTime == 0)
                {
                    vector<int> newCpu = {ioIndex, currentTime, processes[ioIndex][ioStep], ioStep + 1, ioTotalBurstTime - processes[ioIndex][ioStep - 1]};
                    cpuQueue.push_back(newCpu);
                    if (cpuQueue.size() == 1)
                    {
                        cpuQueue[0][2] -= 1;
                    }
                    ioQueue.erase(ioQueue.begin());
                    if (ioQueue.size() > 0)
                    {
                        ioQueue[0][2] -= 1;
                    }
                }
                else
                {
                    ioQueue[0][2] -= 1;
                }
            }
        }
        currentTime += 1;
    }

    // key - process id
    // values in vector:
    // 0 - response time
    // 1 - waiting time
    // 2 - turn around time
    // 3 - penalty ratio ((wait+burst)/burst)

    vector<int> performanceMetrics;
    performanceMetrics = {0, 0, 0};
    float avgPr = 0;

    cout<<"----------------------------------------------------------------------------------\n";
    cout<<"PERFORMANCE ANALYSIS\n";
    cout<<"------------------------------------------------------------------------------------------------\n";
    cout<<"|Process Number\t|Arrival\t|Turnaround\t|Waiting\t|Response\t|Penalty Ratio\t|"<<endl;

    for(int i = 0; i < processes.size(); i++)
    {
        int pid = i;
        int respTime = responseTimes[i];
        int turnaroundTime = exitTime[i] - processes[i][0];
        int complTime = exitTime[i];
        int arrivalTime = processes[i][0];
        int cpuBurst = cpuBurstTime[i];
        int waitingTime = turnaroundTime - cpuBurst;
        int totalBurst = total_burst_time(processes[i]);
        float penaltyRatio = (waitingTime + totalBurst) * 1.0 / totalBurst;
        performanceMetrics[0]+= respTime;
        performanceMetrics[1]+= waitingTime;
        performanceMetrics[2]+= turnaroundTime;
        avgPr+= penaltyRatio;
        cout<<"------------------------------------------------------------------------------------------------"<<endl;
        cout<<"|"<<pid<<"\t\t|"<<arrivalTime<<"\t\t|"<<turnaroundTime<<"\t\t|"<<waitingTime<<"\t\t|"<<respTime<<"\t\t|"<<penaltyRatio<<"\t|"<<endl;
    }
    cout<<"------------------------------------------------------------------------------------------------"<<endl;
    cout<<"AVERAGES\n";
    cout<<"Average response time = "<<1.0 * performanceMetrics[0]/processes.size()<<"\n";
    cout<<"Average waiting time = "<<1.0 * performanceMetrics[1]/processes.size()<<"\n";
    cout<<"Average turnaround time = "<<1.0 * performanceMetrics[2]/processes.size()<<"\n";
    cout<<"Average penalty ratio = "<<(1.0 * avgPr)/processes.size()<<"\n";
    cout<<"System Throughput = "<<(1.0 * processes.size())/(currentTime - 1)<<"\n";
    return 0;
}