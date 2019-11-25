#ifndef SFML_SLIDER_H
#define SFML_SLIDER_H

#include <SFML/Graphics.hpp>

class sfml_slider
{
public:

  sfml_slider(float x, float y, float l, float min, float max, bool ver);

  sf::RectangleShape line;
  
  sf::RectangleShape button;
  
  void set_val(int nval) { m_val = nval; }
  int get_val() const { return m_val; }
  
  void recreate();

private:

  float m_x;
  
  float m_y;
  
  float m_l;

  float m_min;
  
  float m_max;
  
  bool m_ver;
  
  int m_val;
  
};

float scale(float num, float smin, float smax, float emin, float emax);

#endif // SFML_SLIDER_H
