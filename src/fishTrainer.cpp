#include<iostream>
#include<vector>

#include "gameRunner.hpp"

int main()
{
    std::vector<FishStrategy> strategies(100);
    EvolutionGameRunner runner(1280.f, 720.f);
    // std::freopen("1.txt", "w", stdout);
    // std::cout << "np.array([";
    for(int i=1; i<=500; i++)
    {
        auto output = runner.train(strategies);
        strategies = output.yieldingStrategies;
        // std::cout << output.meanLifespan << ",]"[i==500];
        std::cerr << "Epoch " << i << " finished, " << "meanLifespan = " << output.meanLifespan << std::endl;
    }
    // std::cout << ")\n";
}