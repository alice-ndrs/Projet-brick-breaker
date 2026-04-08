#include "Game.h"
#include "graphic.h"
#include <gtkmm/application.h>

int main(int argc, char* argv[]) 
{
    Game game;

    if (argc < 2) return 1;

    std::string filename = argv[1];

    if (!game.getLevel(filename)) {
        return 1;
    }

    auto app = Gtk::Application::create("org.brickbreaker");
    Graphic gui;
    
    gui.set_game(&game); 
    app->signal_activate().connect([&]() {
        app->add_window(gui);
        gui.show(); 
    });

    return app->run(0, nullptr);
};