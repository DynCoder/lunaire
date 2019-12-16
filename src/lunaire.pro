HEADERS += game_state.h \
           sfml_drawing_screen.h \
           sfml_resources.h \
           sfml_slider.h \
           sfml_window_manager.h
SOURCES += game_state.cpp \
           main.cpp \
           sfml_drawing_screen.cpp \
           sfml_resources.cpp \
           sfml_slider.cpp \
           sfml_window_manager.cpp

# SFML
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
