#include "map_loop_functions.h"
#include <argos3/plugins/simulator/entities/box_entity.h>
namespace argos {
void MapLoopFunctions::Init(TConfigurationNode& t_tree) {
   /* Create the box */
   CBoxEntity* b = new CBoxEntity("box",                    // the id
                                 CVector3(0.5,0.5,0.0),     // the position of the base center in m
                                 CQuaternion(),             // the orientation
                                 false,                     // this box is movable
                                 CVector3(100.0,100.0,0.01),// the size of the box in m
                                 1);                        // the mass in kg
   b->SetColor(CVector3(0.0f,0.5f,0.1f));
   /* Add the box to the space */
   AddEntity(*b);
}
}
REGISTER_LOOP_FUNCTIONS(MapLoopFunctions, "map_loop_functions");