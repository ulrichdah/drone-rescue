#include "belief_map.h"
#include <fstream>
#include <map>
#include <algorithm>
#include <cmath>

BeliefMap::BeliefMap(const std::string& initial_belief_map_filename){
            std::ifstream file(initial_belief_map_filename);
            file >> size_x_ >> size_y_;
            belief_map_ = std::vector<std::vector<float>>(size_x_);
            for (int i = 0; i <= size_x_; i++)
            {
                belief_map_[i] = std::vector<float>(size_y_);
                for (int j = 0; j <= size_y_; j++)
                {
                    file >> belief_map_[i][j];
                } 
            }  
        }

void BeliefMap::CreateFakeBeliefMap(const std::vector<std::pair<int, int>>& robots_position, int size_x, int size_y){
            std::ofstream file("/home/docker/simulation/sample_maps/fake0.txt");
            file << size_x << " " << size_y << std::endl;
            for (int i = 0; i <= size_x; i++)
            {
                for (int j = 0; j <= size_y; j++)
                {
                    float belief = 0.0;
                    for (int k = 0; k <= robots_position.size(); k++)
                    {
                        belief += ComputeBelief(i, j, robots_position[k].first, robots_position[k].second);
                    } 
                    if (belief > 1.0){
                        belief = 1.0;
                    }

                    file << belief << " ";
                } 
                file << std::endl;
            }  
        }

float BeliefMap::ComputeBelief(int i, int j, int robot_i, int robot_j){
            float distance = std::sqrt(std::pow(i-robot_i,2) + std::pow(j-robot_j,2));
            float belief = 1.0 - ( distance / 1.0 );
            if (belief > 1.0){
                belief = 1.0;
            }
            return belief;
        }