#ifndef GAME_H
#define GAME_H

#include <memory>
#include <sstream>
#include <vector>

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"

enum Status { ONGOING, WON, LOST };

enum EtatLecture { // differente lecture
    SCORE,
    LIVES,
    PADDLE,
    NB_BRICKS,
    BRICKS,
    NB_BALLS,
    BALLS,
    FIN
};

//------------- Classe principale (logique globale du jeu) -------------

class Game {
  public:
    Game(int lives = 0, int score = 0);
    virtual ~Game() = default;
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;
    Game(Game &&) = default;
    Game &operator=(Game &&) = default;

    bool get_level(
        const std::string &filename); // lit un fichier de niveau et initialise le jeu
    bool save_level(
        const std::string &filename); // sauvegarde le niveau courant dans un fichier

    //------ Méthodes de gestion du jeu ------
    void reset();                        // remet le jeu dans son état initial
    void update();                       // met à jour la physique et l'état du niveau
    void update_paddle(double target_x); // déplace la raquette vers la position cible
    void create_new_ball();              // crée une nouvelle balle après perte

    const std::vector<std::unique_ptr<Brick>> &get_bricks() const { return bricks; }
    const std::vector<std::unique_ptr<Ball>> &get_balls() const { return balls; }

    const Paddle &get_paddle() const { return paddle; }

    int get_score() const { return score; }
    int get_lives() const { return lives; }
    Status get_status() const { return status; }

  private:
    // --- Méthodes de gestion du jeu ---
    void update_status();
    void set_initial_status();
    void update_balls();
    void add_pending_objects();
    void nettoyer_objets();

    // --- Méthodes de décodage ---
    bool decodage_ligne(std::istringstream &data);
    bool decodage_score(std::istringstream &data);
    bool decodage_lives(std::istringstream &data);
    bool decodage_paddle(std::istringstream &data);
    bool decodage_nb_bricks(std::istringstream &data);
    bool decodage_brick(std::istringstream &data);
    bool decodage_nb_balls(std::istringstream &data);
    bool decodage_ball(std::istringstream &data);
    bool is_line_empty(std::istringstream &data);

    // --- Gestion des collisions ---
    bool check_collisions() const;
    bool collision_bricks() const;
    bool collision_balls() const;
    bool collision_ball_brick() const;
    bool collision_ball_paddle() const;
    bool collision_brick_paddle() const;
    // --- Gestion des rebonds ---
    bool ball_hits_brick(Ball &b);
    bool ball_hits_paddle(Ball &b);
    bool ball_hits_ball(Ball &b);

    void process_ball_brick_collision(Ball &ball, const Brick &brick);
    void handle_brick_hit(const Point &incident_delta, Brick &brick);

    int lives;
    int score;

    std::vector<std::unique_ptr<Brick>> bricks;
    std::vector<std::unique_ptr<Ball>> balls;
    std::vector<std::unique_ptr<Ball>> pending_balls;
    std::vector<std::unique_ptr<Brick>> pending_bricks;

    Paddle paddle;

    Status status;
    EtatLecture etat;
    unsigned int total;
    unsigned int count;
};
#endif