#include "belief_map.h"
#include <fstream>
#include <map>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

BeliefMap::BeliefMap(const std::string& initial_belief_map_filename){
            std::ifstream file(initial_belief_map_filename);
            file >> size_x_ >> size_y_;
            for (int i = 0; i < size_x_; i++)
            {
                belief_map_.push_back(std::vector<float>());
                for (int j = 0; j < size_y_; j++)
                {
                    float value;
                    file >> value;
                    belief_map_[i].push_back(value);
                } 
            }  
        }

void BeliefMap::CreateFakeBeliefMap(const std::vector<std::pair<int, int>>& robots_position, int size_x, int size_y){
            std::ofstream file("/home/docker/drone-rescue/sim/sample_maps/fake0.txt");
            file << size_x << " " << size_y << std::endl;
            file << std::fixed << std::setprecision(4);
            for (int i = 0; i < size_x; i++)
            {
                for (int j = 0; j < size_y; j++)
                {
                    float belief = 0.0;
                    for (int k = 0; k < robots_position.size(); k++)
                    {
                        int shifted_rposx = robots_position[k].first + size_x/2;
                        int shifted_rposy = robots_position[k].second + size_y/2;
                        belief += ComputeBelief(i, j, shifted_rposx, shifted_rposy, (size_x+size_y)/(2*(size_x/5)));
                    } 
                    if (belief > 1.0){
                        belief = 1.0;
                    }

                    file << belief << " ";
                } 
                file << std::endl;
            }  
        }

void BeliefMap::CreateFakeBeliefMap(const std::vector<std::pair<int, int>>& robots_position, int size_x, int size_y,
                                    const std::string& existing_map){ 
            // Read previous file                    
            std::ifstream previous_map(existing_map);  
            int temp;
            previous_map >> temp >> temp;
            std::vector<float> previous_beliefs;//(size_x*size_y);
            for (int i = 0; i < size_x; i++)
            {
                for (int j = 0; j < size_y; j++)
                {
                    float belief;
                    previous_map >> belief;
                    previous_beliefs.push_back(belief);
                } 
            } 

            // Write new file
            std::ofstream file("/home/docker/drone-rescue/sim/sample_maps/fake0.txt");
            file << size_x << " " << size_y << std::endl;
            file << std::fixed << std::setprecision(4);
            for (int i = 0; i < size_x; i++)
            {
                for (int j = 0; j < size_y; j++)
                {
                    float belief = previous_beliefs[i*size_x+j];
                    for (int k = 0; k < robots_position.size(); k++)
                    {
                        int shifted_rposx = robots_position[k].first + size_x/2;
                        int shifted_rposy = robots_position[k].second + size_y/2;
                        belief += ComputeBelief(i, j, shifted_rposx, shifted_rposy, (size_x+size_y)/(2*(size_x/5)));
                    } 
                    if (belief > 1.0){
                        belief = 1.0;
                    }

                    file << belief << " ";
                } 
                file << std::endl;
            }    
        }

float BeliefMap::ComputeBelief(int i, int j, int robot_i, int robot_j, int damping){
            float distance = std::sqrt(std::pow(((float)(i-robot_i)-0.5)/damping,2) + std::pow(((float)(j-robot_j)-0.5)/damping,2));
            float belief = 1.0 - distance;
            if (belief > 1.0){
                belief = 1.0;
            } else if (belief < 0.0){
                belief = 0.0;
            }
            return belief;
        }