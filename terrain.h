#ifndef _TERRAIN_H__
#define _TERRAIN_H__

#include "cached.h"
#include "load_terrain.h"
#include "sdl-plus-plus/drawing.h"
#include <vector>
#include <memory>
#include <functional>
#include <iostream>

namespace Maze_cracker 
{
    class Bad_matrice_load;
    class Direction;
    class Path;
    class Bruteforce;
    
    using Direction_vector = std::vector<Direction>;
    using Results = std::vector<Point_vector>;
    
    auto next_point(const Direction&, Sdl::Point) noexcept -> Sdl::Point;
    auto previous_point(const Direction&, Sdl::Point) noexcept -> Sdl::Point;
    
    class Direction {
    public:
        Direction() noexcept;
        
        auto rotate() noexcept -> void;
        auto fully_rotated() const noexcept -> bool;
        auto current_point() const noexcept -> Sdl::Point;
        
    private:
        int rotation_;
    };
    
    class Path {
    public:
        explicit Path(Sdl::Point);
        
        // Manipulation functions
        auto advance() -> void;
        auto digress() noexcept -> void;
        
        // Getters
        auto raw_vector() const-> const Point_vector&;
        auto end_point() const noexcept -> Sdl::Point;
        auto current_rotation_done() const noexcept -> bool;
        auto iterated_everything() const noexcept -> bool;
        
    private:
        auto calculate_raw_vector() const -> Point_vector;
        auto rotate_back_direction() noexcept -> void;
        auto pop_back_direction() noexcept -> void;
        
        Sdl::Point end_point_;
        Direction_vector directions_;
        General_purpose::Cached<Point_vector> raw_vector_cache_;
    };
    
    auto step(Path&, const Terrain_data&) -> void;
    auto bounds_ok(const Path&, const Terrain_data&) noexcept -> bool;
    auto walls_ok(const Path&, const Terrain_data& walls) noexcept -> bool;
    auto self_collision_ok(const Path&) noexcept -> bool;
    
    auto reaches_finish(const Path&, const Terrain_data&) noexcept -> bool;
    auto update_results(
        Results& results, 
        const Path&, 
        const Terrain_data&) -> void;
    auto contains(const Results&, const Path&) noexcept -> bool;
}

#endif
