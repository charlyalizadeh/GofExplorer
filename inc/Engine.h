#ifndef KNIGHTCA_ENGINE_H_
#define KNIGHTCA_ENGINE_H_

#include "../lib/olcPixelGameEngine.h"
#include <vector>
#include <array>
#include <random>
#include <iostream>


enum class NeighborType {
    Conway,
    Knight
};

enum CellState {
    Dead=0,
    Alive=1,
};

class Engine: public olc::PixelGameEngine {
    public:
        Engine();

        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;
        bool OnUserDestroy() override;

        void set_state(int index, int value);
        void set_state(int i, int j, int value);
        void set_rules_factors(const std::array<int,3>& rules_factors);
        void set_rule(int index, int r);
        void next_rule();
        void previous_rule();
        void set_frequency(float frequency);

        int get_state(int index);
        int get_state(int i, int j);
        int get_rule(int r);
        std::vector<int> get_states();
        float get_frequency();

    private:
        void draw_states();
        void init_states(float frequency);
        void next_iteration();
        int get_living_neighbors_knight(int i, int j);
        int get_living_neighbors_regular(int i, int j);
        void display_help();

    private:
        std::vector<int> m_states;
        std::array<int,3> m_rules_factors;
        float m_frequency;
        NeighborType m_neighbor_type;
};

#endif // KNIGHTCA_ENGINE_H_
