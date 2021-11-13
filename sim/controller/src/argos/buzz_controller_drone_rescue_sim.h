#ifndef BUZZ_CONTROLLER_DRONE_RESCUE_SIM_H
#define BUZZ_CONTROLLER_DRONE_RESCUE_SIM_H

#include <buzz/argos/buzz_controller_spiri.h>
#include <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_colored_blob_perspective_camera_sensor.h>
#include "map/belief_map.h"

#include <random>
#include <chrono>

using namespace argos;

namespace buzz_drone_rescue_sim {

/*
* Buzz controller
*/
class CBuzzControllerDroneRescueSim : public CBuzzControllerSpiri {

public:

   CBuzzControllerDroneRescueSim();
   
   virtual ~CBuzzControllerDroneRescueSim();

   virtual void Init(TConfigurationNode& t_node);

   // Control functions
   void GoTo(const CVector2& position);

   std::default_random_engine& GetRandomEngine()
   {
      return random_engine_;
   }

   bool HasReached(const CVector2& position, const float& delta);

   int DetectTargets();

   std::pair<float, float> GetGPSPosition();

   float GetBelief(const CVector2& position);

   std::string GetCurrentKey();

   void LogRelay(const int& step, const int& id);

   void LogFoundTarget(const int& step, const int& id);

   void LogDatasize(const int& step, const int& id, const int& datasize);

   bool experiment_done;
protected:

   virtual buzzvm_state RegisterFunctions();

private:

   std::default_random_engine random_engine_;
   BeliefMap belief_map_;
};
}
#endif
