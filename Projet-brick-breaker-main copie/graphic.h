#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "tools.h"
#include "Game.h"
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/drawingarea.h>

class Graphic : public Gtk::Window
{
    public:
        Graphic ();
        virtual ~Graphic ()= default;
        void set_game(Game* game) { m_game = game; }

    protected:

        void on_button_clicked_exit();
        void on_button_clicked_open();
        void on_button_clicked_save();
        void on_button_clicked_restart();
        void on_button_clicked_start();
        void on_button_clicked_step();

    private: 
        void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, 
            int width, int height);
        Game* m_game = nullptr;
            
        Gtk::Box m_Main_Box;
        Gtk::Box m_Panel_Box;
        Gtk::Box m_Buttons_Box;
        Gtk::Frame m_Panel_Frame;
        Gtk::DrawingArea m_Area;

        Gtk::Button m_Button_Exit;
        Gtk::Button m_Button_Open;
        Gtk::Button m_Button_Save;
        Gtk::Button m_Button_Restart;
        Gtk::Button m_Button_Start;
        Gtk::Button m_Button_Step;
};

#endif