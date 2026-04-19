#ifndef BRICK_H
#define BRICK_H

#include "tools.h"
#include "Constants.h"

class Paddle;

//enum differents types de Brick
enum class BrickType
{
    RAINBOW = 0,
    BALL = 1,
    SPLIT = 2
};


class Brick 
{
    public:
        Brick(BrickType t, double x, double y, double c);
        virtual ~Brick() = default;// destructeur virtuel par defaut

        int check_Brick() const;
        virtual int check_specific() const { return 0; }

        // --- Détection de collisions ---
        bool collision_brick (const Brick& other) const;
        bool collision_paddle (const Paddle& p) const;

        // --- Accesseurs ---
        double getX() const { return square.center.x; }
        double getY() const { return square.center.y; }
        double getC() const { return square.side; }
        const Square& getSquare() const { return square; }
        BrickType getType() const { return t; }

        virtual void hit () { remove = true; } //virtual permet de regarder 
        bool clear() const { return remove; }         // les points de vies

        virtual int getHitPoints() const { return 1; };

    protected:
        BrickType t; // type de brick parmis les 3
        Square square;// position et taille
        bool remove; //true si la brick est a enlever
};  

//------ Sous-classes de Brick ------

class Rainbow_brick : public Brick 
{
    public :
        Rainbow_brick(double x, double y, double c, int h);
        int check_specific() const override;
        void hit() override; //surcharge pour decrementer les vies 
                            // avant de la detruire
        int getHitPoints() const override { return hit_points; }
    private : 
        int hit_points; // nbre de coups necessaires pour la casser
};


class Ball_brick : public Brick 
{
    public :
        Ball_brick(double x, double y, double c);
    private :
        double new_ball_radius; // (rendu 3) rayon de Ball à creer
};


class Split_brick : public Brick 
{
    public :
        Split_brick(double x, double y, double c);
    private :
        double split_brick_gap;// (rendu 3) parametre pour la de separation
};

#endif