#include <argos3/core/simulator/loop_functions.h>
namespace argos {

class MapLoopFunctions : public CLoopFunctions {
   public:
      virtual void Init(TConfigurationNode& t_tree);
};

}