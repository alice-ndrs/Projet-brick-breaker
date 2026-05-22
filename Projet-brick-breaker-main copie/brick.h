#ifndef BRICK_H
#define BRICK_H

#include <vector>
#include <memory>
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
    Brick(const Brick&) = delete; // constructeur de copie par défaut
    Brick& operator=(const Brick&) = delete; // opérateur d'affectation par defaut 

    int check_brick() const;
    virtual int check_specific() const { return 0; }

    // --- Détection de collisions ---
    bool collision_brick (const Brick& other) const;
    bool collision_paddle (const Paddle& p, bool epsil = false) const;

    // --- Accesseurs ---
    double getX() const { return square.center.x; }
    double getY() const { return square.center.y; }
    double getC() const { return square.side; }
    const Square& getSquare() const { return square; }
    BrickType getType() const { return t; }

    virtual void hit () { remove = true; } //virtual permet de regarder 
    bool clear() const { return remove; }         // les points de vies

    virtual int get_hit_points() const { return 1; }

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
    ~Rainbow_brick() override = default;
    Rainbow_brick(const Rainbow_brick&) = delete;
    Rainbow_brick& operator=(const Rainbow_brick&) = delete;

    int check_specific() const override;
    void hit() override; // surcharge pour decrementer les vies 
                        // avant de la detruire
    int get_hit_points() const override { return hit_points; }
private : 
    int hit_points; // nbre de coups necessaires pour la casser
};


class Ball_brick : public Brick 
{
public :
    ~Ball_brick() override = default;
    Ball_brick(const Ball_brick&) = delete;
    Ball_brick& operator=(const Ball_brick&) = delete;

    Ball_brick(double x, double y, double c);
    void hit() override;
};


class Split_brick : public Brick 
{
public :
    ~Split_brick() override = default;
    Split_brick(const Split_brick&) = delete;
    Split_brick& operator=(const Split_brick&) = delete;

    Split_brick(double x, double y, double c);
    void hit() override;
    std::vector<std::unique_ptr<Brick>> split() const;
};

#endif