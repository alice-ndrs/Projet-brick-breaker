#ifndef BALL_H
#define BALL_H

#include "tools.h"
#include "Paddle.h"

class Brick;
class Paddle;

class Ball 
{
    public:

        Ball (double x,double y,double r,double dx,double dy);
        virtual ~Ball() = default; // destructeur virtuel par défaut

        int check_ball () const;

        // --- Détection de collisions ---
        bool collision_ball (const Ball& other) const;
        bool collision_brick (const Brick& b) const;
        bool collision_paddle (const Paddle& p) const;
        bool lost() const;
        bool hits_vertical_wall() const;
        bool hits_top_wall() const;
        void reverse_dx() { dx = -dx; }
        void reverse_dy() { dy = -dy; }
        
        // --- Accesseurs ---
        double getX() const { return circle.center.x; }
        double getY() const { return circle.center.y; }
        double getR() const { return circle.r; }
        double getDx()const { return dx; }
        double getDy()const { return dy; }
        const Circle& getCircle() const { return circle; }

        void move();
        void undo_move();

        void reset(const Paddle& p);  // repositionnement de la ball si lives>0
        void inactive() { active=false; }// desactive la Ball
        bool clear() {return !active; } // indique si la Ball est conservée

    private:
        Circle circle;
        double dx=0.0,dy=0.0;// vecteur deplacement
        bool active;    // indique si la Ball est active ou pas
};

#endif