#ifndef BRICK_H
#define BRICK_H

#include "Constants.h"
#include "tools.h"
#include <memory>
#include <vector>

class Paddle;

enum class BrickType { RAINBOW = 0, BALL = 1, SPLIT = 2 };

class Brick {
  public:
    Brick(BrickType type, double x, double y, double side);
    // Règle des 3
    virtual ~Brick() = default;
    Brick(const Brick &) = delete;
    Brick &operator=(const Brick &) = delete;

    // vérifie que la brique est bien positionnée et de taille valide
    bool check_brick() const;
    virtual bool check_specific() const { return true; }

    // --- Détection de collisions ---
    bool collision_brick(const Brick &other) const;
    bool collision_paddle(const Paddle &p, bool epsil = false) const;

    // --- Accesseurs ---
    double getX() const { return square.center.x; }
    double getY() const { return square.center.y; }
    double getC() const { return square.side; }
    const Square &getSquare() const { return square; }
    BrickType getType() const { return t; }

    // --- Fonction de Jeu ---
    virtual void hit() { remove = true; } // marque la brique comme détruite
    bool clear() const { return remove; } // indique si la brique doit être supprimée

    virtual int get_hit_points() const { return 1; }

  protected:
    BrickType t;   // type de brique parmi les 3
    Square square; // position et taille
    bool remove;   // true si la brick est a enlever
};

//------ Sous-classes de Brick ------

class Rainbow_brick : public Brick {
  public:
    Rainbow_brick(double x, double y, double c, int h);
    // Règle des 3
    ~Rainbow_brick() override = default;
    Rainbow_brick(const Rainbow_brick &) = delete;
    Rainbow_brick &operator=(const Rainbow_brick &) = delete;

    bool check_specific() const override;
    void hit() override; // surcharge pour décrémenter les vies
                        // avant de la détruire
    int get_hit_points() const override { return hit_points; }

  private:
    int hit_points;
};

class Ball_brick : public Brick {
  public:
    // Règle des 3
    ~Ball_brick() override = default;
    Ball_brick(const Ball_brick &) = delete;
    Ball_brick &operator=(const Ball_brick &) = delete;

    Ball_brick(double x, double y, double side);
    void hit() override;
};

class Split_brick : public Brick {
  public:
    // Règle des 3
    ~Split_brick() override = default;
    Split_brick(const Split_brick &) = delete;
    Split_brick &operator=(const Split_brick &) = delete;

    Split_brick(double x, double y, double side);
    void hit() override;
    std::vector<std::unique_ptr<Brick>> split() const;
};

#endif