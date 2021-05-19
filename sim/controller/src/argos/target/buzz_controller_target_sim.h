#ifndef BUZZ_CONTROLLER_TARGET_SIM_H
#define BUZZ_CONTROLLER_TARGET_SIM_H

#include <buzz/argos/buzz_controller_footbot.h>
#include <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_colored_blob_perspective_camera_sensor.h>

#include <random>
#include <chrono>

using namespace argos;

namespace buzz_drone_rescue_sim {

/*
* Buzz controller
*/
class CBuzzControllerTargetSim : public CBuzzControllerFootBot {

public:

   CBuzzControllerTargetSim();
   
   virtual ~CBuzzControllerTargetSim();

   virtual void Init(TConfigurationNode& t_node);

   // Control functions
   void SetVelocity(const CVector2& c_heading);

   std::default_random_engine& GetRandomEngine()
   {
      return random_engine_;
   }

protected:

   virtual buzzvm_state RegisterFunctions();

private:

   std::default_random_engine random_engine_;
   
};
}
#endif
