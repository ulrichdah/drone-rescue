#include <string>
#include <vector>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/datatypes/set.h>
using namespace argos;
class BeliefMap {
    public:
        BeliefMap(){}
        BeliefMap(const std::string& initial_belief_map_filename);

        float GetCaseBelief(int i, int j) {
            return belief_map_[i][j];
        }

        float GetWorldPositionBelief(const CVector2& position) {
            float world_offset_x = - size_x_/2 - 0.5;
            float world_offset_y = - size_y_/2 - 0.5;
            int i = static_cast<int>(std::round(position.GetX() - world_offset_x));
            int j = static_cast<int>(std::round(position.GetY() - world_offset_y));
            return belief_map_[i][j];
        }

        int GetSizeX() {
            return size_x_;
        }

        int GetSizeY() {
            return size_y_;
        }

        static void CreateFakeBeliefMap(const std::vector<std::pair<int, int>>& robots_position, int size_x, int size_y);

        static void CreateFakeBeliefMap(const std::vector<std::pair<int, int>>& robots_position, int size_x, int size_y,
                                    const std::string& existing_map);

        static float ComputeBelief(int i, int j, int robot_i, int robot_j, int damping);

    private:
        std::vector<std::vector<float>> belief_map_;
        int size_x_, size_y_;
};