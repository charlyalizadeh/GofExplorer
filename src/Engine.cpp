#include "../inc/Engine.h"

int correct_coordinate(int coord, int limit) {
    if(coord < 0) {
        return limit + coord; 
    }
    else if(coord >= limit) {
        return coord - limit; 
    }
    return coord;
}

Engine::Engine() {
    m_rules_factors = {3, 2, 3};
    m_frequency = 0.5;
};

bool Engine::OnUserCreate() {
    m_states = std::vector<int>(ScreenWidth() * ScreenHeight(), 0);
    init_states(m_frequency);
    return true;
}
bool Engine::OnUserUpdate(float fElapsedTime) {
    draw_states();
    if(GetKey(olc::Key::N).bPressed) {
        next_rule();
    }
    if(GetKey(olc::Key::P).bPressed) {
        previous_rule();
    }
    if(GetKey(olc::Key::CTRL).bHeld && GetKey(olc::Key::L).bPressed) {
        m_states = std::vector<int>(m_states.size(), 0);
    } 
    if(GetKey(olc::Key::R).bPressed) {
        m_states = std::vector<int>(m_states.size(), 0);
        init_states(m_frequency);
    } 
    if(GetKey(olc::Key::UP).bPressed) {
        if(GetKey(olc::Key::SHIFT).bHeld)
            set_frequency(get_frequency() + 0.1);
        else
            set_frequency(get_frequency() + 0.01);
    } 
    if(GetKey(olc::Key::DOWN).bPressed) {
        if(GetKey(olc::Key::SHIFT).bHeld)
            set_frequency(get_frequency() - 0.1);
        else
            set_frequency(get_frequency() - 0.01);
    } 
    if(GetKey(olc::Key::ENTER).bHeld || GetKey(olc::Key::RIGHT).bPressed) {
        next_iteration();
    }
    if(GetKey(olc::Key::H).bHeld || GetKey(olc::Key::H).bPressed) {
        display_help();
    }
    return true;
}
bool Engine::OnUserDestroy() {
    return true; 
}

void Engine::set_state(int index, int value) {
    m_states[index] = value;
}
void Engine::set_state(int i, int j, int value) {
    m_states[i * ScreenWidth() + j] = value;
}
void Engine::set_rules_factors(const std::array<int,3>& rules_factors) {
    m_rules_factors = rules_factors;
}
void Engine::set_rule(int index, int r) {
    m_rules_factors[index] = r;
}
void Engine::next_rule() {
    // Not smart I will admit
    if(m_rules_factors[2] < 8) {
        m_rules_factors[2]++;
    }
    else if(m_rules_factors[1] < 8) {
        m_rules_factors[2] = 0;
        m_rules_factors[1]++;
    }
    else if(m_rules_factors[0] < 8) {
        m_rules_factors[2] = 0;
        m_rules_factors[1] = 0;
        m_rules_factors[0]++;
    }
}
void Engine::previous_rule() {
    if(m_rules_factors[2] == 0) {
        if(m_rules_factors[1] == 0 && m_rules_factors[0] != 0) {
            m_rules_factors[0]--;
            m_rules_factors[1] = 8;
            m_rules_factors[0] = 8;
        }
        else if(m_rules_factors[1] != 0) {
            m_rules_factors[1]--;
            m_rules_factors[2] = 8;
        }
    }
    else {
        m_rules_factors[2]--;
    }
}
void Engine::set_frequency(float frequency) {
    if(frequency >= 0 && frequency <= 1) {
        m_frequency = frequency;
    }
}

int Engine::get_state(int index) {
    return m_states[index];
}
int Engine::get_state(int i, int j) {
    return m_states[i * ScreenWidth() + j];
}
int Engine::get_rule(int r) {
    return m_rules_factors[r];
}
std::vector<int> Engine::get_states() {
    return m_states;
}
float Engine::get_frequency() {
    return m_frequency;
}

void Engine::draw_states() {
    for(int i = 0; i < ScreenHeight(); i++) {
        for(int j = 0; j < ScreenWidth(); j++) {
            if(get_state(i, j) == 1) {
                Draw(j, i, olc::Pixel(255, 255, 255));
            }
            else {
                Draw(j, i, olc::Pixel(0, 0, 0));
            }
       }
    }
}
void Engine::init_states(float frequency) {
    int nb_cell = ScreenWidth() * ScreenHeight();
    int nb_living_cells = frequency * nb_cell;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, nb_cell);
    for(int i = 0; i < nb_living_cells; i++) {
        int index = dist(rng);
        while(get_state(index)) {
            index = dist(rng);
        }
        set_state(index, CellState::Alive);
    }
}
void Engine::next_iteration() {
    std::vector<int> new_states = get_states();
    for(int i = 0; i < ScreenHeight(); i++) {
        for(int j = 0; j < ScreenWidth(); j++) {
            int nb_living_neighbors = get_living_neighbors_knight(i, j);
            int index = i * ScreenWidth() + j;
            if(nb_living_neighbors == get_rule(0)) {
                new_states[index] = CellState::Alive;
            }
            else if(nb_living_neighbors < get_rule(1) || nb_living_neighbors > get_rule(2)) {
                new_states[index] = CellState::Dead;
            }
        }
    }
    m_states = new_states;
}
int Engine::get_living_neighbors_knight(int i, int j) {
    int nb_living_neighbors = 0;
    std::array<std::pair<int,int>,8> neighbors_coord = {
        // When using the {} notation the compiler gives me an error but not when using make_pair() for
        // the first element, weird.
        std::make_pair(-3, 1), 
        {-1, 3},
        {+1, 3},
        {+3, 1},
        {+3, -1},
        {+1, -3},
        {-1, -3},
        {-3, -1}
    };
    for(auto const& coord : neighbors_coord) {
        int nghb_i = correct_coordinate(i + coord.first, ScreenHeight());
        int nghb_j = correct_coordinate(j + coord.second, ScreenWidth());
        nb_living_neighbors += get_state(nghb_i, nghb_j);
    }
    return nb_living_neighbors;
}
int Engine::get_living_neighbors_regular(int i, int j) {
    int nb_living_neighbors = 0;
    for(int ni = i - 1; ni <= i + 1; ni++) {
        for(int nj = j - 1; nj <= j + 1; nj++) {
            if(ni == i && nj == j) {
                continue; 
            }
            int nghb_i = correct_coordinate(ni, ScreenHeight());
            int nghb_j = correct_coordinate(nj, ScreenWidth());
            nb_living_neighbors += get_state(nghb_i, nghb_j);
        }
    }
    return nb_living_neighbors;
}
void Engine::display_help() {
    FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);
    DrawRect(0, 0, ScreenWidth(), ScreenHeight(), olc::WHITE);
    std::string rule = "Rule: ";
    for(const int& r : m_rules_factors) {
        rule += std::to_string(r);
    }
    int start_i = 2;
    DrawString(2, start_i, rule);
    DrawString(2, start_i + 8, std::to_string(get_frequency()));
    DrawString(2, start_i + 2 * 8,  "Commands:");
    DrawString(2, start_i + 3 * 8,  "    Next rule:                                        N");
    DrawString(2, start_i + 4 * 8,  "    Previous rule:                                    P");
    DrawString(2, start_i + 5 * 8,  "    Clear:                                            Ctrl-L");
    DrawString(2, start_i + 6 * 8,  "    Restart pop:                                      R");
    DrawString(2, start_i + 7 * 8,  "    Increase first generation pop frequency by 0.01:  UP (arrow)");
    DrawString(2, start_i + 8 * 8,  "    Decrease first generation pop frequency by 0.01:  DOWN (arrow)");
    DrawString(2, start_i + 9 * 8,  "    Run:                                              ENTER");
    DrawString(2, start_i + 10 * 8, "    Next iteration:                                   RIGHT (arrow)");
    DrawString(2, start_i + 11 * 8, "    Display help:                                     H");
}

