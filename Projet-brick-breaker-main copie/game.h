#ifndef Game_H
#define Game_H
#include <vector>
#include <memory>
#include "Ball.h"
#include "Brick.h"  
#include "Paddle.h"
#include <sstream>

enum Status {
    ONGOING,
    WON,
    LOST
};

enum EtatLecture {// differente lecture
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

class Game
{
public:
    Game (int lives = 0,int score = 0);
    virtual ~Game()=default;

    bool getLevel (const std::string& filename); // déclaration de la 
                                                    //fin de lecture de fichier
    bool saveLevel (const std:: string& filename);
    // --- Gestion des collisions ---
    int check_collisions () const;              
    bool collision_bricks() const;
    bool collision_balls() const;
    bool collision_ball_brick() const;
    bool collision_ball_paddle() const;
    bool collision_brick_paddle() const;
    bool ball_hits_brick(Ball& b);
    bool ball_hits_paddle(Ball& b);
    bool ball_hits_ball(Ball& b);

    void reset();  // reinitialise l'etat du jeu
    void nettoyer_objets();
    bool decodage_ligne(std::istringstream& data);

    // --- Méthodes de décodage spécifiques ---
    bool decodage_score(std::istringstream& data);
    bool decodage_lives(std::istringstream& data);
    bool decodage_paddle(std::istringstream& data);
    bool decodage_nb_bricks(std::istringstream& data);
    bool decodage_brick(std::istringstream& data);
    bool decodage_nb_balls(std::istringstream& data);
    bool decodage_ball(std::istringstream& data);


    bool is_line_empty(std::istringstream& data);//verifie la ligne lue est
                                        //vide ou ne contient que des espaces
    
    const std::vector<std::unique_ptr<Brick>>& get_bricks() const { return bricks; }
    const std::vector<std::unique_ptr<Ball>>&  get_balls()  const { return balls;  }
    Paddle& get_paddle() { return paddle; }
    int get_score() {return score;}
    int get_lives() {return lives;}
    Status get_status () {return status;}

    void update();

private:
    void update_status();
    void update_balls();

    int lives;
    int score;

    std::vector<std::unique_ptr<Brick>> bricks;
    std::vector<std::unique_ptr<Ball>> balls;

    Paddle paddle;

    Status status;
    EtatLecture etat;
    unsigned int total;
    unsigned int count;
};
#endif