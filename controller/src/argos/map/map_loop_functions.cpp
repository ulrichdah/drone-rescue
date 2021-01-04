#include "map_loop_functions.h"
#include <argos3/plugins/simulator/entities/box_entity.h>
namespace argos {
void MapLoopFunctions::Init(TConfigurationNode& t_tree) {
   /* Create the box */
   CBoxEntity* b = new CBoxEntity("box",                 // the id
                                 CVector3(1,1,1),       // the position of the base center in m
                                 CQuaternion(),         // the orientation
                                 true,                  // this box is movable
                                 CVector3(0.1,0.1,0.1), // the size of the box in m
                                 1);                    // the mass in kg
   /* Add the box to the space */
   AddEntity(*b);
}
}
REGISTER_LOOP_FUNCTIONS(MapLoopFunctions, "map_loop_functions");