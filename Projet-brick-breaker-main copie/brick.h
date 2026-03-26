#ifndef BRICK_H
#define BRICK_H

#include "tools.h"
#include "Constants.h"

class Paddle;

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
        virtual ~Brick() = default;
        int check_Brick() const;
        virtual int check_specific() const { return 0; }
        bool collision_brick (const Brick& other) const;
        bool collision_paddle (const Paddle& p) const;
        double getX() const { return square.center.x; }
        double getY() const { return square.center.y; }
        double getC() const { return square.side; }
        const Square& getSquare() const { return square; }
        BrickType getType() const { return t; }
        void clear() { t = BrickType::RAINBOW; square = {{0,0}, 0}; }
    protected:
        BrickType t;// type de brick parmis les 3
        Square square;// position et taille
};

class Rainbow_brick : public Brick 
{
    public :
        Rainbow_brick(double x, double y, double c, int h) : Brick(BrickType::RAINBOW, x, y, c), hit_points(h) {}
        int check_specific() const override;
    private : 
        int hit_points;
};

class Ball_brick : public Brick 
{
    public :
        Ball_brick(double x, double y, double c) : Brick(BrickType::BALL, x, y, c), new_ball_radius(::new_ball_radius) {}
    private :
        double new_ball_radius;
};

class Split_brick : public Brick 
{
    public :
        Split_brick(double x, double y, double c) : Brick(BrickType::SPLIT, x, y, c), split_brick_gap(::split_brick_gap) {}
    private :
        double split_brick_gap;
};

#endif