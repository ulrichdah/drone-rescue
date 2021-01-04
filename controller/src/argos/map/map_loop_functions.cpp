#include "map_loop_functions.h"
#include <argos3/plugins/simulator/entities/box_entity.h>
namespace argos {

void MapLoopFunctions::Init(TConfigurationNode& t_tree) {
   /* Create the box */
   CBoxEntity* b = new CBoxEntity("box",                    // the id
                                 CVector3(0.0,-12.0,0.0),     // the position of the base center in m
                                 CQuaternion(),             // the orientation
                                 false,                     // this box is not movable
                                 CVector3(24.0,4.0,0.01),// the size of the box in m
                                 1);                        // the mass in kg
   b->SetColor(CVector3(96.0f/255, 145.0f/255, 65.0f/255), 0.5f);
   /* Add the box to the space */
   AddEntity(*b);
}

}

REGISTER_LOOP_FUNCTIONS(MapLoopFunctions, "map_loop_functions");