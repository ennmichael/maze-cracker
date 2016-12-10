#include "terrain.h"
#include "visuals.h"
#include "load_terrain.h"
#include "sdl-plus-plus/drawing.h"
#include "sdl-plus-plus/flow.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "cached.h"

// TODO

// TODO fix modularity

class Bad_arguments : public std::exception { };

auto args_okay(int argc, char**) noexcept -> bool
{
    return argc == 2;
}

constexpr auto args_error_message =
    "Wrong arguments supplied.\n"
    "Usage: trn <input_file>\n";

enum class App_state : bool {
    Checking, Displaying_results
};

int main(int argc, char** argv) 
{
    using namespace Sdl;
    using namespace Maze_cracker;
    
    try 
    {
        if (!args_okay(argc, argv))
        {
            throw Bad_arguments { };
        }
        
        std::ifstream input { argv[1] };
        
        if (!input)
        {
            throw Invalid_input_file { };
        }
        
        const auto terrain_data = Terrain_builder { }
                                  .open_file(argv[1])
                                  .read_lines()
                                  .process_lines()
                                  .finalize();
        
        Control _ { };
        
        Screen_properties props { };
        
        Sdl::Point snap = snap_from_terrain_data(terrain_data);
        size_from_terrain_data(terrain_data, snap, props);
        
        props.title = "TRN";
        props.position.x = SDL_WINDOWPOS_UNDEFINED;
        props.position.y = SDL_WINDOWPOS_UNDEFINED;
        
        Screen screen { props };
        Path path { terrain_data.start_ };
        
        Results results { };
        App_state state { App_state::Checking };
        
        Main_loop loop
        {
            [&screen, &terrain_data, &path, &snap, &results, &state]
            {
                if (state == App_state::Checking)
                {
                    if (path.iterated_everything())
                    {
                        state = App_state::Displaying_results;
                    }
                    else
                    {
                        draw_point_vector(
                            screen, path.raw_vector(), snap, 
                            color_blue(255, 100));
                        
                        update_results(results, path, terrain_data);
                        
                        SDL_Delay(10);
                        
                        step(path, terrain_data);
                    }
                }
                else
                {
                    draw_result(
                        screen, results, snap, 
                        color_red(255, 50)
                    );
                }
                
                draw_point_vector(
                        screen, 
                        terrain_data.walls_, 
                        snap,
                        color_black());
                
                screen.redraw(color_white());
            }
        };
        
        loop.start();
    }
    catch (Bad_arguments&)
    {
        std::cout << args_error_message;
    }
    catch (std::bad_alloc&)
    {
        std::cout << "Memory exhausted!\n";
    }
    catch (Sdl_exception& e)
    {
        std::cout << "An SDL error occured!\n";
        std::cout << "Error message: " << e.what() << '\n';
    }
    catch (Invalid_input_file&)
    {
        std::cout << "Input file ill-formatted.\n";
    }
    catch (...)
    {
        std::cout << "An error occured!\n";
    }
}
