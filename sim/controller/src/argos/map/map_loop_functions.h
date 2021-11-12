#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/spiri/simulator/spiri_entity.h>
namespace argos {

class MapLoopFunctions : public CLoopFunctions {
   public:
      virtual void Init(TConfigurationNode& t_tree);
      virtual bool IsExperimentFinished();

   private:
      void AddBeliefBox(const CVector3& position, const CVector3& color);
      CVector3 ConvertBeliefToColor(float belief);
};

}