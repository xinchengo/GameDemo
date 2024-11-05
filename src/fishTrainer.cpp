#include<iostream>
#include<fstream>
#include<vector>

#include "gameRunner.hpp"

int main()
{
    std::vector<FishStrategy> strategies(100);
    HeadlessGameRunner runner(1280.f, 720.f);

    std::ofstream logfile("./bin/1.txt");
    logfile << "np.array([";

    for(int i=1; i<=200; i++)
    {
        auto output = runner.train(strategies);
        strategies = output.yieldingStrategies;
        logfile << "[";
        for(int j=0; j<100; j++)
        {
            logfile << output.lifespans[j] << (j == 99 ? "]" : ",");
        }
        logfile << "]";
        if(i != 200)
            logfile << ",";
        std::cerr << "Epoch " << i << " finished, " << "meanLifespan = " << output.meanLifespan << std::endl;
    }
    logfile << "])\n";
}