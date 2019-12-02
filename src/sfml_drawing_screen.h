#ifndef SFML_DRAWING_SCREEN_H
#define SFML_DRAWING_SCREEN_H

#include "game_state.h"
#include "sfml_resources.h"
#include "sfml_slider.h"
#include <SFML/Graphics.hpp>
#include <vector>

class sfml_drawing_screen
{
public:

    sfml_drawing_screen(std::vector<sf::Vector3f> dat);

    void exec();

    void process_event(sf::Event event);

    ///@param game state to change to
    void close(game_state s);

    ///Close the game
    void close();

    void set_positions();
    void set_sizes();
    
    void draw_objects();
    
    void set_path(std::string p) { m_path = p; }

private:
    
    sf::RenderWindow& m_window;

    sf::RectangleShape m_tool_bar;
    sf::RectangleShape m_drawing_area;
    
    sf::View m_drawing_view;
    
    std::vector<sf::Vector3f> m_data;
    
    bool hover(float x, float y, float range);

    std::string m_path;
    
    sf::Texture& m_add_image;
  
    sf::Texture& m_remove_image;
  
    sf::Texture& m_split_image;
    
    sfml_slider m_meter_h;
    
    float m_max_h;
    
};

std::string get_time();

#endif // SFML_DRAWING_SCREEN_H
