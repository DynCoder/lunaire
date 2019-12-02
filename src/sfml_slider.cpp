#include "sfml_slider.h"

sfml_slider::sfml_slider(float x, float y, float l, float min, float max, bool ver)
    : m_x{ x }, m_y{ y } ,m_l{ l }, m_min{ min }, m_max{ max }, m_val{ 0 }, m_ver{ ver }
{
  recreate();
}

void sfml_slider::recreate() {
//  sf::RenderWindow window& = sfml_window_manager::get().get_window();
  // Create line and button shapes
  button = sf::RectangleShape(sf::Vector2f(20, 20));
  if (m_ver) {
    line = sf::RectangleShape(sf::Vector2f(5, m_l));
    button.setPosition(m_x - 7.5, m_y + scale(m_val, m_min, m_max, 0, m_l) - 7.5);
  } else {
    line = sf::RectangleShape(sf::Vector2f(m_l, 5));
    button.setPosition(m_x + scale(m_val, m_min, m_max, 0, m_l) - 7.5, m_y - 7.5);
  }
  // set position
  line.setPosition(m_x, m_y);
#ifndef CI
  // set colors
  button.setFillColor(sf::Color(230, 50, 50));
  line.setFillColor(sf::Color(180, 180, 180));
#endif
}

float scale(float num, float smin, float smax, float emin, float emax) {
  double percent = (num - smin)/(smax - smin);
  return ((emax - emin) * percent) + emin;
}