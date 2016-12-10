#ifndef _VISUALS_H__
#define _VISUALS_H__

#include "sdl-plus-plus/drawing.h"
#include "terrain.h"
#include "load_terrain.h"
#include <map>

namespace Maze_cracker
{
    constexpr auto default_width = 400;
    constexpr auto default_height = 400;
    
    // Actually, we should calculate the screen width/height based on
    // the input size, that is, it's going to become a bit smaller
    // If the default screen size is not divisible with the input size
    
    auto size_from_terrain_data(
        const Terrain_data&,
        Sdl::Point snap, 
        Sdl::Screen_properties&) noexcept -> void;
    auto snap_from_terrain_data(const Terrain_data&) -> Sdl::Point;
    
    auto draw_result(
        Sdl::Screen&, 
        const Results&, 
        Sdl::Point snap, 
        SDL_Color) -> void;
    
    auto draw_point_vector(
        Sdl::Screen&, 
        const Point_vector&, 
        Sdl::Point snap,
        SDL_Color) -> void;
        
    auto draw_point_rect(
        Sdl::Screen&, 
        Sdl::Point draw_point,
        Sdl::Point snap,
        SDL_Color) -> void;
}

#endif
