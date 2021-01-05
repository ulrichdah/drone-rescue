#include "map_loop_functions.h"
#include "belief_map.h"
#include <argos3/plugins/simulator/entities/box_entity.h>
#include <iostream>
#include <stdlib.h> 
namespace argos {

void MapLoopFunctions::Init(TConfigurationNode& t_tree) {
   // Create fake belief map
   std::vector<std::pair<int, int>> robots_position;
   robots_position.push_back(std::make_pair(0,0));
   robots_position.push_back(std::make_pair(7,7));
   robots_position.push_back(std::make_pair(-7,-7));
   BeliefMap::CreateFakeBeliefMap(robots_position, 30, 30);

   // Indicate grass area
   CBoxEntity* grass = new CBoxEntity("grass",                    // the id
                                 CVector3(0.0,-12.0,0.0),     // the position of the base center in m
                                 CQuaternion(),             // the orientation
                                 false,                     // this box is not movable
                                 CVector3(24.0,4.0,0.01),// the size of the box in m
                                 1);                        // the mass in kg
   grass->SetColor(CVector3(96.0f/255, 145.0f/255, 65.0f/255), 0.5f);
   AddEntity(*grass);
   
   // Draw belief map
   BeliefMap belief_map("/home/docker/simulation/sample_maps/fake0.txt");
   float world_offset_x = - belief_map.GetSizeX()/2 + 0.5;
   float world_offset_y = - belief_map.GetSizeY()/2 + 0.5;
   for (int i = 0; i < belief_map.GetSizeX(); i++)
   {
      for (int j = 0; j < belief_map.GetSizeY(); j++)
      {
         AddBeliefBox(CVector3(i + world_offset_x, j + world_offset_y, 0.0),
                     ConvertBeliefToColor(belief_map.GetCaseBelief(i,j)));
      } 
   }

   // Indicate water area
   CBoxEntity* water = new CBoxEntity("water",                    // the id
                                 CVector3(0.0,0.0,0.0),     // the position of the base center in m
                                 CQuaternion(),             // the orientation
                                 false,                     // this box is not movable
                                 CVector3(50.0,50.0,0.01),// the size of the box in m
                                 1);                        // the mass in kg
   water->SetColor(CVector3(1.0, 0.0, 0.0), 0.5f);
   AddEntity(*water);
}

void MapLoopFunctions::AddBeliefBox(const CVector3& position, const CVector3& color){
   // Create the grass space
   std::cout << position.GetX() << " " << position.GetY() << " " << position.GetZ() << std::endl;
   CBoxEntity* box = new CBoxEntity("c" + std::to_string((int)(position.GetX())) + "_" + std::to_string((int)(position.GetY())) + "_" + std::to_string((int)(rand()*1000)),  // the id
                                 CVector3(position.GetX(),position.GetY(),position.GetZ()),     // the position of the base center in m
                                 CQuaternion(),             // the orientation
                                 false,                     // this box is not movable
                                 CVector3(1.0,1.0,0.01),    // the size of the box in m
                                 1);                        // the mass in kg
   box->SetColor(color, 0.5f);
   AddEntity(*box);
}

CVector3 MapLoopFunctions::ConvertBeliefToColor(float belief){
   CVector3 color(2.0f * (1 - belief), 2.0f * belief, 0);
   return color;
}

}

REGISTER_LOOP_FUNCTIONS(MapLoopFunctions, "map_loop_functions");