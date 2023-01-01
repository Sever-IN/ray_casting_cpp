#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
// #include <set>
// #include "fps.cpp"

int main()
{
// FPS = 60
// X = WIDTH / len(text_map[0])
// Y = HEIGHT / len(text_map)
// 
// FOV = math.pi / 2
// HALF_FOV = FOV / 2
// NUM_RAYS = 480
// MAX_DEPTH = 800
// DELTA_ANGLE = FOV / NUM_RAYS
// DIST = NUM_RAYS / (2 * math.tan(HALF_FOV))
// PROJ_COEFF = 2 * DIST * X
// SCALE = WIDTH // NUM_RAYS
// 
// player_pos = (WIDTH/2, HEIGHT/2)
// player_angle = 0
// player_speed = 2
// 
    int w = 480;
    int h = 320;
    int hw = w/2;
    int hh = h/2;
    int fps = 60;
    float FOV = M_PI / 2;
    float hf = FOV / 2;
    int sx = 12;
    int sy = 8;
    float X = w / sx;
    float Y = h / sy;
    int NUM_RAYS = 480;
    int MAX_DEPTH = 512;
    float DELTA_FOV = FOV / 2;
    float DELTA_ANGLE = FOV / NUM_RAYS;
    float DIST = NUM_RAYS / (2 * tan(hf));
    float PROJ_COEFF = 2 * DIST * X;
    int SCALE = w / NUM_RAYS;
    sf::Vector2f player_pos(hw, hh);
    float player_angle = 0;
    float player_speed = 2.0;
    float sin_a = 0.0;
    float cos_a = 0.0;
    bool map[12][8] = {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,1,1,0,0,1},
        {1,0,1,0,0,0,0,1},
        {1,0,0,1,0,0,0,1},
        {1,1,0,0,1,0,0,1},
        {1,0,0,0,1,0,1,1},
        {1,0,0,0,0,0,0,1},
        {1,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,1,1},
        {1,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1}
    };
    // bool map[12][8] = {
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,0,0,0,0,0}
    // };
    sf::Vector2i world[sx*sy];
    // std::cout << std::to_string(world[0]) << std::endl;
    for (int i = 0; i < sx*sy; i++) {
        int a = i % sx;
        int b = i / sx;
        // std::cout << std::to_string(a) << std::endl;
        if (map[a][b])
            world[i] = sf::Vector2i(a, b);
    }
    // for (int a = 1; a <= 16; a++) {
    //     for (int b = 1; b <= 12; b++) {
    //         // if (map[a][b]) {
    //         world[a*b] = sf::Vector2f( a * X, b * Y );
    //             // world[a][b];
    //             // world[a][b][1] = b * Y;
    //         // }
    //     }
    // }

    sf::RenderWindow window(sf::VideoMode(w, h), "game", sf::Style::Titlebar | sf::Style::Close);
    // window.setFramerateLimit(60);
    // getchar();

    sf::Texture texture;
    texture.create(w, h);

    sf::Sprite sprite(texture);
    // auto shape = sf::RectangleShape{ sf::Vector2f{ window.getSize() }};

    sf::Shader shader;
    shader.loadFromFile("ray_casting.frag", sf::Shader::Fragment);
    shader.setUniform("resolution", sf::Vector2f(w, h));

    sf::Clock clock;
    clock.restart();
    float last = 0;

    // FPS fps;
    sf::Font font;
    font.loadFromFile("calibri.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(25);
    text.setFillColor(sf::Color::Green);
    text.setPosition(20, 20);

    sf::CircleShape shape(12);
    shape.setFillColor(sf::Color(150, 50, 250));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            // if (Events::pre)
            if (event.type == sf::Event::KeyPressed) {
            sin_a = sin(player_angle);
            cos_a = cos(player_angle);
            if (event.key.code == sf::Keyboard::W) {
                player_pos.x += player_speed * cos_a;
                player_pos.y += player_speed * sin_a;
            }
            if (event.key.code == sf::Keyboard::S) {
                player_pos.x -= player_speed * cos_a;
                player_pos.y -= player_speed * sin_a;
            }
            if (event.key.code == sf::Keyboard::A) {
                player_pos.x += player_speed * sin_a;
                player_pos.y -= player_speed * cos_a;
            }
            if (event.key.code == sf::Keyboard::D) {
                player_pos.x -= player_speed * sin_a;
                player_pos.y += player_speed * cos_a;
            }
            if (event.key.code == sf::Keyboard::Left) player_angle -= 0.2;
            if (event.key.code == sf::Keyboard::Right) player_angle += 0.2;
            }

        //     if (event.type == sf::Event::Resized)
        //         doSome.close();
        }
        shape.setPosition(player_pos - sf::Vector2f(12.0, 12.0));
        sf::Vertex line[] =
        {
            sf::Vertex(player_pos),
            sf::Vertex(sf::Vector2f(player_pos.x + w * cos(player_angle), player_pos.y + w * sin(player_angle)))
        };

        shader.setParameter("time", clock.getElapsedTime().asSeconds());
        
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        shader.setParameter("mouse", sf::Vector2f(mouse.x, mouse.y));
        text.setString(std::to_string( 1 / (clock.getElapsedTime().asSeconds() - last) ));
        last = clock.getElapsedTime().asSeconds();

        window.clear();
        // window.draw(sprite, &shader);
        window.draw(shape);
        // window.draw(line, 2, sf::Lines);


        sf::VertexArray lines(sf::Lines, NUM_RAYS*2 /*количество точек*/);

        float cur_angle = player_angle - FOV / 2;
        for (int ray = 0; ray < NUM_RAYS; ray++) {
            float sina = sin(cur_angle);
            float cosa = cos(cur_angle);
            float x = 0;
            float y = 0;
            for (int depth = 1; depth <= MAX_DEPTH; depth+=4) {
                x = player_pos.x + depth * cosa;
                y = player_pos.y + depth * sina;
                bool e = 0;
                sf::Vector2i vec(x/X, y/Y);
                for (int i = 0; i < 96; i++) {
                    if (vec == world[i]) {
                        e = 1;
                        break;
                    }
                }
                if (e == 1) {
                    break;
                }
                // if (std::find(world, world+96, sf::Vector2f(x, y)))
                    // std::cout << std::to_string(x) << std::endl; 
                    // break;
                // x = depth;
                // y = depth-1;
            }
            // std::cout << std::to_string(x) << std::endl;
            // std::cout << std::to_string(x) << std::endl;
            // std::cout << std::to_string(ray) << std::endl;
            lines[ray*2] = sf::Vertex(player_pos);
            lines[ray*2+1] = sf::Vertex(sf::Vector2f(x, y));
            cur_angle += DELTA_ANGLE;
        }

        sf::RectangleShape rectangle(sf::Vector2f(X, Y));
        rectangle.setFillColor(sf::Color(128,128,128,255));
        rectangle.setOutlineColor(sf::Color(0,255,0,255));

        for (int i = 0; i < 96; i++) {
            if (world[i] != sf::Vector2i(0, 0)) {
                rectangle.setPosition(sf::Vector2f(world[i].x*X, world[i].y*Y));
                window.draw(rectangle);
            }
        }
        line->color = sf::Color(0,255,0);
        window.draw(lines);
        window.draw(line, 2, sf::Lines);

        window.draw(text);


        window.display();

    }

    return 0;
}