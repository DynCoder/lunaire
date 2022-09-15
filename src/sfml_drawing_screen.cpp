#include "sfml_drawing_screen.h"
#include "sfml_window_manager.h"
#include "sfml_resources.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <functional>
#include <algorithm>

sfml_drawing_screen::sfml_drawing_screen(std::vector<sf::Vector3f> dat, float ts)
    : m_window{ sfml_window_manager::get().get_window() }, m_data{ dat },
      m_add_image{ sfml_resources::get().get_add_image() },
      m_remove_image{ sfml_resources::get().get_remove_image() },
      m_split_image{ sfml_resources::get().get_split_image() },
      m_meter_h(10, 110, 600, 0, 1000, true), m_step{ 0 }, m_ts{ ts }
{
  m_tool_bar.setFillColor(sf::Color(100, 100, 100));
  m_drawing_area.setFillColor(sf::Color(220, 220, 220));
  
  m_drawing_view = sf::View(sf::Vector2f(m_window.getSize().x / 2,
                                         (m_window.getSize().y / 2) + 50),
                            sf::Vector2f(m_window.getSize().x,
                                         m_window.getSize().y - 100));
  
  // Unusual way of getting the max coordinates
  m_max_h = 0;
  float max_x = 0;
  float max_y = 0;
  for (sf::Vector3f v : m_data) {
    if (v.z > m_max_h) m_max_h = v.z;
    if (v.x > max_x) max_x = v.x;
    if (v.y > max_y) max_y = v.y;
    if (-v.x > max_x) max_x = -v.x;
    if (-v.y > max_y) max_y = -v.y;
  }
  if (max_x > max_y) {
    m_max_c = max_x;
  } else {
    m_max_c = max_y;
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
  update();
  
  m_pt = get_time();
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
    
    {
      std::chrono::steady_clock::time_point t = get_time();
      bool b = std::chrono::duration_cast<std::chrono::milliseconds>(t - m_pt).count() >= m_ts;
      if (m_step < static_cast<int>(m_data.size()) && b) {
        m_pt = t;
        update();
        m_meter_h.recreate();
      }
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
  m_window.draw(m_posind);
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
  sf::Vector2f mapsize = m_posmap.getSize();
  float x = scale(d.x, -m_max_c, m_max_c, 0, mapsize.x);
  float y = scale(d.y, -m_max_c, m_max_c, 0, mapsize.y);
  m_posind.setPosition(200 + x, 100 + y);
  m_meter_h.set_val(m_max_h - d.z);
  m_step++;
}

std::chrono::steady_clock::time_point get_time() {
  return std::chrono::steady_clock::now();
}
