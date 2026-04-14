#include <iostream>
#include <vector>
#include <memory>
#include "Game.h"
#include "Message.h"
#include "Brick.h"
#include <fstream>
#include <sstream>
using namespace std;

//------------- Constructeur Game -------------

Game::Game (int lives,int score):
lives(lives),score(score), paddle(), etat(SCORE), total(0), count(0)
{}

//reinitialise l'etat du jeu
void Game::reset() 
{
    etat = SCORE;
    total = 0;
    count = 0;
    lives = 0;
    score = 0;
    bricks.clear();
    balls.clear();
    paddle = Paddle();
}

void Game::nettoyer_objets() // pour les rendus suivants
{
    for (size_t i = 0; i < balls.size(); ) {
        if (balls[i]->clear()) {
            balls.erase(balls.begin() + i); 
        }else{
            i++;
        }
    }

    for (size_t i = 0; i < bricks.size();) {
        if (bricks[i]->clear()) {
            bricks.erase(bricks.begin() + i);
        } else{
            i++;
        }
    }
}

// cette fonction recoit un ligne de donnee puis decider quelle methode
// appeler
bool Game::decodage_ligne(istringstream& data)
{
    switch (etat) {
        case SCORE:    
            return decodage_score(data);

        case LIVES:
            return decodage_lives(data);

        case PADDLE:
            return decodage_paddle(data);

        case NB_BRICKS:
            return decodage_nb_bricks(data);

        case BRICKS:
            return decodage_brick(data);

        case NB_BALLS:
            return decodage_nb_balls(data);

        case BALLS:
            return decodage_ball(data);

        case FIN:
            return false;
    }

    return false;
}


bool Game::getLevel(const string& filename) // méthode de lecture de fichier
{ 
    reset();  // On repart de zero avant de charger

    ifstream file(filename);
    if (!file.fail()) 
    {
        string line; // lecture ligne par ligne en ignorant les espaces
        while (getline(file>>ws, line)) {
            if (line.empty()) continue;
            if (line[0]=='#') continue;

            istringstream data(line);

            if (decodage_ligne(data) == false)
                return false;
        }

        if (etat != FIN) return false;

        if (check_Collisions()) return false;

        cout<<message::success();
        return true;
    }
    return false;
}


bool Game::saveLevel(const string& filename){
    ofstream file(filename);
    if (!file.fail()) 
    {
        file<<score<<endl;
        file<<lives<<endl;
        file<<paddle.getX()<<" "<<paddle.getY()<<" "<<paddle.getR()<<endl;

        file<<bricks.size()<<endl;

        for(size_t i = 0; i < bricks.size(); ++i){
            file<<static_cast<int>(bricks[i]->getType())<<" "<<bricks[i]->getX()<<" "<<bricks[i]->getY();
            file<<" "<<bricks[i]->getC();
            if(bricks[i]->getType()==BrickType::RAINBOW){
                file <<" "<<bricks[i]->getHitPoints();
            }
            file<<endl;
        }

        file<< balls.size()<<endl;
        for(size_t i = 0; i < balls.size(); ++i){
            file<<balls[i]->getX()<<" "<<balls[i]->getY()<<" "<<balls[i]->getR();
            file<<" "<<balls[i]->getDx()<<" "<<balls[i]->getDy();
            file<<endl;
        }
        return true;
    }
    return false;

}

//------------- Fonctions de Décodage Spécifiques -------------

bool Game::decodage_score(istringstream& data) 
{
    int score;
    if (!(data >> score)) return false;

    if (score < 0) {
        cout << message::invalid_score(score);
        return false;
    }

    if (!is_line_empty(data)) return false;
    this->score = score;
    etat = LIVES;
    return true;
}


bool Game::decodage_lives(istringstream& data) 
{
    int lives;
    if (!(data >> lives)) return false;
    if (!is_line_empty(data)) return false;

    if (lives < 0) {
        cout << message::invalid_lives(lives);
        return false;
    }

    this->lives = lives;
    etat = PADDLE;
    return true;
}   


bool Game::decodage_paddle(istringstream& data) 
{
    double xPaddle, yPaddle, rPaddle;
    if (!(data >> xPaddle >> yPaddle >> rPaddle)) return false;
    if (!is_line_empty(data)) return false;
    
    Paddle p(xPaddle, yPaddle, rPaddle);
    if (p.check_Paddle()) return false; // limites arene
    
    this->paddle = p;
    etat = NB_BRICKS;
    return true;
}


bool Game::decodage_nb_bricks(istringstream& data) 
{
    int nb_brick;
    if (!(data >> nb_brick)) return false;
    if (!is_line_empty(data)) return false;

    if (nb_brick < 0) {
        return false;
    }

    total = nb_brick;
    count = 0;
    // si 0 briques -> on saute à la lecture des Ball
    if (nb_brick == 0) {
        etat = NB_BALLS;
    } else {
        etat = BRICKS;
    }
    return true;
}


bool Game::decodage_brick(istringstream& data) 
{
    int t;      // type
    double x, y, c; // position et cote

    if (!(data >> t >> x >> y >> c)) return false;

    switch (t) {
        case 0: { // Rainbow_brick-> paramètre supplémentaire (hit_points)
            int h;
            if (!(data >> h)) return false;
            if (!is_line_empty(data)) return false;
            std::unique_ptr<Rainbow_brick> b(new Rainbow_brick(x, y, c, h));
            if (b->check_Brick()) return false;
            bricks.push_back(std::move(b));
            break;
        }
        case 1: {
            if (!is_line_empty(data)) return false;
            std::unique_ptr<Ball_brick> b(new Ball_brick(x, y, c));
            if (b->check_Brick()) return false;
            bricks.push_back(std::move(b));
            break;
        }
        case 2: {
            if (!is_line_empty(data)) return false;
            std::unique_ptr<Split_brick> b(new Split_brick(x, y, c));
            if (b->check_Brick()) return false;
            bricks.push_back(std::move(b));
            break;
        }
        default:
            cout << message::invalid_brick_type(t);
            return false;
    }

    ++count;

    if (count == total) {
        etat = NB_BALLS;
    }

    return true;
}


bool Game::decodage_nb_balls(istringstream& data) 
{
    int nb_ball;
    if (!(data >> nb_ball)) return false;
    if (!is_line_empty(data)) return false;
    
    if (nb_ball < 0) return false;
    
    total = nb_ball;
    count = 0;

    if (nb_ball == 0) {
        etat = FIN;
    } else {
        etat = BALLS;
    }
    return true;
}


bool Game::decodage_ball(istringstream& data) 
{
    double xBall, yBall, rBall, dx, dy;
    if (!(data >> xBall >> yBall >> rBall >> dx >> dy)) return false;
    if (!is_line_empty(data)) return false;

    std::unique_ptr<Ball> b(new Ball(xBall, yBall, rBall, dx, dy));
    if (b->check_Ball()) {
        return false;
    }
    balls.push_back(std::move(b));
    ++count;

    if (count == total) {
        etat = FIN;
    }
    return true;
}

//------------- Gestion des collisions -------------

int Game::check_Collisions() const 
{
    if(collision_bricks()) return 1;
    if(collision_balls()) return 1;
    if(collision_ball_brick()) return 1;
    if(collision_ball_paddle()) return 1;
    if(collision_brick_paddle()) return 1;
    return 0;
}


bool Game::collision_bricks() const 
{
    for (size_t i = 0; i < bricks.size(); ++i) {
        for (size_t j = i + 1; j < bricks.size(); ++j) {
            if (bricks[i]->collision_brick(*bricks[j])) {
                cout << message::collision_bricks(i, j);
                return true;
            }
        }
    }
    return false;
}


bool Game::collision_balls() const 
{
    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            if (balls[i]->collision_ball(*balls[j])) {
                cout << message::collision_balls(i, j);
                return true;
            }
        }
    }
    return false;
}


bool Game::collision_ball_brick() const 
{
    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = 0; j < bricks.size(); ++j) {
            if (balls[i]->collision_brick(*bricks[j])) {
                cout << message::collision_ball_brick(i, j);
                return true;
            }
        }
    }
    return false;
}


bool Game::collision_ball_paddle() const 
{
    for (size_t i = 0; i < balls.size(); ++i) {
        if (balls[i]->collision_paddle(paddle)) {
            cout << message::collision_paddle_ball(i);
            return true;
        }
    }
    return false;
}


bool Game::collision_brick_paddle() const
{
    for (size_t i = 0; i < bricks.size(); ++i) {
        if (bricks[i]->collision_paddle(paddle)) {
            cout << message::collision_paddle_brick(i);
            return true;
        }
    }
    return false;
}


bool Game::is_line_empty(istringstream& data){
    data>>ws;               // White space: on evite les espaces 
    return data.eof();      //return false si il rest du text
}