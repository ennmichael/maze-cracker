#include "terrain.h"
#include <exception>
#include <algorithm>
#include <cassert>
#include <cstddef>

namespace Maze_cracker
{
    auto next_point(
        const Direction& direction,
        Sdl::Point point) noexcept -> Sdl::Point
    {
        return direction.current_point() + point;
    }
    
    auto previous_point(
        const Direction& direction, 
        Sdl::Point point) noexcept -> Sdl::Point
    {
        return point - direction.current_point();
    }
    
    Direction::Direction() noexcept
        : rotation_ { 0 }
    { }
    
    auto Direction::rotate() noexcept -> void
    {
        ++rotation_;
    }
    
    auto Direction::fully_rotated() const noexcept -> bool
    {
        return rotation_ == 4;
    }
    
    auto Direction::current_point() const noexcept -> Sdl::Point
    {
        switch (rotation_)
        {
            case 1:
                return { -1, 0 };
            case 2:
                return { 1, 0 };
            case 3:
                return { 0, 1 };
            case 4:
                return { 0, -1 };
            default:
                std::terminate();
                return { };
        }
    }
    
    Path::Path(Sdl::Point start)
        : end_point_ { start }
        , directions_ { { } }
        , raw_vector_cache_
          {
              [this]()
              {
                  return calculate_raw_vector();
              }
          }
    {
    }
    
    auto Path::advance() -> void
    {
        raw_vector_cache_.notify_change();
        
        rotate_back_direction();
        end_point_ = next_point(directions_.back(), end_point_);
        directions_.push_back({});
    }
    
    auto Path::digress() noexcept -> void
    {
        raw_vector_cache_.notify_change();
        
        pop_back_direction();
        end_point_ = previous_point(directions_.back(), end_point_);
    }
    
    auto Path::raw_vector() const -> const Point_vector&
    {
        return raw_vector_cache_.get();
    }
    
    auto Path::end_point() const noexcept -> Sdl::Point
    {
        return end_point_;
    }
    
    auto Path::current_rotation_done() const noexcept -> bool
    {
        return directions_.back().fully_rotated();
    }
    
    auto Path::iterated_everything() const noexcept -> bool
    {
        return directions_.size() == 1 &&
               current_rotation_done();
    }
    
    auto Path::calculate_raw_vector() const -> Point_vector
    {
        auto pt = end_point_;
        Point_vector result { pt };
        
        std::for_each( // Iterate in reverse order
            directions_.crbegin() + 1, directions_.crend(), 
            [this, &result, &pt](const auto& direction)
            {
                pt = previous_point(direction, pt);
                
                result.insert( // Insert a new element at the beginning
                    result.cbegin(),
                    pt);
            });
        
        return result;
    }
    
    auto Path::rotate_back_direction() noexcept -> void
    {
        directions_.back().rotate();
    }
    
    auto Path::pop_back_direction() noexcept -> void
    {
        directions_.pop_back();
    }
    
    auto step(Path& path, const Terrain_data& td) -> void
    {
        if (path.current_rotation_done())
        { // First check if we can rotate
            path.digress();
            return;
        }
        
        path.advance();
        
        if (!walls_ok(path, td) || 
            !bounds_ok(path, td) ||
            !self_collision_ok(path))
        {
            path.digress();
            return;
        }
    }
    
    auto bounds_ok(
        const Path& path, 
        const Terrain_data& td) noexcept -> bool
    {
        auto end = path.end_point();
        
        if (end.x < 0 || end.y < 0)
        {
            return false;
        }
        
        // Static casting to an unsigned type is okay here,
        // because the previous check makes sure both x and y
        // are greater than 0
        if (static_cast<std::size_t>(end.x) >= td.width_ || 
            static_cast<std::size_t>(end.y) >= td.height_)
        {
            return false;
        }
        
        return true;
    }
    
    auto walls_ok(
        const Path& path, 
        const Terrain_data& td) noexcept -> bool
    {
        auto found = std::find(
            td.walls_.cbegin(), td.walls_.cend(), 
            path.end_point());
        
        return found == td.walls_.cend();
    }
    
    auto self_collision_ok(const Path& path) noexcept -> bool
    {
        const auto& raw_vector = path.raw_vector();
        
        if (raw_vector.size() == 1)
            return true;
        
        auto found = std::find(
            raw_vector.cbegin(), raw_vector.cend() - 1, 
            raw_vector.back());
        
        return found == raw_vector.cend() - 1;
    }
    
    auto update_results(
        Results& results, 
        const Path& path, 
        const Terrain_data& td) -> void
    {
        // TODO this code is quite ugly
        if (contains(results, path))
            return;
        
        if (!reaches_finish(path, td))
            return;
        
        if (results.empty())
        {
            results.push_back(path.raw_vector());
            return;
        }
        
        if (path.raw_vector().size() <= results.back().size())
        {
            if (path.raw_vector().size() < results.back().size())
            {
                results.clear();
            }
            
            results.push_back(path.raw_vector());
        }
    }
    
    auto reaches_finish(const Path& path, const Terrain_data& td) noexcept -> bool
    {
        return path.end_point() == td.finish_;
    }
    
    auto contains(const Results& results, const Path& path) noexcept -> bool
    {
        return std::any_of(
            results.cbegin(), results.cend(), 
            [&path](const auto& e)
            {
                return path.raw_vector() == e;
            });
    }
}
