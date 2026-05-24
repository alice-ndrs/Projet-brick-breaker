#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"

class Paddle {
  public:
    Paddle(double x = 0, double y = 0, double r = 0); // constructeur par défaut
    ~Paddle() = default;                              // destructeur par défaut
    Paddle &operator=(const Paddle &) = default; // opérateur d'affectation par défaut
    Paddle(const Paddle &) = default;            // constructeur de copie par défaut

    bool check_paddle(bool print_error = true, bool epsil = false) const; // vérifie que la raquette reste dans l'arène

    // --- Accesseurs ---
    double getX() const { return circle.center.x; }
    double getY() const { return circle.center.y; }
    double getR() const { return circle.r; }
    void setX(double x) { circle.center.x = x; }
    const Circle &getCircle() const { return circle; }

    void setPrevX(double x) { prev_x = x; }
    double getPrevX() const { return prev_x; }

  private:
    double prev_x = 0.0;
    Circle circle;
};

#endif