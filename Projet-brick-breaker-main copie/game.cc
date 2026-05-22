#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include "Message.h"
#include "Brick.h"
#include "Game.h"

using namespace std;

//------------- Constructeur Game -------------

Game::Game (int lives,int score)
    : lives(lives),score(score), paddle(), 
    status(Status::ONGOING), etat(SCORE), total(0), count(0)
{}

//------------- Gestion du jeu -------------
//reinitialise l'etat du jeu
void Game::reset() 
{
    etat = SCORE;
    total = 0;
    count = 0;
    lives = 0; 
    score = 0;
    bricks.clear();
    balls.clear();
    pending_balls.clear();
    pending_bricks.clear();
    paddle = Paddle();
    status = Status::ONGOING;
}


void Game::nettoyer_objets()
{
    for (size_t i = 0; i < balls.size(); ) {
        if (balls[i]->clear()) {
            balls.erase(balls.begin() + i); 
        } else {
            i++;
        }
    }

    for (size_t i = 0; i < bricks.size();) {
        if (bricks[i]->clear()) {
            bricks.erase(bricks.begin() + i);
        } else {
            i++;
        }
    }
}

// cette fonction recoit un ligne de donnee puis decider quelle methode
// appeler
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


bool Game::get_level(const string& filename) // méthode de lecture de fichier
{ 
    reset();  // On repart de zero avant de charger

    ifstream file(filename);
    if (!file.fail()) 
    {
        string line; // lecture ligne par ligne en ignorant les espaces
        while (getline(file>>ws, line)) {
            if (line.empty()) continue;
            if (line[0] == '#') continue;

            istringstream data(line);

            if (decodage_ligne(data) == false)
            {
                reset();
                return false;
            }
        }

        if (etat != FIN) {
            reset();
            return false;
        }

        if (check_collisions()) {
            reset();
            return false;
        }
        set_initial_status();
        cout << message::success();
        return true;
    }
    return false;
}


bool Game::save_level(const string& filename)
{
    ofstream file(filename);
    if (!file.fail()) 
    {
        file << score << endl;
        file << lives << endl;
        file << paddle.getX() << " " << paddle.getY() << " " << paddle.getR() << endl;

        file << bricks.size()<< endl;

        for (size_t i = 0; i < bricks.size(); ++i){
            file << static_cast<int>(bricks[i]->getType()) << " " 
                << bricks[i]->getX() << " " 
                << bricks[i]->getY();
            file << " " << bricks[i]->getC();
            if (bricks[i]->getType() == BrickType::RAINBOW){
                file << " " << bricks[i]->get_hit_points();
            }
            file << endl;
        }

        file << balls.size() << endl;
        for (size_t i = 0; i < balls.size(); ++i){
            file << balls[i]->getX() << " " << balls[i]->getY() << " " << balls[i]->getR();
            file << " " << balls[i]->getDx() << " " << balls[i]->getDy();
            file << endl;
        }
        return true;
    }
    return false;

}

void Game::update() 
{
    if (status != Status::ONGOING) return;

    update_balls();
    nettoyer_objets();
    add_pending_objects();
    update_status();
}

void Game::update_status()
{
    if (status != Status::ONGOING) return;
    
    if (balls.empty() && lives == 0) 
    {
        status = Status::LOST;
        cout << message::lost();
        return;
    }
    
    if (bricks.empty()) 
    {
        score += score_per_life * lives;
        status = Status::WON;
        cout << message::won();
        return;
    }
}

void Game::set_initial_status()
{
    if (balls.empty() && lives == 0) 
    {
        status = Status::LOST;
        cout << message::lost();
        return;
    }

    if (bricks.empty()) 
    {
        status = Status::WON;
        cout << message::won();
        return;
    }

    status = Status::ONGOING;
}


bool Game::is_line_empty(istringstream& data)
{
    data >> ws;               // White space: on evite les espaces 
    return data.eof();      //return false si il rest du text
}

//------------- Mise a jour des Modules Game -------------

void Game::update_paddle(double target_x)
{
    const double old_x = paddle.getX();
    const double diff = target_x - old_x;
    double new_x = old_x;

    if (std::abs(diff) <= delta_norm_max)
    {   
        new_x = target_x;
    }
    else
    {
        if (diff > 0) {
            new_x = old_x + delta_norm_max;
        }
        else {
            new_x = old_x - delta_norm_max;
        }
    }

    paddle.setX(new_x);

    if (paddle.check_paddle(false, true))
    {
        paddle.setX(old_x);
    }
    else
    {
        for (const auto& brick : bricks)
        {
            if (circle_intersects_square(paddle.getCircle(),
                    brick->getSquare(), true))
            {
                paddle.setX(old_x);
                break;
            }
        }
    }
    paddle.setPrevX(old_x);
}

void Game::update_balls()
{
    for (auto& ball : balls)
    {
        Point initial_position = ball->get_position();
        
        ball->move();
        if (ball->lost()) {
            ball->inactive();
            continue;
        }

        unsigned nb_bounce = 0;
        bool collision = true;

        while (collision && nb_bounce < nb_bounce_max)
        {
            collision = false;

            if (ball->hits_vertical_wall()) {
                ball->undo_move();
                ball->reverse_dx();
                ball->move();
                collision = true;
            }
            else if (ball->hits_top_wall()) {
                ball->undo_move();
                ball->reverse_dy();
                ball->move();
                collision = true;
            }
            else if (ball_hits_brick(*ball)) {collision = true;}
            else if (ball_hits_paddle(*ball)) {collision = true;}
            else if (ball_hits_ball(*ball)) {collision = true;}

            if (collision) {++nb_bounce;}
            
            if (ball->lost()) {
                ball->inactive();
                break;
            }
        }
        if (nb_bounce >= nb_bounce_max && !ball->clear()) {
            ball->set_position(initial_position);
        }
    }
}

void Game::create_new_ball()
{
    if (status != Status::ONGOING) return;
    if (!balls.empty()) return;
    if (lives <= 0) return;

    auto ball = std::make_unique<Ball>();
    ball->reset(paddle);

    balls.push_back(std::move(ball));
    --lives;
}

void Game::add_pending_objects()
{
    for (auto& ball : pending_balls)
    {
        balls.push_back(std::move(ball));
    }
    pending_balls.clear();

    for (auto& brick : pending_bricks)
    {
        bricks.push_back(std::move(brick));
    }
    pending_bricks.clear();
}

//------------- Fonctions de Décodage Spécifiques -------------

bool Game::decodage_score(istringstream& data) 
{
    int score;
    if (!(data >> score)) return false;

    if (score < 0) {
        cout << message::invalid_score(score);
        return false;
    }

    if (!is_line_empty(data)) return false;
    this->score = score;
    etat = LIVES;
    return true;
}


bool Game::decodage_lives(istringstream& data) 
{
    int lives;
    if (!(data >> lives)) return false;
    if (!is_line_empty(data)) return false;

    if (lives < 0) {
        cout << message::invalid_lives(lives);
        return false;
    }

    this->lives = lives;
    etat = PADDLE;
    return true;
}   


bool Game::decodage_paddle(istringstream& data) 
{
    double xPaddle, yPaddle, rPaddle;
    if (!(data >> xPaddle >> yPaddle >> rPaddle)) return false;
    if (!is_line_empty(data)) return false;
    
    Paddle p(xPaddle, yPaddle, rPaddle);
    if (p.check_paddle()) return false; // limites arene
    
    this->paddle = p;
    etat = NB_BRICKS;
    return true;
}


bool Game::decodage_nb_bricks(istringstream& data) 
{
    int nb_brick;
    if (!(data >> nb_brick)) return false;
    if (!is_line_empty(data)) return false;

    if (nb_brick < 0) {
        return false;
    }

    total = nb_brick;
    count = 0;
    // si 0 briques -> on saute à la lecture des Ball
    if (nb_brick == 0) {
        etat = NB_BALLS;
    } else {
        etat = BRICKS;
    }
    return true;
}


bool Game::decodage_brick(istringstream& data) 
{
    int t; // type
    double x, y, c; // position et cote

    if (!(data >> t >> x >> y >> c)) return false;

    switch (t) {
        case 0: { // Rainbow_brick-> paramètre supplémentaire (hit_points)
            int h;
            if (!(data >> h)) return false;
            if (!is_line_empty(data)) return false;
            auto b = std::make_unique<Rainbow_brick>(x, y, c, h);
            // std::unique_ptr<Rainbow_brick> b(new Rainbow_brick(x, y, c, h));
            if (b->check_brick()) return false;
            bricks.push_back(std::move(b));
            break;
        }
        case 1: {
            if (!is_line_empty(data)) return false;
            auto b = std::make_unique<Ball_brick>(x, y, c);
            // std::unique_ptr<Ball_brick> b(new Ball_brick(x, y, c));
            if (b->check_brick()) return false;
            bricks.push_back(std::move(b));
            break;
        }
        case 2: {
            if (!is_line_empty(data)) return false;
            auto b = std::make_unique<Split_brick>(x, y, c);
            // std::unique_ptr<Split_brick> b(new Split_brick(x, y, c));
            if (b->check_brick()) return false;
            bricks.push_back(std::move(b));
            break;
        }
        default:
            cout << message::invalid_brick_type(t);
            return false;
    }

    ++count;

    if (count == total) {
        etat = NB_BALLS;
    }

    return true;
}


bool Game::decodage_nb_balls(istringstream& data) 
{
    int nb_ball;
    if (!(data >> nb_ball)) return false;
    if (!is_line_empty(data)) return false;
    
    if (nb_ball < 0) return false;
    
    total = nb_ball;
    count = 0;

    if (nb_ball == 0) {
        etat = FIN;
    } else {
        etat = BALLS;
    }
    return true;
}


bool Game::decodage_ball(istringstream& data) 
{
    double xBall, yBall, rBall, dx, dy;
    if (!(data >> xBall >> yBall >> rBall >> dx >> dy)) return false;
    if (!is_line_empty(data)) return false;

    auto b = std::make_unique<Ball>(xBall, yBall, rBall, dx, dy);
    // std::unique_ptr<Ball> b(new Ball(xBall, yBall, rBall, dx, dy));
    if (b->check_ball()) {
        return false;
    }
    balls.push_back(std::move(b));
    ++count;

    if (count == total) {
        etat = FIN;
    }
    return true;
}

//------------- Gestion des collisions -------------

int Game::check_collisions() const 
{
    if (collision_bricks()) return 1;
    if (collision_balls()) return 1;
    if (collision_ball_brick()) return 1;
    if (collision_ball_paddle()) return 1;
    if (collision_brick_paddle()) return 1;
    return 0;
}


bool Game::collision_bricks() const 
{
    for (size_t i = 0; i < bricks.size(); ++i) {
        for (size_t j = i + 1; j < bricks.size(); ++j) {
            if (bricks[i]->collision_brick(*bricks[j])) {
                cout << message::collision_bricks(i, j);
                return true;
            }
        }
    }
    return false;
}


bool Game::collision_balls() const 
{
    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            if (balls[i]->collision_ball(*balls[j])) {
                cout << message::collision_balls(i, j);
                return true;
            }
        }
    }
    return false;
}


bool Game::collision_ball_brick() const 
{
    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = 0; j < bricks.size(); ++j) {
            if (balls[i]->collision_brick(*bricks[j])) {
                cout << message::collision_ball_brick(i, j);
                return true;
            }
        }
    }
    return false;
}


bool Game::collision_ball_paddle() const 
{
    for (size_t i = 0; i < balls.size(); ++i) {
        if (balls[i]->collision_paddle(paddle)) {
            cout << message::collision_paddle_ball(i);
            return true;
        }
    }
    return false;
}


bool Game::collision_brick_paddle() const
{
    for (size_t i = 0; i < bricks.size(); ++i) {
        if (bricks[i]->collision_paddle(paddle)) {
            cout << message::collision_paddle_brick(i);
            return true;
        }
    }
    return false;
}

bool Game::ball_hits_brick(Ball& ball) 
{
    for (auto& brick : bricks)
    {
        if (brick->clear()) continue;

        if (ball.collision_brick(*brick, true))
        {
            Point incident_delta = {ball.getDx(), ball.getDy()};
            
            process_ball_brick_collision(ball, *brick);
            handle_brick_hit(incident_delta, *brick);

            score += score_per_hit;

            return true;
        }
    }
    return false;
}

void Game::process_ball_brick_collision(Ball& ball, const Brick& brick)
{
    const Point difference = ball.getCircle().center - brick.getSquare().center;
    const double half = brick.getC() / 2.0;

    const Point bounded = { // pt du carré le + proche du centre de la balle
        std::max(-half, std::min(difference.x, half)),
        std::max(-half, std::min(difference.y, half))
    };

    const Point normal = difference - bounded;// direction nominale/normale du choc
    const double normal_squared_norm = squared_norm(normal);
    const double epsil_squared = epsil_zero * epsil_zero;

    ball.undo_move();

    if (normal_squared_norm > epsil_squared)
    {
        const Point v = {ball.getDx(), ball.getDy()};

        const Point vn = (dot(v, normal) / normal_squared_norm) * normal; // partie de la vitesse dirigée selon la normale
        const Point new_v = v - 2.0 * vn; // on inverse que la normale

        ball.set_delta(new_v.x, new_v.y);
    }
    else
    {
        // pour éviter "effet tunnel"
        if (std::abs(ball.getDx()) > std::abs(ball.getDy())) { 
            ball.reverse_dx(); 
        }
        else { 
            ball.reverse_dy(); 
        }
    }
    ball.move();
}

void Game::handle_brick_hit(const Point& incident_delta, Brick& brick)
{
    const BrickType type = brick.getType();
    const double x = brick.getX();
    const double y = brick.getY();

    std::vector<std::unique_ptr<Brick>> new_bricks;

    // On prépare les nouvelles briques avant de modifier le vector bricks
    if (type == BrickType::SPLIT)
    {
        auto* split_brick = dynamic_cast<Split_brick*>(&brick);
        if (split_brick)
        {
            new_bricks = split_brick->split();
        }
    }

    // L'ancienne brique est marquée comme détruite / décrémentée
    brick.hit();

    if (type == BrickType::BALL)
    {
        auto new_ball = std::make_unique<Ball>();
        new_ball->set_position({ x, y });
        new_ball->set_delta(incident_delta.x, incident_delta.y);

        pending_balls.push_back(std::move(new_ball));
    }

    // On ajoute les sous-briques seulement après brick.hit()
    for (auto& new_brick : new_bricks)
    {
        pending_bricks.push_back(std::move(new_brick));
    }
}


bool Game::ball_hits_paddle(Ball& ball)
{
    if (ball.collision_paddle(paddle, true))
    {
        // direction nominale balle → paddle (cercle contre cercle)
        const Point diff = ball.getCircle().center - paddle.getCircle().center;
        const double dist = squared_norm(diff);
        const double epsil_squared = epsil_zero * epsil_zero;

        ball.undo_move();

        if (dist > epsil_squared)
        {
            const Point v= {ball.getDx(), ball.getDy()};
            const Point v_p= {paddle.getX() - paddle.getPrevX(), 0.0};

            const Point vn = (dot(v, diff) / dist) * diff;
            const Point vn_other = (dot(v_p, diff) / dist) * diff;
            const Point impulsion = 2.0 * (vn_other - vn);
            Point new_v = v+impulsion;

            double d = sqrt(squared_norm(new_v));
            if (d > delta_norm_max && d > epsil_zero)
            {
                new_v.x *= delta_norm_max / d;
                new_v.y *= delta_norm_max / d;
            }

            ball.set_delta(new_v.x, new_v.y);
        }
        else
        {
            ball.reverse_dy();
        }

        ball.move();
        return true;
    }
    return false;
}


bool Game::ball_hits_ball(Ball& ball)
{
    for (auto& b : balls)
    {
        if (&ball == b.get()) continue;

        if(ball.collision_ball(*b, true)){

            const Point diff = ball.getCircle().center - b->getCircle().center;
            const double dist = squared_norm(diff);
            const double epsil_squared = epsil_zero * epsil_zero;

            ball.undo_move();

            if (dist > epsil_squared)
            {
                const Point v= {ball.getDx(), ball.getDy()};
                const Point v_b= {b->getDx(),b->getDy()};

                const double vn = dot(v, diff) / dist;
                const double vn_other = dot(v_b, diff) / dist;

                const double r_autre = b->getR() * b->getR();
                const double r = ball.getR() * ball.getR();
                const double cte = 2 * r_autre / (r+r_autre);

                const double impulsion = cte * (vn_other-vn);
                Point new_v = v + impulsion * diff;
                Point new_v_b = v_b - impulsion * (r/r_autre) * diff;

                double d = sqrt(squared_norm(new_v));
                if (d > epsil_zero && d > delta_norm_max)
                {
                    new_v.x *= delta_norm_max / d;
                    new_v.y *= delta_norm_max / d;
                }
                double d_b = sqrt(squared_norm(new_v_b));
                if (d_b > epsil_zero && d_b > delta_norm_max)
                {
                    new_v_b.x *= delta_norm_max / d_b;
                    new_v_b.y *= delta_norm_max / d_b;
                }

                ball.set_delta(new_v.x, new_v.y);
                b->set_delta(new_v_b.x, new_v_b.y);
            }
            else
            {
                ball.reverse_dy();
            }
            ball.move();
            return true;
        }
    }
    return false;
}
