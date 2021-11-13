#include "buzz_controller_target_sim.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include "../map/belief_map.h"
namespace buzz_drone_rescue_sim {


/****************************************/
/****************************************/

CBuzzControllerTargetSim::CBuzzControllerTargetSim() : CBuzzControllerFootBot() {
   std::chrono::high_resolution_clock::time_point previous = 
      std::chrono::high_resolution_clock::now();
   usleep(10);
   std::chrono::high_resolution_clock::duration duration(
      std::chrono::high_resolution_clock::now() -  previous);
   random_engine_.seed(duration.count());
}

/****************************************/
/****************************************/

CBuzzControllerTargetSim::~CBuzzControllerTargetSim() {
}

/****************************************/
/****************************************/

void CBuzzControllerTargetSim::Init(TConfigurationNode& t_node)  {
   CBuzzControllerFootBot::Init(t_node);
   // Create fake belief map
   CVector3 position = m_pcPos->GetReading().Position;
   std::vector<std::pair<int, int>> robots_position;
   robots_position.push_back(std::make_pair( position.GetX(), position.GetY()) );

   std::ifstream f("/home/docker/drone-rescue/sim/sample_maps/fake1.txt");
   if (f.good()) {
      BeliefMap::CreateFakeBeliefMap(robots_position, 40, 40, "/home/docker/drone-rescue/sim/sample_maps/fake1.txt");
   }
   else {
      BeliefMap::CreateFakeBeliefMap(robots_position, 40, 40);
   }
}

/****************************************/
/****************************************/

void CBuzzControllerTargetSim::SetVelocity(const CVector2& c_heading) {
   m_pcWheelsA->SetLinearVelocity(c_heading.GetX(), c_heading.GetY());
}

}