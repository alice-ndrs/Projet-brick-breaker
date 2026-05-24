#ifndef GUI_H
#define GUI_H

#include "Game.h"
#include <array>
#include <filesystem>
#include <gtkmm.h>
#include <string>

class My_window : public Gtk::Window {
  public:
    using Gtk::Window::Window;
    My_window(std::string file_name);

  private:
    // --- Données du jeu ---
    double init_x = 0;
    Game m_game;
    std::string last_file;
    bool level_loaded = false;

    // --- Widgets ---
    Gtk::Box main_box, panel_box, command_box;
    Gtk::Grid info_grid;
    bool loop_activated;
    sigc::connection loop_conn;
    std::array<Gtk::Button, 6> buttons;
    Gtk::Frame info_frame;
    std::array<Gtk::Label, 4> info_text, info_value;
    Gtk::DrawingArea drawing;

    void set_commands();

    void exit_clicked();
    void open_clicked();
    void save_clicked();
    void restart_clicked();
    void start_clicked();
    void step_clicked();

    // --- Gestion du clavier ---
    void set_key_controller();
    bool key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);

    // --- Gestion des fichiers ---
    void set_dialog(Gtk::FileChooserDialog *dialog);
    void dialog_response(int response, Gtk::FileChooserDialog *dialog);
    void open_file(const std::filesystem::path &file_name);
    void save_file(const std::filesystem::path &file_name);

    bool loop();

    void set_infos();
    void update_infos();
    void update_buttons();

    // --- Méthodes de dessin ---
    void set_drawing();
    void on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);
    void draw_paddle(const Cairo::RefPtr<Cairo::Context> &cr) const;
    void draw_bricks(const Cairo::RefPtr<Cairo::Context> &cr) const;
    void draw_balls(const Cairo::RefPtr<Cairo::Context> &cr) const;

    // --- Méthode de la souris ---
    void set_mouse_controller();
    void on_drawing_left_click(int n_press, double x, double y);
    void on_drawing_move(double x, double y);
};

#endif