#include <iostream>
#include <cmath>
#include <vector>
#include "Game.h"
#include "message.h"
#include "Brick.h"
#include "Ball.h"
#include "Paddle.h"
#include <fstream>
#include <sstream>
#include "constants.h"
using namespace std;

void Game::getLevel(const string& filename){ // méthode de lecture de fichier
    ifstream file(filename);

    if (file.fail()) {
        // afficher mess erreur "Erreur ouverture fichier"
    }

    string line;
    while (getline(file>>ws, line)) {
        istringstream data(line);
        // lire les données, créer les objets nécessaires
        // lecture du score :
        int score;
        data >> score;

        if (score < 0) {
            cout << message::invalid_score(score);
            return;
        }

        int lives;
        data >> lives;
        if (lives < 0){
            cout<<message::invalid_lives(lives);
        }

        double xPaddle,yPaddle,rPaddle;
        data>>xPaddle>>yPaddle>>rPaddle;
        erreur_paddle_arena(xPaddle,yPaddle,rPaddle);
        Paddle p (xPaddle,yPaddle,rPaddle);

        int nb_brick;
        data>> nb_brick;
        vector <Brick> Bricks;
        for (int i(0);i<nb_brick;i++){
            int t,h;
            double x,y,c;
            data>>t;
            erreur_type(t);
            if(t==0){
                data>>x>>y>>c>>h;
                erreur_brick_arena(x,y,c);
                erreur_taille_brick(c);
                erreur_hit(h);
                Brick b(t,x,y,c,h);
                Bricks.push_back(b);
            }else{
                data>>x>>y>>c;
                erreur_brick_arena(x,y,c);
                erreur_taille_brick(c);
                Brick b(t,x,y,c);
                Bricks.push_back(b);
            }
        }
        erreur_superposition_brick(Bricks);

        int nb_ball;
        data>>nb_ball;
        
        vector<Ball> Balls;
        for (int i(0); i<nb_ball;i++){
            double xBall,yBall,rBall,dx,dy;
            data>>xBall>>yBall>>rBall>>dx>>dy;
            erreur_delta (dx,dy);
            erreur_ball_arena (xBall,yBall,rBall);
            Ball b(xBall,yBall,rBall,dx,dy);
            Balls.push_back(b);
        }

        erreur_superposition_ball_paddle(nb_ball,Balls,p);



    }

    // mettre un destructeur avec "Correct file"
}

// getLevel est responsable de lire le fichier et vérifier que son contenu est correct.
// Les classes Brick, Ball, Paddle, etc. sont responsables de vérifier que leurs paramètres sont valides.
void erreur_paddle_arena(double x, double y, double r){
    double x1,x2;
    x1=x-sqrt(r*r-y*y);
    x2=x+sqrt(r*r-y*y);
    if (x1<0 or x2>arena_size){
        cout<<message::paddle_outside(x,y);
    }
}

void erreur_brick_arena(double x,double y,double c){
    if ((x - c/2 < arena_size and arena_size < x + c/2) and
    (y - c/2 < arena_size and arena_size < y + c/2)){
        cout<<message::brick_outside(x,y);
    }
}

void erreur_taille_brick (double size){
    if (size<brick_size_min){
        cout<<message::invalid_brick_size(size);
    }
}

void erreur_type (int type){
    if (type>2 or type<0){
        cout <<message::invalid_brick_type(type);
    }
}

void erreur_hit(int h){
    if (h<1 or h>7){
        cout<<message::invalid_hit_points(h);
    }

}

void erreur_superposition_brick(const vector<Brick>& b){
    for (int i (0); i < b.size(); i++){
        for (int j = i + 1; j < b.size(); j++){
            double dx = abs(b[i].getX() - b[j].getX());// distance selon x
            double dy = abs(b[i].getY() - b[j].getY());// distance selon y
            double max = (b[i].getC() + b[j].getC()) / 2.0;// distance max entre les deux coordonnée 

            if (dx <max and dy <max){
                cout <<message::collision_bricks(b[i].getC(), b[j].getC());
            }
        }
    }
}

void erreur_ball_arena (double x,double y,double r){
    if ((x - r/2 < arena_size and arena_size < x + r/2) and
    (y - r/2 < arena_size and arena_size < y + r/2)){
        cout<<message::ball_outside(x,y);
    }
}

void erreur_superposition_ball (const vector<Ball>& b){
    for(int i(0); i<b.size();i++){
        for (int j=i+1; j<b.size();j++){
            double dx=abs(b[i].getX() - b[j].getX());
            double dy = abs(b[i].getY() - b[j].getY());
            double distance=sqrt(dx*dx+dy*dy);
            if (distance<(b[i].getR()+b[j].getR())){
                cout<<message::collision_balls(i+1,j+1);
            }
        }
    }
}

void erreur_delta (double dx,double dy){
    double delta =sqrt (dx*dx +dy*dy);
    if (delta>delta_norm_max){
        cout<<message::invalid_delta(dx, dy);
    }
}

void erreur_superposition_ball_paddle(int nb_ball,vector<Ball> b,Paddle p){
    for(int i(0); i<b.size();i++){
        double dx=abs(b[i].getX() - p.getX());
        double dy = abs(b[i].getY() - p.getY());
        double distance=sqrt(dx*dx+dy*dy);
        if (distance<(b[i].getR()+p.getR())){
            cout<<message::collision_paddle_ball(i+1);
        }
    }
}


Game::Game (int lives,int score):
lives(lives),score(score)
{}