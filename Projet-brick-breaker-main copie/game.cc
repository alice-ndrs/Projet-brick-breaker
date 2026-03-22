#include <iostream>
#include <cmath>
#include <vector>
#include "Game.h"
#include "Message.h"
#include "Brick.h"
#include "Ball.h"
#include "Paddle.h"
#include <fstream>
#include <sstream>
#include "Constants.h"
using namespace std;

void Game::reset() {
    etat = SCORE;
    total = 0;
    count = 0;
    lives = 0;
    score = 0;
    bricks.clear();
    balls.clear();
    paddle = Paddle();
}

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

bool Game::getLevel(const string& filename){ // méthode de lecture de fichier
    
    reset();

    ifstream file(filename);
    if (!file.fail()) 
    {
        string line;
        while (getline(file>>ws, line)) {
            if (line.empty()) continue;
            if(line[0]=='#')  continue;

            istringstream data(line);

            if(decodage_ligne(data) == false)
                return false;
        }
        if (etat != FIN) return false;

        if (check_Collisions()) { return false; }
        
        cout<<message::success() << endl;
        return true;
    }
    return false;
}

bool Game::decodage_score(istringstream& data) {
    int score;
    data >> score;

    if (score < 0) {
        cout << message::invalid_score(score);
        return false;
    }
    this->score = score;
    etat = LIVES;
    return true;
}

bool Game::decodage_lives(istringstream& data) {
    int lives;
    data >> lives;
    if (lives < 0) {
        cout << message::invalid_lives(lives);
        return false;
    }
    this->lives = lives;
    etat = PADDLE;
    return true;
}   

bool Game::decodage_paddle(istringstream& data) {
    double xPaddle, yPaddle, rPaddle;
    data >> xPaddle >> yPaddle >> rPaddle;
    Paddle p(xPaddle, yPaddle, rPaddle);
    if (p.check_Paddle()) {
        return false;
    }
    this->paddle = p;
    etat = NB_BRICKS;
    return true;
}

bool Game::decodage_nb_bricks(istringstream& data) {
    int nb_brick;
    data >> nb_brick;
    if (nb_brick < 0) {
        return false;
    }
    total = nb_brick;
    count = 0;

    if (nb_brick == 0) {
        etat = NB_BALLS;
    } else {
        etat = BRICKS;
    }
    return true;
}

bool Game::decodage_brick(istringstream& data) {
    int t;
    double x, y, c;

    if (!(data >> t >> x >> y >> c)) return false;

    if (t == 0) {
        int h;
        if (!(data >> h)) return false;

        Rainbow_brick b(t, x, y, c, h);
        if (b.check_Brick()) {
            return false;
        }
        bricks.push_back(b);
    } 
    if (t == 1) {
        Ball_brick b(t, x, y, c);
        if (b.check_Brick()) {
            return false;
        }
        bricks.push_back(b);
    }
    if (t == 2) {
        Split_brick b(t, x, y, c);
        if (b.check_Brick()) {
            return false;
        }
        bricks.push_back(b);
    }

    ++count;

    if (count == total) {
        etat = NB_BALLS;
    }

    return true;
}

bool Game::decodage_nb_balls(istringstream& data) {
    int nb_ball;
    data >> nb_ball;
    if (nb_ball < 0) {
        return false;
    } 
    total = nb_ball;
    count = 0;

    if (nb_ball == 0) {
        etat = FIN;
    } else {
        etat = BALLS;
    }
    return true;
}

bool Game::decodage_ball(istringstream& data) {
    double xBall, yBall, rBall, dx, dy;
    data >> xBall >> yBall >> rBall >> dx >> dy;
    Ball b(xBall, yBall, rBall, dx, dy);
    if (b.check_Ball()) {
        return false;
    }
    balls.push_back(b);
    ++count;

    if (count == total) {
        etat = FIN;
    }
    return true;
}






// ANCIEN GETLINE :

//     string line;
//     while (getline(file>>ws, line)) {
//         istringstream data(line);
//         // lire les données, créer les objets nécessaires
//         // lecture du score :
//         int score;
//         data >> score;

//         if (score < 0) {
//             cout << message::invalid_score(score);
//             return;
//         }

//         int lives;
//         data >> lives;
//         if (lives < 0){
//             cout<<message::invalid_lives(lives);
//         }

//         double xPaddle,yPaddle,rPaddle;
//         data>>xPaddle>>yPaddle>>rPaddle;
        
//         //erreur_paddle_arena(xPaddle,yPaddle,rPaddle);
//         Paddle p (xPaddle,yPaddle,rPaddle);
//         Paddle.check_Paddle(p);

//         int nb_brick;
//         data>> nb_brick;
//         vector <Brick> Bricks;
//         for (int i(0);i<nb_brick;i++){
//             int t,h;
//             double x,y,c;
//             data>>t;
//             // erreur_type(t);
//             if(t==0){
//                 data>>x>>y>>c>>h;
//                 //erreur_brick_arena(x,y,c);
//                 //erreur_taille_brick(c);
//                 //erreur_hit(h);
//                 Brick b(t,x,y,c,h);
//                 Bricks.push_back(b);
//             }else{
//                 data>>x>>y>>c;
//                 //erreur_brick_arena(x,y,c);
//                 //erreur_taille_brick(c);
//                 Brick b(t,x,y,c);
//                 Bricks.push_back(b);
//             }
//         }
//         //erreur_superposition_brick(Bricks);

//         int nb_ball;
//         data>>nb_ball;
        
//         vector<Ball> Balls;
//         for (int i(0); i<nb_ball;i++){
//             double xBall,yBall,rBall,dx,dy;
//             data>>xBall>>yBall>>rBall>>dx>>dy;
//             //erreur_delta (dx,dy);
//             //erreur_ball_arena (xBall,yBall,rBall);
//             Ball b(xBall,yBall,rBall,dx,dy);
//             Balls.push_back(b);
//         }

//         //erreur_superposition_ball_paddle(nb_ball,Balls,p);



//     }

//     // mettre un destructeur avec "Correct file"
// }



int Game::check_Collisions() const { // à quoi sert le const ???
    if(collision_bricks()) return 1;
    if(collision_balls()) return 1;
    if(collision_ball_brick()) return 1;
    if(collision_ball_paddle()) return 1;
    if(collision_brick_paddle()) return 1;
    return 0;
}

bool Game::collision_bricks() const {
    for (size_t i = 0; i < bricks.size(); ++i) {
        for (size_t j = i + 1; j < bricks.size(); ++j) {
            if (bricks[i].collision_brick(bricks[j])) {
                cout << message::collision_bricks(i, j) << endl;
                return true;
            }
        }
    }
    return false;
}

bool Game::collision_balls() const {
    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            if (balls[i].collision_ball(balls[j])) {
                cout << message::collision_balls(i, j) << endl;
                return true;
            }
        }
    }
    return false;
}

bool Game::collision_ball_brick() const {
    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = 0; j < bricks.size(); ++j) {
            if (balls[i].collision_brick(bricks[j])) {
                cout << message::collision_ball_brick(i, j) << endl;
                return true;
            }
        }
    }
    return false;
}

bool Game::collision_ball_paddle() const {
    for (size_t i = 0; i < balls.size(); ++i) {
        if (balls[i].collision_paddle(paddle)) {
            cout << message::collision_paddle_ball(i) << endl;
            return true;
        }
    }
    return false;
}

bool Game::collision_brick_paddle() const
{
    for (size_t i = 0; i < bricks.size(); ++i) {
        if (bricks[i].collision_paddle(paddle)) {
            cout << message::collision_paddle_brick(i) << endl;
            return true;
        }
    }
    return false;
}

// getLevel est responsable de lire le fichier et vérifier que son contenu est correct.
// Les classes Brick, Ball, Paddle, etc. sont responsables de vérifier que leurs paramètres sont valides.

// void erreur_paddle_arena(double x, double y, double r){
//     double x1,x2;
//     x1=x-sqrt(r*r-y*y);
//     x2=x+sqrt(r*r-y*y);
//     if (x1<0 or x2>arena_size){
//         cout<<message::paddle_outside(x,y);
//     }
// }

// void erreur_brick_arena(double x,double y,double c){
//     if ((x - c/2 < arena_size and arena_size < x + c/2) and
//     (y - c/2 < arena_size and arena_size < y + c/2)){
//         cout<<message::brick_outside(x,y);
//     }
// }

// void erreur_taille_brick (double size){
//     if (size<brick_size_min){
//         cout<<message::invalid_brick_size(size);
//     }
// }

// void erreur_type (int type){
//     if (type>2 or type<0){
//         cout <<message::invalid_brick_type(type);
//     }
// }

// void erreur_hit(int h){
//     if (h<1 or h>7){
//         cout<<message::invalid_hit_points(h);
//     }
// }

// int erreur_superposition_brick(const vector<Brick>& b){
//     for (int i (0); i < b.size(); i++){
//         for (int j = i + 1; j < b.size(); j++){
//             double dx = abs(b[i].getX() - b[j].getX());// distance selon x
//             double dy = abs(b[i].getY() - b[j].getY());// distance selon y
//             double max = (b[i].getC() + b[j].getC()) / 2.0;// distance max entre les deux coordonnée 

//             if (dx <max and dy <max){
//                 cout <<message::collision_bricks(b[i].getC(), b[j].getC());
//                 return 1;
//             }
//         }
//     }
//     return 0;
// }

// void erreur_ball_arena (double x,double y,double r){
//     if ((x - r/2 < arena_size and arena_size < x + r/2) and
//     (y - r/2 < arena_size and arena_size < y + r/2)){
//         cout<<message::ball_outside(x,y);
//     }
// }

// int erreur_superposition_ball (const vector<Ball>& b){
//     for(int i(0); i<b.size();i++){
//         for (int j=i+1; j<b.size();j++){
//             double dx=abs(b[i].getX() - b[j].getX());
//             double dy = abs(b[i].getY() - b[j].getY());
//             double distance=sqrt(dx*dx+dy*dy);
//             if (distance<(b[i].getR()+b[j].getR())){
//                 cout<<message::collision_balls(i+1,j+1);
//                 return 1;
//             }
//         }
//     }
//     return 0;
// }

// void erreur_delta (double dx,double dy){
//     double delta =sqrt (dx*dx +dy*dy);
//     if (delta>delta_norm_max){
//         cout<<message::invalid_delta(dx, dy);
//     }
// }

// void erreur_superposition_ball_paddle(int nb_ball,vector<Ball> b,Paddle p){
//     for(int i(0); i<b.size();i++){
//         double dx=abs(b[i].getX() - p.getX());
//         double dy = abs(b[i].getY() - p.getY());
//         double distance=sqrt(dx*dx+dy*dy);
//         if (distance<(b[i].getR()+p.getR())){
//             cout<<message::collision_paddle_ball(i+1);
//         }
//     }
// }


Game::Game (int lives,int score):
lives(lives),score(score)
{}