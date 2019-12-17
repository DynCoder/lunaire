Om de data die de raket verzamelt te visualiseren heb ik een nieuw programma gemaakt genaamd Lunaire. Deze naam had ik al op mijn lijstje met software namen staan en ik vond het wel toepasselijk voor dit programma aangezien het programma bedoeld is om de baan van een waterraket te laten zien en raketten omhoog gaan richting de maan. De code bevat ongeveer 19 duizend characters verdeeld over 772 regels. Het programma neemt input van een .lun bestand met behulp van de volgende code:
```cpp
  ...
  float ts;
  for (auto &arg : args) {
    if (arg.size() > 4) { //!OCLINT for exception safety non-collapsible
      if (arg.substr(arg.size() - 4) == ".lun") {
        std::ifstream sfile;
        std::string line;
        sfile.open(arg);
        if (sfile.is_open()) {
          {
            std::getline(sfile, line);
            std::istringstream iss(line);
            if (!(iss >> ts)) { break; }
          }
          std::getline(sfile, line);
//          std::clog << "HEAD: " << line << std::endl;
//          assert(line == "x y z");//      <--------------------------------------------------- ASSERT
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
  ...
```
Dit stuk code bekijkt de argumenten die het programma heeft gekregen bij het starten. Als er een .lun bestand wordt geopend met Lunaire zal de locatie van het geopende bestand meegegeven worden als argument. Als er een argument op .lun eindigt dan wordt er aangenomen dat dit het bestand is dat geopend moet worden en wordt het bestand lijn voor lijn uitgelezen. De eerste lijn wordt opgeslagen als de tijdstap. Met de tweede lijn wordt niks gedaan en de rest wordt een voor een opgeslagen in een lijst met 3D vectoren.

Een .lun bestand is een door mijzelf bedacht bestandsformaat gebasseerd op csv bedoeld om overzichtelijk data in Lunaire te visualiseren. Het .lun bestand bestaat uit drie onderdelen: De tijdstap, de header en de data. De tijdstap staat bovenaan in het bestand en bepaalt hoeveel milliseconden er tussen twee lijnen data zitten. De tijdstap is constant, waardoor er even veel tijd tussen alle stappen zit. Op de lijn onder de tijdstap staat de header, dit is puur bedoelt om het bestand overzichtelijk te maken en het maakt niet uit wat hier staat. Onder de header staat alle data; elke stap staat op een nieuwe regel. Een lijn data bestaat uit drie waarden: X, Y, en Z. X en Y laten zien welke kant de raket op gaat en Z is de hoogte van de raket. In het programma worden al deze lijnen een voor een bij langs gegaan met deze code:
```cpp
    ...
    {
      std::chrono::steady_clock::time_point t = get_time();
      bool b = std::chrono::duration_cast<std::chrono::milliseconds>(t - m_pt).count() >= m_ts;
      if (m_step < static_cast<int>(m_data.size()) && b) {
        m_pt = t;
        update();
        m_meter_h.recreate();
      }
    }
    ...
```
```cpp
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
```
De data wordt dan als volgt gevisualiseerd in Lunaire: \
![screenshot](https://github.com/Joshua260403/lunaire/blob/master/doc/image.png) \
Aan de linker kant van het programma staat de hoogte meter en het vierkant is een bovenaanzicht van welke kant de raket op gaat. De hoogtemeter en het richtingsvierkant zijn in de huidige versie (v1.0) niet op dezelfde schaal, ook staan er nog geen labels bij die laten zien hoe hoog of ver de raket nou gaat.
