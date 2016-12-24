#include "visuals.h"

namespace Maze_cracker
{
    auto size_from_terrain_data(
        const Terrain_data& td,
        Sdl::Point snap, 
        Sdl::Screen_properties& props) noexcept -> void
    {
        props.width = td.width_ * snap.x;
        props.height = td.height_ * snap.y;
    }
    
    auto snap_from_terrain_data(const Terrain_data& td) -> Sdl::Point
    {
        return { 
            static_cast<int>(default_width / td.width_), 
            static_cast<int>(default_height / td.height_) };
    }
    
    auto draw_result(
        Sdl::Screen& screen, 
        const Results& results, 
        Sdl::Point snap, 
        SDL_Color color) -> void
    {
        for (const auto& points : results)
        {
            draw_point_vector(screen, points, snap, color);
        }
    }
    
    auto draw_point_vector(
        Sdl::Screen& screen, 
        const Point_vector& points, 
        Sdl::Point snap,
        SDL_Color color) -> void
    {
        for (const auto& point : points)
        {
            draw_point_rect(screen, point, snap, color);
        }
    }
    
    auto draw_point_rect(
        Sdl::Screen& screen,
        Sdl::Point draw_point,
        Sdl::Point snap,
        SDL_Color color,
        Sdl::Color_filling filling) -> void
    {
        screen.add_draw(
            Sdl::make_rect(
                draw_point * snap, 
                snap.x, snap.y),
            color, filling);
    }
}
