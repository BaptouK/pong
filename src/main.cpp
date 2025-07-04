#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>

std::mutex mutex;

float lerp(float a, float b, float t) {
    return a+t*(b-a);
}

void update_ball(sf::Vector2f* p1,sf::Vector2f* p2,sf::Vector2f* coord_ball, sf::Vector2f* velocity,sf::CircleShape* ball) {
    std::lock_guard<std::mutex> lock(mutex);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    if (coord_ball->y >= 590 || coord_ball->y <= 0) {
        velocity->y *= -1;
    }
    if (coord_ball->x <=10 && (coord_ball->y>=p1->y && coord_ball->y <= p2->y+180)) {
        velocity->x *= -1;
    }

    if (coord_ball->x >=980 && (coord_ball->y>=p2->y && coord_ball->y <= p2->y+180)) {
        velocity->x *= -1;
    }

    std::cout << p1->x << " " << p1->y << std::endl;
    std::cout << p2->x << " " << p2->y << std::endl;

    float t = 0.1f;
    sf::Vector2f current = *coord_ball;

    auto target =*velocity + *coord_ball;

    coord_ball->x = lerp(target.x, current.x, t);
    coord_ball->y = lerp(target.y, current.y, t);
}

void up_coord_ball(int &x,int &y, sf::Vector2f* coord_ball, sf::Vector2f* velocity) {
    
}

int main() {

    int win_x = 1000;
    int win_y = 600;

    sf::RenderWindow window(sf::VideoMode(win_x, win_y), "Change the size");
    window.setFramerateLimit(60);

    int pixel_by_tick{10};

    sf::RectangleShape background(sf::Vector2f(win_x, win_y));
    background.setPosition(sf::Vector2f(0, 0));
    background.setFillColor(sf::Color::Black);

    window.draw(background);

    sf::Vector2f coord_player1(1.f, 0.f);
    sf::RectangleShape player1(sf::Vector2f(15, 180));
    player1.setFillColor(sf::Color::Red);


    sf::RectangleShape player2(sf::Vector2f(15, 180));
    sf::Vector2f coord_player2(999.f-player2.getSize().x, 0.f);
    player2.setFillColor(sf::Color::Blue);

    sf::CircleShape ball(sf::CircleShape(15));
    sf::Vector2f coord_ball(sf::Vector2f(50,50));
    sf::Vector2f velocity (sf::Vector2f(10,10));
    ball.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z )&& coord_player1.y>0) coord_player1.y -= pixel_by_tick;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)&& coord_player1.y + player1.getSize().y <=600) coord_player1.y += pixel_by_tick;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)&& coord_player2.y >0) coord_player2.y -= pixel_by_tick;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)&& coord_player2.y + player2.getSize().y <=600) coord_player2.y += pixel_by_tick;

        std::thread coords_calculator(&update_ball,&coord_player1, &coord_player2,&coord_ball,&velocity,&ball);
        coords_calculator.detach();

        window.clear();

        player1.setPosition(coord_player1);
        player2.setPosition(coord_player2);

        ball.setPosition(coord_ball);

        window.draw(player1);
        window.draw(player2);
        window.draw(ball);

        window.display();
    }

    return 0;
}


