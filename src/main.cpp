#include "sfml_resources.h"
#include "game_state.h"
#include "sfml_window_manager.h"
#include "sfml_drawing_screen.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <sstream>
#include <SFML/Graphics.hpp>

int show_sfml_drawing_screen(std::string path, std::vector<sf::Vector3f> dat) {
  sfml_drawing_screen ds(dat);
  ds.set_path(path);
  ds.exec();
  return 0;
}

int main(int argc, char **argv) { //!OCLINT
#ifndef NDEBUG
  // test();
#else
  assert(1 == 2);
#endif

//  std::cout << get_time() << std::endl;

  const std::vector<std::string> args(argv, argv + argc);
  
  std::string path = args.at(0);
  
//  std::clog << path << std::endl;
  
  std::vector<sf::Vector3f> pos_data;
  
  if ((!std::count(path.begin(), path.end(), '/')) &&
      (!std::count(path.begin(), path.end(), '\\'))) {
    path = "";
  } else {
    while (path.back() != '/' && path.back() != '\\') {
      path.pop_back();
    }
  }
  
  for (auto &arg : args) {
    if (arg.size() > 4) { //!OCLINT for exception safety non-collapsible
      if (arg.substr(arg.size() - 4) == ".lun") {
        std::ifstream sfile;
        std::string line;
        sfile.open(arg);
        if (sfile.is_open()) {
          std::getline(sfile, line);
//          std::clog << "HEAD: " << line << std::endl;
          assert(line == "x y z");//      <--------------------------------------------------- ASSERT
          while (std::getline(sfile, line)) {
              std::istringstream iss(line);
              float x, y, z;
              if (!(iss >> x >> y >> z)) { break; }
//              std::clog << "LINE: " << x << " " << y << " " << z << std::endl;
              pos_data.push_back(sf::Vector3f(x, y, z));
          }
          sfile.close();
        } else {
//          std::clog << "Couldn't open .lun file" << std::endl;
          return 2;
        }
      }
    }
  }
  if (pos_data.size() < 1) { return 3; }
  sfml_resources::get().load(path);
  
  if (std::count(std::begin(args), std::end(args), "--version")) {
    // Travis: 2.3.2
    // RuG: 2.3.2
    std::cout
      << "SFML version: " << SFML_VERSION_MAJOR
      << "." << SFML_VERSION_MINOR
#if(SFML_VERSION_MINOR > 1)
      << "." << SFML_VERSION_PATCH
#endif
      << std::endl
    ;
    return 0;
  }
  
  if (std::count(std::begin(args), std::end(args), "--ci")) {
    std::clog << "CI argument passed!" << std::endl;
#ifdef CI
    std::clog << "CI defined!" << std::endl;
#endif
  }
  
  while (sfml_window_manager::get().get_window().isOpen()) {
    if (sfml_window_manager::get().get_state() == game_state::drawing) {
      show_sfml_drawing_screen(path, pos_data);
    }
  }
  
  return 0;
}
