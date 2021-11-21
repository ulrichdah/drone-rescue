#include "buzz_controller_drone_rescue_sim.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <cmath>

namespace buzz_drone_rescue_sim {


const std::string TARGET_FILE = "/home/docker/drone-rescue/sim/controller/src/argos/results/target_4040_10_random.txt";
const std::string RELAY_FILE = "/home/docker/drone-rescue/sim/controller/src/argos/results/relay_4040_10_random.txt";
const std::string DATASIZE_FILE = "/home/docker/drone-rescue/sim/controller/src/argos/results/datasize_4040_10_random.txt";

/****************************************/
/****************************************/

CBuzzControllerDroneRescueSim::CBuzzControllerDroneRescueSim() : CBuzzControllerSpiri() {
   random_engine_.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
   experiment_done = false;
}

/****************************************/
/****************************************/

CBuzzControllerDroneRescueSim::~CBuzzControllerDroneRescueSim() {
}

/****************************************/
/****************************************/

void CBuzzControllerDroneRescueSim::Init(TConfigurationNode& t_node)  {
   CBuzzControllerSpiri::Init(t_node);
   m_pcCamera->Enable();
   belief_map_ = BeliefMap("/home/docker/drone-rescue/sim/sample_maps/fake1.txt");
}

/****************************************/
/****************************************/

float CBuzzControllerDroneRescueSim::GetBelief(const CVector2& position) {
   return belief_map_.GetWorldPositionBelief(position);
}
   
/****************************************/
/****************************************/

void CBuzzControllerDroneRescueSim::GoTo(const CVector2& position) {
   CVector3 new_position;

   new_position.SetX(position.GetX());
   new_position.SetY(position.GetY());
   new_position.SetZ(5.0f); // To ensure that the quadrotor flies
   
   m_pcPropellers->SetAbsolutePosition(new_position);
}

/****************************************/
/****************************************/

bool CBuzzControllerDroneRescueSim::HasReached(const CVector2& position, const float& delta) {
   float difference = std::sqrt(
      std::pow(m_pcPos->GetReading().Position.GetX() - position.GetX(),2)+
      std::pow(m_pcPos->GetReading().Position.GetY() - position.GetY(),2));

   return difference < delta;   
}

/****************************************/
/****************************************/

std::pair<float, float> CBuzzControllerDroneRescueSim::GetGPSPosition() {
   std::pair<float, float> position;
   position.first = m_pcPos->GetReading().Position.GetX();
   position.second = m_pcPos->GetReading().Position.GetY();
   return position;   
}

/****************************************/
/****************************************/

int CBuzzControllerDroneRescueSim::DetectTargets() {
   int number_of_blobs = m_pcCamera->GetReadings().BlobList.size();
   return number_of_blobs;  
}

/****************************************/
/****************************************/

std::string CBuzzControllerDroneRescueSim::GetCurrentKey() {
   int x = static_cast<int>(std::rint(m_pcPos->GetReading().Position.GetX()));
   int y = static_cast<int>(std::rint(m_pcPos->GetReading().Position.GetY()));
   std::string key = std::to_string(x) + '_' + std::to_string(y);
   return key;
}

/****************************************/
/****************************************/

void CBuzzControllerDroneRescueSim::LogFoundTarget(const int& step, const int& id){
   
   std::ofstream result_file;
   result_file.open(TARGET_FILE, std::ios::out | std::ios::app);

   result_file << step << "," << id << std::endl;
}

/****************************************/
/****************************************/

void CBuzzControllerDroneRescueSim::LogRelay(const int& step, const int& id, const int& last_relay){
   
   std::ofstream result_file;
   result_file.open(RELAY_FILE, std::ios::out | std::ios::app);
   if (last_relay == 0) {
      result_file << step << "," << id << ", last" << std::endl;
   } else {
      result_file << step << "," << id << std::endl;
   }
}

void CBuzzControllerDroneRescueSim::LogDatasize(const int& step, const int& id, const int& datasize) {
   std::ofstream result_file;
   result_file.open(DATASIZE_FILE, std::ios::out | std::ios::app);

   if (id == -1)
      result_file << step << "," << datasize << "," << "ROOT" << std::endl;
   else
      result_file << step << "," << datasize << "," << id << std::endl;

}

}
