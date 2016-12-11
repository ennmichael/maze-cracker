#include "load_terrain.h"
#include <algorithm>

namespace Maze_cracker
{
    Terrain_builder::Terrain_builder()
        : terrain_data_ { }
        , lines_ { }
        , input_ { }
    { }
    
    auto Terrain_builder::open_file(
        const std::string& file_name) -> Terrain_builder&
    {
        input_.open(file_name);
        return *this;
    }
    
    auto Terrain_builder::read_lines() -> Terrain_builder&
    {
        std::string line { };
        
        while (std::getline(input_, line))
        {
            lines_.push_back(line);
        }
        
        calculate_wh();
        return *this;
    }
    
    auto Terrain_builder::process_lines() -> Terrain_builder&
    {
        Sdl::Point point { };
        
        for (const auto& line : lines_)
        {
            for (const auto& c : line)
            {
                process_character(c, point);
                ++point.x;
            }
            
            ++point.y;
            point.x = 0;
        }
        
        return *this;
    }
    
    auto Terrain_builder::finalize() const -> Terrain_data
    {
        return terrain_data_;
    }
    
    auto Terrain_builder::calculate_wh() -> void
    {
        terrain_data_.height_ = lines_.size();
        terrain_data_.width_ = lines_.front().size();
        
        check_wh();
    }
    
    auto Terrain_builder::check_wh() const -> void
    {
        auto different = std::find_if(
            lines_.cbegin(), lines_.cend(), 
            [w = terrain_data_.width_](const auto& str)
            {
                if (str.length() != w)
                    return true;
                return false;
            });
        
        if (different != lines_.cend())
            throw Invalid_input_file { };
    }
    
    auto Terrain_builder::process_character(
        char c, Sdl::Point point) -> void
    {
        switch (c)
        {
            case 'o':
                return;
            case 'x':
                terrain_data_.walls_.push_back(point);
                return;
            case 's':
                terrain_data_.start_ = point;
                return;
            case 'f':
                terrain_data_.finish_ = point;
                return;
            default:
                throw Invalid_input_file { };
        }
    }
}
