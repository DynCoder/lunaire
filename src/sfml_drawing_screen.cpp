#include "sfml_drawing_screen.h"
#include "sfml_window_manager.h"
#include "sfml_resources.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <functional>
#include <algorithm>
#include <chrono>

/* IDEAS
 * /////
 * Include time scale at the top of .lun file
 */

sfml_drawing_screen::sfml_drawing_screen(std::vector<sf::Vector3f> dat)
    : m_window{ sfml_window_manager::get().get_window() }, m_data{ dat },
      m_add_image{ sfml_resources::get().get_add_image() },
      m_remove_image{ sfml_resources::get().get_remove_image() },
      m_split_image{ sfml_resources::get().get_split_image() },
      m_meter_h(10, 110, 600, 0, 1000, true), m_step{ 1 }
{
  m_tool_bar.setFillColor(sf::Color(100, 100, 100));
  m_drawing_area.setFillColor(sf::Color(220, 220, 220));
  
  m_drawing_view = sf::View(sf::Vector2f(m_window.getSize().x / 2,
                                         (m_window.getSize().y / 2) + 50),
                            sf::Vector2f(m_window.getSize().x,
                                         m_window.getSize().y - 100));
  
  m_max_h = 0;
  m_max_x = 0;
  m_max_y = 0;
  for (sf::Vector3f v : m_data) {
    if (v.z > m_max_h) m_max_h = v.z;
    if (std::abs(v.x) > m_max_x) m_max_x = v.x;
    if (std::abs(v.y) > m_max_y) m_max_y = v.y;
  }
  
  m_posmap = sf::RectangleShape(sf::Vector2f(sf::Vector2f(m_window.getSize()).y - 120,
                                             sf::Vector2f(m_window.getSize()).y - 120));
  m_posmap.setPosition(210, 110);
  m_posmap.setFillColor(sf::Color(190, 190, 190));
  m_posind = sf::RectangleShape(sf::Vector2f(20, 20));
  m_posind.setPosition(210, 110);
  m_posind.setFillColor(sf::Color(230, 50, 50));
  
  m_meter_h.m_l = sf::Vector2f(m_window.getSize()).y - 120;
  m_meter_h.m_max = m_max_h;
  m_meter_h.set_val(m_max_h - m_data.at(0).z);
  
  m_meter_h.recreate();
}

void sfml_drawing_screen::exec() { //!OCLINT can be complex
  set_sizes();
  while (active(game_state::drawing)) {
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      process_event(event);
    }
    if (!m_window.isOpen()) return;
    
    set_positions();
    draw_objects();
    if (m_step < static_cast<int>(m_data.size())) {
      update();
    }
#ifdef CI
    close();
#endif
  }
}

void sfml_drawing_screen::process_event(sf::Event event) { //!OCLINT can be complex
  int u = 0;
  switch (event.type) {
    case sf::Event::Closed:
        close();
        break;
/*
    case sf::Event::Resized:
      {
        sf::View view = m_window.getDefaultView();
        sfml_window_manager::get().update();
        view.setSize(static_cast<float>(m_window.getSize().x),
                     static_cast<float>(m_window.getSize().y));
        m_window.setView(view);
        set_sizes();
      }
      break;
*/
    case sf::Event::KeyPressed:
      switch (event.key.code)
      {
        case sf::Keyboard::Escape:
          close();
          break;

        case sf::Keyboard::Left:
        case sf::Keyboard::A:
          // Do something
          break;

        case sf::Keyboard::Right:
        case sf::Keyboard::D:
          // Do something
          break;

        case sf::Keyboard::Up:
        case sf::Keyboard::W:
          // Do something
          break;

        case sf::Keyboard::Down:
        case sf::Keyboard::S:
          // Do something
          break;

        default:
          break;
      }
      break;

    case sf::Event::KeyReleased:
      switch (event.key.code)
        {
          case sf::Keyboard::Left:
          case sf::Keyboard::A:
            // Do something
            break;

          case sf::Keyboard::Right:
          case sf::Keyboard::D:
            // Do something
            break;

          case sf::Keyboard::Up:
          case sf::Keyboard::W:
            // Do something
            break;

          case sf::Keyboard::Down:
          case sf::Keyboard::S:
            // Do something
            break;

          default:
            break;
        }
      break;

    case sf::Event::MouseButtonPressed:
      // Do something
      break;
      
    case sf::Event::MouseButtonReleased:
      // Do something
      break;

    default:
      sfml_window_manager::get().process();
      break;
  }
}

void sfml_drawing_screen::set_positions() {
  m_tool_bar.setPosition(m_window.mapPixelToCoords(sf::Vector2i(0, 0)));
  m_drawing_area.setPosition(m_window.mapPixelToCoords(sf::Vector2i(0, 100)));
}

void sfml_drawing_screen::set_sizes() {
  m_tool_bar.setSize(sf::Vector2f(m_window.getSize().x, 100));
  m_drawing_area.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y - 100));
  
  float tb_per = 100.0/m_window.getSize().y;
  m_drawing_view.setViewport(sf::FloatRect(0, tb_per, 1, 1 - tb_per));
  sf::Vector2f size(m_window.getSize());
  //m_drawing_view.setCenter(size.x / 2, size.y / 2);
}

void sfml_drawing_screen::draw_objects() {
  // Clear window
  m_window.clear();

  // Draw tool bar
  m_window.draw(m_tool_bar);

  // Draw tree viewing port
  m_window.draw(m_drawing_area);

  sf::View o_view = m_window.getView();
  m_window.setView(m_drawing_view);

  // Draw objects //
  
  m_window.draw(m_posmap);
  m_window.draw(m_meter_h.line);
  m_window.draw(m_meter_h.button);
  
  //////////////////

  m_window.setView(o_view);

  
  // Display all to window
  m_window.display();
}

void sfml_drawing_screen::close(game_state s) {
  sfml_window_manager::get().set_state(s);
}

void sfml_drawing_screen::close() {
  m_window.close();
}

bool sfml_drawing_screen::hover(float x, float y, float range) {
  sf::Vector2f mp = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window), m_drawing_view);
  return mp.x > x - range && mp.x < x + range &&
         mp.y > y - range && mp.y < y + range;
}

void sfml_drawing_screen::update() {
  m_meter_h.recreate();
  sf::Vector3f d = m_data.at(m_step);
  
  m_step++;
}

std::string get_time() {
  std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                                            std::chrono::system_clock::now().time_since_epoch());
  std::string ts = std::to_string(ms.count());
  return ts;
}
