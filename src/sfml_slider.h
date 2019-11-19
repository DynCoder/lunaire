#ifndef SFML_SLIDER_H
#define SFML_SLIDER_H

#include <SFML/Graphics.hpp>

class sfml_slider
{
public:

  sfml_slider(float x, float y, float l, float min, float max, bool ver);

  sf::RectangleShape line;
  
  sf::RectangleShape button;
  
private:

  float m_x;
  
  float m_y;
  
  float m_l;

  float m_min;
  
  float m_max;
  
  bool m_ver;
  
  void recreate();
  
};

#endif // SFML_SLIDER_H
