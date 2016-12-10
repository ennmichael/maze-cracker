#ifndef _LOAD_TERRAIN__
#define _LOAD_TERRAIN__

#include "sdl-plus-plus/drawing.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <fstream>

namespace Maze_cracker
{
    using Point_vector = std::vector<Sdl::Point>;
    
    class Invalid_input_file : public std::exception { };
    
    struct Terrain_data {
        Point_vector walls_ { };
        Sdl::Point start_ { };
        Sdl::Point finish_ { };
        std::size_t width_ { };
        std::size_t height_ { };
    };
    
    class Terrain_builder {
    public:
        Terrain_builder();
        
        auto open_file(const std::string&) -> Terrain_builder&;
        auto read_lines() -> Terrain_builder&;
        auto process_lines() -> Terrain_builder&;
        auto finalize() const -> Terrain_data;
    private:
        auto calculate_wh() -> void;
        auto check_wh() const -> void;
        auto process_character(char, Sdl::Point) -> void;
        auto check_sf() const -> void;
        // Check if this indice exists and set it to 0 if not
        
        Terrain_data terrain_data_;
        std::vector<std::string> lines_;
        std::ifstream input_;
    };
}

#endif
