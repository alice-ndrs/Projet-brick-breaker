#include "Brick.h"
#include "Message.h"
#include "Paddle.h"
#include <iostream>
using namespace std;

//------------- Constructeur Brick et des sous-classes -------------

Brick::Brick(BrickType type, double x, double y, double side)
    : t(type), square{{x, y}, side}, remove(false) {}

Ball_brick::Ball_brick(double x, double y, double side)
    : Brick(BrickType::BALL, x, y, side) {}

Split_brick::Split_brick(double x, double y, double side)
    : Brick(BrickType::SPLIT, x, y, side) {}

Rainbow_brick::Rainbow_brick(double x, double y, double c, int h)
    : Brick(BrickType::RAINBOW, x, y, c), hit_points(h) {}

//------------- Fonctions de test pour la classe Brick -------------

bool Brick::check_brick() const {
    const int type_value = static_cast<int>(t);

    if (type_value < 0 || type_value > 2) { // verifie le type
        cout << message::invalid_brick_type(type_value);
        return false;
    }

    const bool outside_x = (square.center.x - square.side / 2 < 0) ||
                           (square.center.x + square.side / 2 > arena_size);

    const bool outside_y = (square.center.y - square.side / 2 < 0) ||
                           (square.center.y + square.side / 2 > arena_size);

    if (outside_x || outside_y) { // verifie si Brick est dans l'arene
        cout << message::brick_outside(square.center.x, square.center.y);
        return false;
    }

    if (square.side < brick_size_min) { // verifie la taille de la Brick
        cout << message::invalid_brick_size(square.side);
        return false;
    }
    return check_specific();
}

bool Rainbow_brick::check_specific() const {
    if (hit_points < 1 || hit_points > 7) { // verifie les hits point (Rainbow_brick)
        cout << message::invalid_hit_points(hit_points);
        return false;
    }
    return true;
}

// -------- Détection de collisions --------

bool Brick::collision_brick(const Brick &other) const // superposition 2 Brick
{
    return square_intersects_square(square, other.square);
}

bool Brick::collision_paddle(const Paddle &p, bool epsil) const // superposition Brick
{                                                               // et Paddle
    return square_intersects_circle(square, p.getCircle(), epsil);
}

// --- Fonction de Jeu ---

void Rainbow_brick::hit() {
    if (hit_points > 0) { // on decremente si la Brick a encore des hits points
        --hit_points;
    }

    if (hit_points <= 0) { // sinon on enleve la Brick
        remove = true;
    }
}

void Ball_brick::hit() { remove = true; }

void Split_brick::hit() { remove = true; }

std::vector<std::unique_ptr<Brick>> Split_brick::split() const {
    std::vector<std::unique_ptr<Brick>> new_bricks;
    const double split_side = (square.side - split_brick_gap) / 2;

    if (split_side < brick_size_min) return new_bricks;

    const double offset = (split_brick_gap / 2) + (split_side / 2);
    const double cx = square.center.x;
    const double cy = square.center.y;

    double positions[4][2] = {{cx - offset, cy - offset},
                              {cx + offset, cy - offset},
                              {cx - offset, cy + offset},
                              {cx + offset, cy + offset}};

    for (auto &pos : positions)
        new_bricks.push_back(
            std::make_unique<Split_brick>(pos[0], pos[1], split_side));

    return new_bricks;
}