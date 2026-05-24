#ifndef BALL_H
#define BALL_H

#include "Constants.h"
#include "tools.h"

class Brick;
class Paddle;

class Ball {
  public:
    Ball(double x = 0, double y = 0, double r = new_ball_radius, double dx = 0,
         double dy = 0);
    ~Ball() = default;                       // destructeur virtuel par défaut
    Ball(const Ball &) = default;            // constructeur de copie par défaut
    Ball &operator=(const Ball &) = default; // opérateur d'affectation par défaut

    // vérifie que la balle est dans l'arène et que sa vitesse est valide
    bool check_ball() const;

    // --- Détection de collisions ---
    bool collision_ball(const Ball &other, bool epsil = false) const;
    bool collision_brick(const Brick &b, bool epsil = false) const;
    bool collision_paddle(const Paddle &p, bool epsil = false) const;
    bool lost() const; // si la balle est perdue en dehors de l'arène inférieure
    bool hits_vertical_wall() const;
    bool hits_top_wall() const;
    void reverse_dx() { dx = -dx; }
    void reverse_dy() { dy = -dy; }
    void set_delta(double dx, double dy);

    // --- Accesseurs ---
    double getX() const { return circle.center.x; }
    double getY() const { return circle.center.y; }
    double getR() const { return circle.r; }
    double getDx() const { return dx; }
    double getDy() const { return dy; }
    const Circle &getCircle() const { return circle; }

    // --- Fonction de Jeu ---
    void move();      // avance la balle selon son vecteur de déplacement
    void undo_move(); // annule le dernier déplacement de la balle

    void reset(const Paddle &p);           // repositionnement de la balle si lives>0
    void inactive() { active = false; }    // desactive la Ball
    bool clear() const { return !active; } // indique si la Ball est conservée

    void set_position(const Point &p) { circle.center = p; }
    Point get_position() const { return circle.center; }

  private:
    Circle circle;
    double dx = 0.0, dy = 0.0; // vecteur deplacement
    bool active;
    void clamp_delta();
};

#endif