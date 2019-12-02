#ifndef SFML_SLIDER_H
#define SFML_SLIDER_H

#include <SFML/Graphics.hpp>

class sfml_slider
{
public:

  sfml_slider(float x, float y, float l, float min, float max, bool ver);

  sf::RectangleShape line;
  
  sf::RectangleShape button;
  
  void set_val(float nval) { m_val = nval; }
  float get_val() const { return m_val; }
  
  void recreate();

  float m_x;
  
  float m_y;
  
  float m_l;

  float m_min;
  
  float m_max;
  
private:
  
  float m_val;
  
  bool m_ver;
  
};

float scale(float num, float smin, float smax, float emin, float emax);

#endif // SFML_SLIDER_H
