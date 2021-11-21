#include "buzz_controller_drone_rescue_sim.h"
#include "utils/buzz_utils.h"

using namespace buzz_utils;

namespace buzz_drone_rescue_sim {

/****************************************/
/****************************************/

static int BuzzRandUniform(buzzvm_t vm){

   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   buzzobj_t buzz_range_lowest = buzzvm_stack_at(vm, 2);
   buzzobj_t buzz_range_highest = buzzvm_stack_at(vm, 1);
   float range_lowest, range_highest;

   if(buzz_range_lowest->o.type == BUZZTYPE_FLOAT) range_lowest = buzz_range_lowest->f.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "uniform(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[buzz_range_lowest->o.type]
         );
      return vm->state;
   } 
   if(buzz_range_highest->o.type == BUZZTYPE_FLOAT) range_highest = buzz_range_highest->f.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "uniform(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[buzz_range_highest->o.type]
         );
      return vm->state;
   } 

   std::uniform_real_distribution<float> distribution(range_lowest, range_highest);

   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   
   /* Call function */
   float random_value = distribution(
      reinterpret_cast<CBuzzControllerDroneRescueSim*>(buzzvm_stack_at(vm, 1)->u.value)->GetRandomEngine());

   buzzvm_pushf(vm, random_value);

   return buzzvm_ret1(vm);
}

/****************************************/
/****************************************/

static int BuzzGoto(buzzvm_t vm) {
   /* Push the vector components */
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   /* Create a new vector with that */
   CVector2 position;
   buzzobj_t tX = buzzvm_stack_at(vm, 2);
   buzzobj_t tY = buzzvm_stack_at(vm, 1);
   if(tX->o.type == BUZZTYPE_INT) position.SetX(tX->i.value);
   else if(tX->o.type == BUZZTYPE_FLOAT) position.SetX(tX->f.value);
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "goto_abs(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[tX->o.type]
         );
      return vm->state;
   }      
   if(tY->o.type == BUZZTYPE_INT) position.SetY(tY->i.value);
   else if(tY->o.type == BUZZTYPE_FLOAT) position.SetY(tY->f.value);
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "goto_abs(x,y): expected %s, got %s in second argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[tY->o.type]
         );
      return vm->state;
   }
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   reinterpret_cast<CBuzzControllerDroneRescueSim*>(buzzvm_stack_at(vm, 1)->u.value)->GoTo(position);
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

static int BuzzRandGauss(buzzvm_t vm){

   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   buzzobj_t buzz_mean = buzzvm_stack_at(vm, 2);
   buzzobj_t buzz_stdev = buzzvm_stack_at(vm, 1);
   int mean, stdev;

   if(buzz_mean->o.type == BUZZTYPE_INT) mean = buzz_mean->i.value;
   else if(buzz_mean->o.type == BUZZTYPE_FLOAT) mean = (int)buzz_mean->f.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "gauss(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_INT],
                      buzztype_desc[buzz_mean->o.type]
         );
      return vm->state;
   } 
   if(buzz_stdev->o.type == BUZZTYPE_INT) stdev = buzz_stdev->i.value;
   else if(buzz_stdev->o.type == BUZZTYPE_FLOAT) stdev = (int)buzz_stdev->f.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "gauss(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_INT],
                      buzztype_desc[buzz_stdev->o.type]
         );
      return vm->state;
   } 
   std::normal_distribution<float> distribution(mean, stdev);
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   float random_value = distribution(
      reinterpret_cast<CBuzzControllerDroneRescueSim*>(buzzvm_stack_at(vm, 1)->u.value)->GetRandomEngine());

   buzzvm_pushf(vm, random_value);

   return buzzvm_ret1(vm);
}

/****************************************/
/****************************************/

static int BuzzHasReached(buzzvm_t vm) {
   /* Push the vector components */
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   buzzvm_lload(vm, 3);
   /* Create a new vector with that */
   CVector2 position;
   float delta;
   buzzobj_t tX = buzzvm_stack_at(vm, 3);
   buzzobj_t tY = buzzvm_stack_at(vm, 2);
   buzzobj_t tDelta = buzzvm_stack_at(vm, 1);
   if(tX->o.type == BUZZTYPE_FLOAT) position.SetX(tX->f.value);
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "goto_abs(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[tX->o.type]
         );
      return vm->state;
   }      
   if(tY->o.type == BUZZTYPE_FLOAT) position.SetY(tY->f.value);
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "goto_abs(x,y): expected %s, got %s in second argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[tY->o.type]
         );
      return vm->state;
   }
   if(tDelta->o.type == BUZZTYPE_FLOAT) delta = tDelta->f.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "goto_abs(x,y): expected %s, got %s in second argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[tDelta->o.type]
         );
      return vm->state;
   }
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   bool has_reached = 
      reinterpret_cast<CBuzzControllerDroneRescueSim*>(buzzvm_stack_at(vm, 1)->u.value)->HasReached(position, delta);
   
   buzzvm_pushi(vm, (int) has_reached);

   return buzzvm_ret1(vm);
}

/****************************************/
/****************************************/

static int BuzzDetectTargets(buzzvm_t vm) {
   
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   int number_of_targets = 
      reinterpret_cast<CBuzzControllerDroneRescueSim*>(buzzvm_stack_at(vm, 1)->u.value)->DetectTargets();
   
   buzzvm_pushi(vm, number_of_targets);

   return buzzvm_ret1(vm);
}

/****************************************/
/****************************************/

static int BuzzUpdateGPSPosition(buzzvm_t vm) {
   
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   std::pair<float, float> position = 
      reinterpret_cast<CBuzzControllerDroneRescueSim*>(buzzvm_stack_at(vm, 1)->u.value)->GetGPSPosition();
   
   buzzobj_t b_gps_position = buzzheap_newobj(vm, BUZZTYPE_TABLE);
   TablePut(vm, b_gps_position, "x", position.first);
   TablePut(vm, b_gps_position, "y", position.second);
   Register(vm, "gps_position", b_gps_position);

   return buzzvm_ret0(vm);
}


/****************************************/
/****************************************/

static int BuzzGetBelief(buzzvm_t vm) {
   /* Push the vector components */
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   /* Create a new vector with that */
   CVector2 position;
   buzzobj_t tX = buzzvm_stack_at(vm, 2);
   buzzobj_t tY = buzzvm_stack_at(vm, 1);
   if(tX->o.type == BUZZTYPE_INT) position.SetX(tX->i.value);
   else if(tX->o.type == BUZZTYPE_FLOAT) position.SetX(tX->f.value);
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "goto_abs(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[tX->o.type]
         );
      return vm->state;
   }      
   if(tY->o.type == BUZZTYPE_INT) position.SetY(tY->i.value);
   else if(tY->o.type == BUZZTYPE_FLOAT) position.SetY(tY->f.value);
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "goto_abs(x,y): expected %s, got %s in second argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[tY->o.type]
         );
      return vm->state;
   }
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   float belief = reinterpret_cast<CBuzzControllerDroneRescueSim*>(buzzvm_stack_at(vm, 1)->u.value)->GetBelief(position);
   
   buzzvm_pushf(vm, belief);
   return buzzvm_ret1(vm);
}

static int BuzzGetCurrentKey(buzzvm_t vm) {
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   std::string key_value =
      reinterpret_cast<CBuzzControllerDroneRescueSim*>(buzzvm_stack_at(vm, 1)->u.value)->GetCurrentKey();

   buzzvm_pushs(vm, buzzvm_string_register(vm, key_value.c_str(), 1));

   return buzzvm_ret1(vm);
}

static int BuzzLogFoundTarget(buzzvm_t vm) {
   /* Push the vector components */
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);

   int step;
   int id;
   buzzobj_t tstep = buzzvm_stack_at(vm, 2);
   buzzobj_t tid = buzzvm_stack_at(vm, 1);
   id = tid->i.value;
   if(tstep->o.type == BUZZTYPE_INT) step = tstep->i.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "log_found_target(step, id): expected %s, got %s in second argument",
                      buzztype_desc[BUZZTYPE_INT],
                      buzztype_desc[tstep->o.type]
         );
      return vm->state;
   }

   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   reinterpret_cast<CBuzzControllerDroneRescueSim*>(buzzvm_stack_at(vm, 1)->u.value)->LogFoundTarget(step, id);

   return buzzvm_ret0(vm);
}

static int BuzzLogRelay(buzzvm_t vm) {
   /* Push the vector components */
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   buzzvm_lload(vm, 3);

   int step;
   int id;
   int last_relay;
   buzzobj_t tstep = buzzvm_stack_at(vm, 3);
   buzzobj_t tid = buzzvm_stack_at(vm, 2);
   buzzobj_t tlast_relay = buzzvm_stack_at(vm, 1);
   id = tid->i.value;
   last_relay = tlast_relay->i.value;
   if(tstep->o.type == BUZZTYPE_INT) step = tstep->i.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "log_relay(step, id, last_relay): expected %s, got %s in second argument",
                      buzztype_desc[BUZZTYPE_INT],
                      buzztype_desc[tstep->o.type]
         );
      return vm->state;
   }

   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   reinterpret_cast<CBuzzControllerDroneRescueSim*>(buzzvm_stack_at(vm, 1)->u.value)->LogRelay(step, id, last_relay);

   return buzzvm_ret0(vm);
}

static int BuzzLogDatasize(buzzvm_t vm) {
   /* Push the vector components */
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   buzzvm_lload(vm, 3);

   int step;
   int id;
   int datasize;
   buzzobj_t tstep = buzzvm_stack_at(vm, 3);
   buzzobj_t tid = buzzvm_stack_at(vm, 2);
   buzzobj_t tdatasize = buzzvm_stack_at(vm, 1);
   id = tid->i.value;
   if(tstep->o.type == BUZZTYPE_INT) step = tstep->i.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "log_datasize(step, id, datasize): expected %s, got %s in second argument",
                      buzztype_desc[BUZZTYPE_INT],
                      buzztype_desc[tstep->o.type]
         );
      return vm->state;
   }
   if(tdatasize->o.type == BUZZTYPE_INT) datasize = tdatasize->i.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "log_datasize(step, id, datasize): expected %s, got %s in third argument",
                      buzztype_desc[BUZZTYPE_INT],
                      buzztype_desc[tstep->o.type]
         );
      return vm->state;
   }

   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   reinterpret_cast<CBuzzControllerDroneRescueSim*>(buzzvm_stack_at(vm, 1)->u.value)->LogDatasize(step, id, datasize);

   return buzzvm_ret0(vm);
}

static int BuzzExperimentDone(buzzvm_t vm) {

   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   reinterpret_cast<CBuzzControllerDroneRescueSim*>(buzzvm_stack_at(vm, 1)->u.value)->experiment_done = true;

   return buzzvm_ret0(vm);
}

/****************************************/
/************ Registration **************/
/****************************************/

buzzvm_state CBuzzControllerDroneRescueSim::RegisterFunctions() {
   CBuzzControllerSpiri::RegisterFunctions();

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "uniform", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzRandUniform));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "gauss", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzRandGauss));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "goto", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzGoto));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "has_reached", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzHasReached));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "detect_targets", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzDetectTargets));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "update_gps_position", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzUpdateGPSPosition));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "get_belief", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzGetBelief));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "get_current_key", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzGetCurrentKey));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "log_found_target", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzLogFoundTarget));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "log_relay", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzLogRelay));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "log_datasize", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzLogDatasize));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "experiment_done", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzExperimentDone));
   buzzvm_gstore(m_tBuzzVM);

   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CBuzzControllerDroneRescueSim, "buzz_controller_drone_rescue_sim");

}