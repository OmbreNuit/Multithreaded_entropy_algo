#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <utility> // std::pair
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <pthread.h>

struct CPUInfo{
    std::string tasks;
    std::vector<int> cpuCount;
};

struct ThreadData {
    CPUInfo* info;
    std::vector<double>* entropyResult;
};

//               My entropy calculation based on Dr.Rincon's incremental entropy algorithm.
void* CalculateEntropy(void* threadData){
    ThreadData* data = (ThreadData *) threadData;

    std::unordered_set<char> seen;// use of unordered_set to keep track of task char already seen before
    std::vector<char> unique;
    for( char c: data->info->tasks){// removing duplicate Task char without changing the order
        if(seen.insert(c).second){
            unique.push_back(c);
        }
    }  
    // creating and initalizing FreqArray with no duplicate Tasks char and 0 frequency
    std::vector<std::pair<char, int>> NewFreqArray;  
    for(auto c: unique){
        NewFreqArray.push_back(std::make_pair(c,0));
    }
    
    std::vector<double> H;
    int FreqOfSelectedTask = 0;
    double h = 0.0;
    int currFreq = 0;
    double currH = 0.0;
    double currentTerm = 0;
    int size = data->info->tasks.size(); // size is calculated based on the number of tasks present in the given string
    
    //entropy calculation
    for(int i = 0; i < size; i++){
        char selectedTask = data->info->tasks.at(i);
        int extraFreq = data->info->cpuCount[i];
        int NFreq = currFreq + extraFreq;
        
        if(NFreq == extraFreq){
            h = 0.00;
        }
        else{
            for(int i = 0; i < NewFreqArray.size(); i++){
                if(selectedTask == NewFreqArray.at(i).first){
                    FreqOfSelectedTask = NewFreqArray.at(i).second;
                }
            }
            if(FreqOfSelectedTask == 0){
                currentTerm = 0;
            }else{
                currentTerm = FreqOfSelectedTask * std::log2(FreqOfSelectedTask);
            }
            double newTerm = (FreqOfSelectedTask + extraFreq) * log2(FreqOfSelectedTask + extraFreq);
            if(NFreq != 0){
                h = log2(NFreq) - ((log2(currFreq) - currH) * currFreq - currentTerm + newTerm) / NFreq;
            }
            else{
                h = 0.00;
            }
        }
        //Updating the New Frequency array 
        for(int i = 0; i < NewFreqArray.size(); i++){
            if(NewFreqArray.at(i).first == selectedTask){
                NewFreqArray.at(i).second += extraFreq;
            }
        }
        //updating h and frequency variables
        currH = h;
        H.push_back(h); //adding the new entropy value to vector H
        currFreq = NFreq;
    }
    // Store the result in the shared memory location.
    *(data->entropyResult) = H; // H is a vector<double> that has the entropy values

    pthread_exit(NULL);
}

int main(){
    std::string input;
    std::vector<CPUInfo> schedulingInfo;
    int nStrings = 0;
    
    //reading in input and stroing them in Struct CPUInfo
    while (getline(std::cin, input)) {
        CPUInfo info;
        info.tasks = "";
        std::string task;
        int count;
    
        std::istringstream iss(input);
        while (iss >> task >> count) {
            info.tasks += task;
            info.cpuCount.push_back(count);
        }
        schedulingInfo.push_back(info);
        nStrings++;
    }

    //Create threads
    pthread_t *tid = new pthread_t[nStrings];
    ThreadData threadData[nStrings];
    std::vector<std::vector<double>> entropyResults(nStrings, std::vector<double>());
    
    //passing in thread infomation and pthread_create
    for(int i = 0; i < nStrings; i++){
        threadData[i].info = &schedulingInfo[i];
        threadData[i].entropyResult = &entropyResults[i];
        
        pthread_create(&tid[i], nullptr, CalculateEntropy, &threadData[i]);
    }
    
    // wait for threads to finish and join
    for(int i = 0; i < nStrings; i++){
        pthread_join(tid[i], nullptr);
    }
    
    // Print the entropy values.
    for (int i = 0; i < nStrings; ++i) {
        std::cout << "CPU " << (i + 1) << std::endl;
        std::cout << "Task scheduling information: ";
        for (size_t j = 0; j < schedulingInfo[i].tasks.size()-1; ++j) {
            std::cout << schedulingInfo[i].tasks[j] << "(" << schedulingInfo[i].cpuCount[j] << "), ";
        }
        std::cout << schedulingInfo[i].tasks[schedulingInfo[i].tasks.size()-1] << "(" << schedulingInfo[i].cpuCount[schedulingInfo[i].tasks.size()-1] << ")";
        std::cout << std::endl;
        
        std::cout << "Entropy for CPU " << (i + 1) << std::endl;
        for (double entropy : entropyResults[i]) {
            std::cout << std::fixed << std::setprecision(2) << entropy << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
    
    delete[] tid;
    return 0;
}

