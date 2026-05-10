#include <filesystem>
#include <iostream>
#include "Constants.h"
#include "graphic_gui.h"
#include "Brick.h"
#include "gui.h"

using namespace std;

enum Response
{
    CANCEL,
    OPEN_FILE,
    SAVE_FILE
};


enum Buttons
{
    EXIT,
    OPEN,
    SAVE,
    RESTART,
    START,
    STEP
};


constexpr unsigned drawing_size(500);

My_window::My_window(string file_name)
    : m_game(), last_file(file_name), main_box(Gtk::Orientation::HORIZONTAL), panel_box(Gtk::Orientation::VERTICAL),
      command_box(Gtk::Orientation::VERTICAL), loop_activated(false),
      buttons({Gtk::Button("exit"), Gtk::Button("open"), Gtk::Button("save"),
               Gtk::Button("restart"), Gtk::Button("start"), Gtk::Button("step")}),
      info_frame("Infos :"),
      info_text({Gtk::Label("score:"), Gtk::Label("lives:"),
                                        Gtk::Label("bricks:"), Gtk::Label("balls:")})
      
{
    set_title("Brick Breaker");
    set_child(main_box);
    main_box.append(panel_box);
    main_box.append(drawing);
    panel_box.append(command_box);
    panel_box.append(info_frame);

    set_commands();
    set_key_controller();
    set_mouse_controller();
    set_infos();
    set_drawing();

    update_infos();

    buttons[SAVE].set_sensitive(false);
    buttons[START].set_sensitive(false);
    buttons[STEP].set_sensitive(false);
    buttons[RESTART].set_sensitive(false);

    if (!file_name.empty())
    {
        if (m_game.getLevel(file_name))
        {
            level_loaded = true;
            init_x = m_game.get_paddle().getX();

            buttons[SAVE].set_sensitive(true);
            buttons[START].set_sensitive(true);
            buttons[STEP].set_sensitive(true);
            buttons[RESTART].set_sensitive(true);

            drawing.queue_draw();
        }
        else
        {
            m_game.reset();
            level_loaded = false;

            buttons[SAVE].set_sensitive(false);
            buttons[START].set_sensitive(false);
            buttons[STEP].set_sensitive(false);


            drawing.queue_draw();
        }
    }
}


void My_window::set_commands()
{
    for (auto &button : buttons)
    {
        command_box.append(button);
        command_box.set_size_request(120, -1);
        button.set_margin(1);
    }

    buttons[EXIT].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::exit_clicked));
    buttons[OPEN].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::open_clicked));
    buttons[SAVE].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::save_clicked));
    buttons[RESTART].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::restart_clicked));
    buttons[START].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::start_clicked));
    buttons[STEP].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::step_clicked));
}


void My_window::exit_clicked()
{
    hide();
}


void My_window::open_clicked()
{
    auto dialog = new Gtk::FileChooserDialog("Choose a text file",
                                             Gtk::FileChooserDialog::Action::OPEN);
    set_dialog(dialog);
}


void My_window::save_clicked()
{
    auto dialog = new Gtk::FileChooserDialog("Choose a text file",
                                             Gtk::FileChooserDialog::Action::SAVE);
    set_dialog(dialog);
}


void My_window::restart_clicked()
{
    loop_conn.disconnect();
    loop_activated = false;
    buttons[START].set_label("start");
    
    if (!last_file.empty())
    {
        if (m_game.getLevel(last_file))
        {
            level_loaded = true;
            init_x = m_game.get_paddle().getX();

            buttons[SAVE].set_sensitive(true);
            buttons[START].set_label("start");
            buttons[START].set_sensitive(true);
            buttons[STEP].set_sensitive(true);
        }
        else
        {
            m_game.reset();
            level_loaded = false;

            buttons[SAVE].set_sensitive(false);
            buttons[START].set_sensitive(false);
            buttons[STEP].set_sensitive(false);
        }
        update_infos();
        drawing.queue_draw();
    }
}


void My_window::start_clicked()
{
    if (loop_activated)
    {
        loop_conn.disconnect();
        loop_activated = false;
        buttons[EXIT].set_sensitive(true);
        buttons[OPEN].set_sensitive(true);
        buttons[SAVE].set_sensitive(true);
        buttons[RESTART].set_sensitive(true);
        buttons[START].set_label("start");
        buttons[STEP].set_sensitive(true);
    }
    else // TODO: only if the game is not finished
    {
        loop_conn =
            Glib::signal_timeout().connect(sigc::mem_fun(*this, &My_window::loop), dt);
        loop_activated = true;
        buttons[EXIT].set_sensitive(false);
        buttons[OPEN].set_sensitive(false);
        buttons[SAVE].set_sensitive(false);
        buttons[RESTART].set_sensitive(false);
        buttons[START].set_label("stop");
        buttons[STEP].set_sensitive(false);
    }
}


void My_window::step_clicked()
{
    if (!level_loaded) return;

    m_game.update_paddle(init_x);
    m_game.update();
    update_infos(); // On met à jour l'affichage
    drawing.queue_draw();
}


void My_window::set_key_controller()
{
    auto contr = Gtk::EventControllerKey::create();
    contr->signal_key_pressed().connect(sigc::mem_fun(*this, &My_window::key_pressed),
                                        false);
    add_controller(contr);
}


bool My_window::key_pressed(guint keyval, guint keycode, Gdk::ModifierType state)
{
    if (!level_loaded && (keyval == 's' || keyval == '1')) {
        return true;
    }
    
    if (loop_activated && keyval != 's') return true;

    switch (keyval)
    {
    case '1':
        step_clicked();
        return true;
    case 's':
        start_clicked();
        return true;
    case 'r':
        restart_clicked();
        return true;
    default:
        break;
    }
    return false;
}


void My_window::set_dialog(Gtk::FileChooserDialog *dialog)
{
    dialog->set_modal(true);
    dialog->set_transient_for(*this);
    dialog->set_select_multiple(false);
    dialog->signal_response().connect(
        sigc::bind(sigc::mem_fun(*this, &My_window::dialog_response), dialog));

    dialog->add_button("_Cancel", CANCEL);
    switch (dialog->get_action())
    {
    case Gtk::FileChooserDialog::Action::OPEN:
        dialog->add_button("_Open", OPEN_FILE);
        break;
    case Gtk::FileChooserDialog::Action::SAVE:
        dialog->add_button("_Save", SAVE_FILE);
        break;
    default:
        break;
    }

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_pattern("*.txt");
    dialog->add_filter(filter_text);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    dialog->show();
}


void My_window::dialog_response(int response, Gtk::FileChooserDialog *dialog)
{
    filesystem::path file_name = "";

    if (dialog->get_file()) {
        file_name = dialog->get_file()->get_path();
        if (file_name.extension() != ".txt") {
            file_name = "";
        }
    }

    switch (response)
    {
    case CANCEL:
        break;

    case OPEN_FILE:
        if (file_name != "") {
            open_file(file_name);
        }
        break;

    case SAVE_FILE:
        if (file_name != "") {
            save_file(file_name);
        }
        break;
        
    default:
        break;
    }

    dialog->hide();
}

void My_window::open_file(const std::filesystem::path& file_name) 
{   
    last_file = file_name.string();

    loop_conn.disconnect();
    loop_activated = false;
    buttons[START].set_label("start");

    if (m_game.getLevel(file_name.string()))
    {
        level_loaded = true;
        init_x = m_game.get_paddle().getX();

        buttons[SAVE].set_sensitive(true);
        buttons[START].set_sensitive(true);
        buttons[STEP].set_sensitive(true);
    }
    else
    {
        m_game.reset();
        level_loaded = false;

        buttons[SAVE].set_sensitive(false);
        buttons[START].set_sensitive(false);
        buttons[STEP].set_sensitive(false);
        buttons[RESTART].set_sensitive(true);
    }

    update_infos();
    drawing.queue_draw();
}

void My_window::save_file(const std::filesystem::path& file_name) 
{
    cout << "save file " << file_name << endl; 
    m_game.saveLevel(file_name.string());
}

bool My_window::loop()
{
    if (loop_activated)
    {
        step_clicked();

        if (m_game.get_status() != Status::ONGOING)
        {
            loop_conn.disconnect();
            loop_activated = false;
            buttons[EXIT].set_sensitive(true);
            buttons[OPEN].set_sensitive(true);
            buttons[SAVE].set_sensitive(true);
            buttons[RESTART].set_sensitive(true);
            buttons[START].set_sensitive(false);
            buttons[STEP].set_sensitive(false);
            return false;
        }
        return true;
    }
    return false;
}


void My_window::set_infos()
{
    info_frame.set_child(info_grid);
    info_grid.set_column_homogeneous(true);
    for (size_t i(0); i < info_text.size(); ++i)
    {
        info_grid.attach(info_text[i], 0, i, 1, 1);
        info_grid.attach(info_value[i], 1, i, 1, 1);
        info_text[i].set_halign(Gtk::Align::START);
        info_value[i].set_halign(Gtk::Align::END);
        info_text[i].set_margin(3);
        info_value[i].set_margin(3);
    }
}


void My_window::update_infos()
{
    for (auto &value : info_value)
    {
        value.set_text("0");
    }
    info_value[0].set_text(to_string(m_game.get_score()));
    info_value[1].set_text(to_string(m_game.get_lives()));
    info_value[2].set_text(to_string(m_game.get_bricks().size()));
    info_value[3].set_text(to_string(m_game.get_balls().size()));
}


void My_window::set_drawing()
{
    drawing.set_content_width(drawing_size);
    drawing.set_content_height(drawing_size);
    drawing.set_expand();
    drawing.set_draw_func(sigc::mem_fun(*this, &My_window::on_draw));
}


void draw_split_square(Square sq,int niveau)
{
    
    Color color[3] = {ORANGE, YELLOW, GREEN};
    if (niveau > 2) {return;}
    if (sq.side < brick_size_min*2) {return;}

    double s = (sq.side-split_brick_gap) / 2;
    if (s < brick_size_min) {return;}

    double o = (split_brick_gap / 2) + (s/2);
    double cx = sq.center.x;
    double cy = sq.center.y;

    Square new_sq[4] = {
        {{cx - o, cy - o}, s},
        {{cx + o, cy - o}, s}, 
        {{cx - o, cy + o}, s},
        {{cx + o, cy + o}, s}
    };

    for (auto& sq_prime:new_sq)
    {
        set_color (color[niveau]);
        draw_square (sq_prime);
        draw_split_square (sq_prime, niveau+ 1);
    }

}


void My_window::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height)
{
    graphic_set_context(cr);
    double side (min(width, height));
    cr->translate((width - side) / 2, (height + side) / 2);
    cr->scale(side / (arena_size), -side / (arena_size));
    
    draw_arena();

    if (!level_loaded) return;
    
    draw_paddle(cr);
    draw_bricks(cr);
    draw_balls(cr);
}

void My_window::draw_paddle(const Cairo::RefPtr<Cairo::Context>& cr) 
{
    set_color(BLACK);
    draw_arc(m_game.get_paddle().getCircle());
    cr->set_line_width(1.0);
    cr->stroke();
}

void My_window::draw_bricks(const Cairo::RefPtr<Cairo::Context>& cr) 
{
    for (const auto& brick : m_game.get_bricks())
    {
        switch (brick->getType()) 
        {
            case BrickType::RAINBOW:
            {
                int hp = brick->get_hit_points();
                if (hp == 1) set_color(RED);
                else if (hp == 2) set_color(ORANGE);
                else if (hp == 3) set_color(YELLOW);
                else if (hp == 4) set_color(GREEN);
                else if (hp == 5) set_color(CYAN);
                else if (hp == 6) set_color(BLUE);
                else if (hp == 7) set_color(PURPLE); 
                draw_square(brick->getSquare());   
                break;
            }

            case BrickType::BALL:
            {
                set_color(RED);
                draw_square(brick->getSquare());
                
                set_color(BLACK);
                Circle c;
                c.center = brick->getSquare().center;
                c.r = new_ball_radius;
                draw_circle(c);
                cr->fill();
                break;
            }

            case BrickType::SPLIT:
            {
                set_color(RED);
                draw_square(brick->getSquare());
                draw_split_square(brick->getSquare(), 0); 
                break;
            }
        }         
    }
}

void My_window::draw_balls(const Cairo::RefPtr<Cairo::Context>& cr) 
{
    set_color(BLACK);
    for (const auto& ball : m_game.get_balls())
    {
        draw_circle(ball->getCircle()); 
        cr->fill(); 
    }
}

void My_window::set_mouse_controller()
{
    auto left_click = Gtk::GestureClick::create();
    auto move = Gtk::EventControllerMotion::create();

    left_click->set_button(GDK_BUTTON_PRIMARY);

    left_click->signal_pressed().connect(
        sigc::mem_fun(*this, &My_window::on_drawing_left_click));
    move->signal_motion().connect(sigc::mem_fun(*this, &My_window::on_drawing_move));

    drawing.add_controller(left_click);
    drawing.add_controller(move);
}


void My_window::on_drawing_left_click(int n_press, double x, double y)
{
    if (!level_loaded) return;

    m_game.create_new_ball();

    update_infos();
    drawing.queue_draw();
}


void My_window::on_drawing_move(double x, double y)
{
    int width = drawing.get_width();
    int height = drawing.get_height();

    double side = min(width, height);
    double offset_x = (width - side) / 2.0;

    // init_x vaut 0 en bas à gauche de l'arène, et pas de la fenêtre de dessin
    init_x = (x - offset_x) * arena_size / side;

    if (init_x < 0) {
        init_x = 0;
    }

    if (init_x > arena_size) {
        init_x = arena_size;
    }
}