#include <gtkmm/application.h>
#include <string>
#include "gui.h"

int main(int argc, char* argv[]) 
{
    //Game game;

    //if (argc < 2) return 1;

    //std::string filename = argv[1];

    //if (!game.getLevel(filename)) {
        //return 1;
    //}

    std::string file_name("");
    if (argc > 1)
    {
        file_name = argv[1];
    }
    auto app = Gtk::Application::create();
    return app->make_window_and_run<My_window>(1, argv, file_name);
};