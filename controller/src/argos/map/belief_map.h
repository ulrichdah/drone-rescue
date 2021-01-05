#include <string>
#include <vector>

class BeliefMap {
    public:
        BeliefMap(const std::string& initial_belief_map_filename);

        float GetCaseBelief(int i, int j) {
            return belief_map_[i][j];
        }

        int GetSizeX() {
            return size_x_;
        }

        int GetSizeY() {
            return size_y_;
        }

        static void CreateFakeBeliefMap(const std::vector<std::pair<int, int>>& robots_position, int size_x, int size_y);

        static float ComputeBelief(int i, int j, int robot_i, int robot_j, int damping);

    private:
        std::vector<std::vector<float>> belief_map_;
        int size_x_, size_y_;
};