#include "graphic.h"
#include "Constants.h"
#include "Game.h"
#include "tools.h"

Graphic::Graphic ():
    m_Main_Box(Gtk::Orientation::HORIZONTAL, 0),
    m_Panel_Box(Gtk::Orientation::VERTICAL, 2),
    m_Buttons_Box(Gtk::Orientation::VERTICAL, 2),
    m_Panel_Frame("Option"),// a verifier
    m_Button_Exit("Exit"),
    m_Button_Open("Open"),
    m_Button_Save("Save"),
    m_Button_Restart("Restart"),
    m_Button_Start("Start"),
    m_Button_Step("Step")
    
{
    // init layout
    set_title("Brick Breaker");
    set_child(m_Main_Box);

    m_Main_Box.append(m_Panel_Box);
    m_Main_Box.append(m_Area);

    m_Panel_Box.append(m_Panel_Frame);
    m_Panel_Frame.set_child(m_Buttons_Box);
    
    m_Buttons_Box.append(m_Button_Exit);
    m_Buttons_Box.append(m_Button_Open);
    m_Buttons_Box.append(m_Button_Save);
    m_Buttons_Box.append(m_Button_Restart);
    m_Buttons_Box.append(m_Button_Start);
    m_Buttons_Box.append(m_Button_Step);
    
    m_Button_Exit.signal_clicked().connect(
        sigc::mem_fun(*this, &Graphic::on_button_clicked_exit));
    m_Button_Open.signal_clicked().connect(
        sigc::mem_fun(*this, &Graphic::on_button_clicked_open));
    m_Button_Save.signal_clicked().connect(
        sigc::mem_fun(*this, &Graphic::on_button_clicked_save));
    m_Button_Restart.signal_clicked().connect(
        sigc::mem_fun(*this, &Graphic::on_button_clicked_restart));
    m_Button_Start.signal_clicked().connect(
        sigc::mem_fun(*this, &Graphic::on_button_clicked_start));
    m_Button_Step.signal_clicked().connect(
        sigc::mem_fun(*this, &Graphic::on_button_clicked_step));
    
    // init drawing sub-window parameters
    m_Area.set_content_width(arena_size);
    m_Area.set_content_height(arena_size);
    m_Area.set_expand(true);
    m_Area.set_draw_func(sigc::mem_fun(*this, &Graphic::on_draw));
}


// a modifier
void Graphic::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    if (!m_game) return;

    draw_arena(cr);

    cr->set_source_rgb(0.1, 0.1, 0.8); 
    draw_circle(cr, m_game->get_paddle().getCircle());

    cr->set_source_rgb(0.8, 0.1, 0.1); 
    for (const auto& brick : m_game->get_bricks()) {
        draw_square(cr, brick->getSquare());
    }

    cr->set_source_rgb(0.1, 0.8, 0.1);
    for (const auto& ball : m_game->get_balls()) {
        draw_circle(cr, ball->getCircle());
    }
}
// à compléter plus tard
void Graphic::on_button_clicked_exit()    { hide(); }
void Graphic::on_button_clicked_open()    { /* TODO */ }
void Graphic::on_button_clicked_save()    { /* TODO */ }
void Graphic::on_button_clicked_restart() { m_game->reset(); m_Area.queue_draw(); }
void Graphic::on_button_clicked_start()   { /* TODO : timer */ }
void Graphic::on_button_clicked_step()    { /* TODO : un pas */ }