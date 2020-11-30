#include "buzz_controller_target_sim.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>

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
}

/****************************************/
/****************************************/

void CBuzzControllerTargetSim::SetVelocity(const CVector2& c_heading) {
   m_pcWheelsA->SetLinearVelocity(c_heading.GetX(), c_heading.GetY());
}

}