#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <fstream>
#include <iostream>
#include <Process.h>
#include <Hardware.h>
#include <bits/stdc++.h>

using namespace std;

void print_metrics(unordered_map<string, vector<double>> &metrics, string output_file_path);
void read_process_file(string file_path, unordered_map<int, queue<int>> &all_process_dict);
bool preempt_existing_process(Process **cpu_process, Process **new_process);

#endif