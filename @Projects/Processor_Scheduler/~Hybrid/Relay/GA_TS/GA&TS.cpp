#include "include/modules.hpp"
#include "GA.hpp"
#include "tabu_search.hpp"

#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <random>

using namespace std;
using namespace os_display;
using namespace std::chrono;



 
  
int main() {
    Config cfg = ReadConfigFile("../../../datasets/n4_00.dag");
    


    int maxIter       = 500;    // 最大迭代次數  
    int tabuTenure    = 10;    // 禁忌期限  
    int numCandidates = 40;   // 一次產生的鄰居數量  

    double Avg_Makespan =0;
    double times = 1;


    GA_Params params_ga;
    params_ga.population_size = 20;
    params_ga.generations = 500;

    vector<double> GB_Recorder;
    vector<double> CB_Recorder;

    for (int i ;i<times;i++){
        
        
        Solution GA_Result = Genetic_Algorithm(cfg,params_ga ,  &GB_Recorder);
        cout<<"GA cost : "<<GA_Result.cost<<endl;
        Solution TS_Result = Tabu_Search(cfg, &GA_Result ,maxIter, tabuTenure, numCandidates , &GB_Recorder);
        Solution& best =  TS_Result;

        cout << "Best makespan: " << best.cost << "\n";
        ScheduleResult sr = Solution_Function(best, cfg , true);
        show_solution(best);
        cout << "Feasible: " << std::boolalpha << is_feasible(sr, cfg ,TS_Result.ms ) << "\n";
        cout << "Cost : " << sr.makespan;
        cout<<"\n\n";
        Avg_Makespan+=best.cost;
    }
    Avg_Makespan /= times;
    //system("cls");
    cout<<"\n\n";
    cout<<"Avg Makespan : "<<Avg_Makespan;


    writeTwoVectorsToFile(GB_Recorder,CB_Recorder,"data.txt");
    Call_Py_Visual();
    return 0;
}