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
    int sx = 18;
    int sy = 12;
    float X = float (w) / float (sx);
    float Y = float (h) / float (sy);
    int NUM_RAYS = w;
    int MAX_DEPTH = 512;
    int DEPTH = 16;
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
    int world[sx][sy];
    // bool map[sx][sy] = {
    //     {1,1,1,1,1,1,1,1,1,1,1,1},
    //     {1,0,0,0,0,0,0,0,0,0,0,1},
    //     {1,0,0,1,1,0,0,0,0,0,0,1},
    //     {1,0,1,0,0,0,0,1,0,0,0,1},
    //     {1,0,0,1,0,0,0,0,0,0,0,1},
    //     {1,1,0,0,1,0,0,0,0,0,0,1},
    //     {1,0,0,0,1,0,1,0,0,1,0,1},
    //     {1,0,0,0,0,0,0,0,0,0,0,1},
    //     {1,0,1,0,0,0,0,0,0,0,0,1},
    //     {1,0,0,0,0,0,1,0,0,0,0,1},
    //     {1,0,0,1,0,0,0,0,0,0,0,1},
    //     {1,0,0,0,0,0,1,0,0,0,0,1},
    //     {1,0,0,0,0,0,0,0,1,0,0,1},
    //     {1,0,0,0,1,0,0,0,0,1,0,1},
    //     {1,0,0,0,0,0,0,0,0,1,0,1},
    //     {1,0,0,0,1,0,0,0,0,0,0,1},
    //     {1,0,0,0,0,0,0,0,0,0,0,1},
    //     {1,1,1,1,1,1,1,1,1,1,1,1}
    // };

    // sf::Vector2i world[sx*sy];
    // std::cout << std::to_string(world[0]) << std::endl;
    for (int i = 0; i < sx*sy; i++) {
        int a = i % sx;
        int b = i / sx;
        // std::cout << std::to_string(a) << std::endl;
        // if (map[a][b])
            world[a][b] = rand() % 9;
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

        // Отрисовка

        window.clear();
        // window.draw(sprite, &shader);
        window.draw(shape);
        // window.draw(line, 2, sf::Lines);


        sf::VertexArray lines(sf::Lines, NUM_RAYS*2 /*количество точек*/);
        // sf::RectangleShape line(sf::Vector2f(150, 5));

        float ox = player_pos.x;
        float oy = player_pos.y;
        float fx, fy, vx, vy, hx, hy, x, y, depth;
        int dx, dy;
        sf::RectangleShape l;

        float cur_angle = player_angle - FOV / 2;
        for (int ray = 0; ray < NUM_RAYS; ray++) {

            float sin_a = sin(cur_angle);
            float cos_a = cos(cur_angle);
            sin_a = (sin_a) ? sin_a : 0.000001;
            cos_a = (cos_a) ? cos_a : 0.000001;
            float tan_a = sin_a/cos_a;
            float cot_a = cos_a/sin_a;
            // dx, fx = (cos_a >= 0) ? (1, X - fmod(ox, X)) : (-1, fmod(ox, X));
            if (cos_a >= 0) {
                fx = X - fmod(ox, X);
                dx = 1;
            } else {
                fx = fmod(ox, X);
                dx = -1;
            }
            for (int i = 0; i < DEPTH; i++) {
                vx = ox + (fx + i * X + 0.001) * dx;
                vy = oy + (tan_a * fx + i * tan_a * Y) * dx;
                if ((0 <= vx / X < sx) && (0 <= vy / Y < sy)) {
                    if (world[std::max(0, std::min(int (vx / X), sx-1))][std::max(0, std::min(int (vy / Y), sy-1))] >= 6) break;
                }
            }

            if (sin_a >= 0) {
                fy = Y - fmod(oy, Y);
                dy = 1;
            } else {
                fy = fmod(oy, Y);
                dy = -1;
            }
            for (int i = 0; i < DEPTH; i++) {
                hx = ox + (cot_a * fy + i * cot_a * X) * dy;
                hy = oy + (fy + i * Y + 0.001) * dy;
                if ((0 <= hx / X < sx) && (0 <= hy / Y < sy)) {
                    // std::cout << std::to_string(sx) << std::endl;
                    if (world[std::max(0, std::min(int (hx / X), sx-1))][std::max(0, std::min(int (hy / Y), sy-1))] >= 6) break;
                }
            }
            float depth_v = sqrt(pow(vx - ox, 2) + pow(vy - oy, 2));
            float depth_h = sqrt(pow(hx - ox, 2) + pow(hy - oy, 2));
            // float depth = (depth_v < depth_h) ? depth_v : depth_h;
            if (depth_v < depth_h) {
                depth = depth_v;
                x = vx;
                y = vy;
            } else {
                depth = depth_h;
                x = hx;
                y = hy;
            }
            float c = 255 / (1 + depth * depth * 0.0001);
            depth *= cos(player_angle - cur_angle);
            // std::cout << std::to_string(vx) << std::endl;
            lines[ray*2] = sf::Vertex(player_pos);
            lines[ray*2+1] = sf::Vertex(sf::Vector2f(x, y));
            // std::cout << std::to_string(ray) << std::endl;
            cur_angle += DELTA_ANGLE;

            float proj_height = PROJ_COEFF / std::max(0.1f, depth);
            l.setPosition(sf::Vector2f(ray * SCALE, h/2 - proj_height/2));
            l.setSize(sf::Vector2f(SCALE, proj_height));
            l.setFillColor(sf::Color(c, c, c, 255.f));
            window.draw(l);


        }
        // for (int ray = 0; ray < NUM_RAYS; ray++) {
        //     float sin_a = sin(cur_angle);
        //     float cos_a = cos(cur_angle);
        //     float x = 0;
        //     float y = 0;
        //     for (int depth = 1; depth <= MAX_DEPTH; depth+=4) {
        //         x = player_pos.x + depth * cos_a;
        //         y = player_pos.y + depth * sin_a;
        //         bool e = 0;
        //         sf::Vector2i vec(x/X, y/Y);
        //         for (int i = 0; i < 96; i++) {
        //             if (vec == world[i]) {
        //                 e = 1;
        //                 break;
        //             }
        //         }
        //         if (e == 1) {
        //             break;
        //         }
        //         // if (std::find(world, world+96, sf::Vector2f(x, y)))
        //             // std::cout << std::to_string(x) << std::endl; 
        //             // break;
        //         // x = depth;
        //         // y = depth-1;
        //     }
        //     // std::cout << std::to_string(x) << std::endl;
        //     // std::cout << std::to_string(x) << std::endl;
        //     // std::cout << std::to_string(ray) << std::endl;
        //     lines[ray*2] = sf::Vertex(player_pos);
        //     lines[ray*2+1] = sf::Vertex(sf::Vector2f(x, y));
        //     cur_angle += DELTA_ANGLE;
        // }

        
        sf::Vertex lineb[] =
        {
            sf::Vertex(player_pos),
            sf::Vertex(sf::Vector2f(vx, vy))
        };
        sf::RectangleShape rectangle(sf::Vector2f(X, Y));
        rectangle.setFillColor(sf::Color(128,128,128,255));
        rectangle.setOutlineColor(sf::Color(0,255,0,255));
        for (int i = 0; i < sx*sy; i++) {
            int a = i % sx;
            int b = i / sx;
            if (world[a][b] >= 6) {
                rectangle.setPosition(sf::Vector2f(a*X, b*Y));
                // window.draw(rectangle);
            }
        }
        
        
        line->color = sf::Color(0,255,0);
        // window.draw(lines);
        window.draw(line, 2, sf::Lines);

        window.draw(text);


        window.display();

    }

    return 0;
}