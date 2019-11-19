#include "sfml_slider.h"

sfml_slider::sfml_slider(float x, float y, float l, float min, float max, bool ver)
    : m_x{ x }, m_y{ y } ,m_l{ l }, m_min{ min }, m_max{ max }, m_ver{ ver }
{
  
}

void sfml_slider::recreate() {
  // Create line and button shapes
  if (m_ver) {
    line = sf::RectangleShape(sf::Vector2f(5, m_l));
  } else {
    line = sf::RectangleShape(sf::Vector2f(m_l, 5));
  }
  //set position
}