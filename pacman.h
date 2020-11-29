#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "game_field.h"
#include <string>

#include "constants.h"

#include <mutex>
recursive_mutex _lock;

class Pacman {
    
    bool mouthOpen = false;
    sf::Vector2f position;
    int speed = Constants::pacmanSpeed;    

    sf::Texture texture;

public:
    uint16_t direction = 0, directionPrev = 0;
    uint16_t mySize = 12, mouthSize = mySize / 2;
    uint16_t lifes = 3;
    bool lifeBreak = false;
    bool eatGhosts = false;
    unsigned int score = 0;

    
    int time_e = 0;
    int bonus_eat;
    void put_coord(size_t i, size_t j, const Field& f) {
        position.x = f.sx + i * f.siz;
        position.y = f.sy + j * f.siz + f.siz / 2;
    }
    void next_mov(Field& field) {
        auto cage = field.get_cage(position.x, position.y);
        if (direction == 3 && cage.second == 27) {
            position.x = field.sx + field.siz / 2;
            goto to;
        }
        if (direction == 1 && cage.second == 0) {
            position.x = field.sx + field.wid - field.siz / 2;
            goto to;
        }
        if (directionPrev == 3 && field.f[cage.first][cage.second + 1] == 0) {
            direction = 3;
            int delta = abs(position.y - field.stabley(position.y));
            position.y = field.stabley(position.y);
            position.x += delta;
            directionPrev = 0;
        }
        if (directionPrev == 1 && field.f[cage.first][cage.second - 1] == 0) {
            direction = 1;
            int delta = abs(position.y - field.stabley(position.y));
            position.y = field.stabley(position.y);
            position.x -= delta;
            directionPrev = 0;
        }
        if (directionPrev == 2 && field.f[cage.first + 1][cage.second] == 0) {
            direction = 2;
            int delta = abs(field.stablex(position.x) - position.x);
            position.x = field.stablex(position.x);
            position.y += delta;
            directionPrev = 0;
        }
        if (directionPrev == 4 && field.f[cage.first - 1][cage.second] == 0) {
            direction = 4;
            int delta = abs(field.stablex(position.x) - position.x);
            position.x = field.stablex(position.x);
            position.y -= delta;
            directionPrev = 0;
        }

        if (direction == 1) {
            cage.second--;
            if (field.f[cage.first][cage.second] == 0) {
                position.x = max(position.x - speed, static_cast<float>(field.sx) + mySize);
            }
        }
        if (direction == 2) {
            cage.first++;
            if (field.f[cage.first][cage.second] == 0) {
                position.y = min(position.y + speed, static_cast<float>(field.hei) - 2 * mySize);
            }
        }
        if (direction == 3) {
            cage.second++;
            if (field.f[cage.first][cage.second] == 0) {
                position.x = min(position.x + speed, static_cast<float>(field.wid) - mySize);
            }
        }
        if (direction == 4) {
            cage.first--;
            if (field.f[cage.first][cage.second] == 0) {
                position.y = max(position.y - speed, static_cast<float>(field.sy) + mySize);
            }
        }
    to:
        int u;
    }
    string name_file() {
        if (!mouthOpen || direction == 0)return "pac\\man1.png";
        if (direction == 1)return "pac\\man2.png";
        if (direction == 2)return "pac\\man3.png";
        if (direction == 3)return "pac\\man4.png";
        return "pac\\man5.png";
    }
    void sh() {
        mouthOpen = (mouthOpen + 1) % 2;
    }
    void draw(sf::RenderWindow& win) {
        texture.loadFromFile(name_file());
        texture.setSmooth(true);
        sf::CircleShape sprite(12.f);
        sprite.setTexture(&texture);
        sprite.setPosition({ (float)position.x - mySize, (float)position.y - mySize });
        win.draw(sprite);
    }

    void end_l() {
        lifes--; direction = 0;
        lifeBreak = true;
    }
    void key_board_1(Field& field) {
        _lock.lock();
        pair<int, int>c = field.get_cage(position.x, position.y);
        c.second--;
        if (c.second == -1) {
            c.second = 27;
        }
        if (field.f[c.first][c.second] == 0) {
            direction = 1;
        }
        directionPrev = 1;
        _lock.unlock();
    }
    void key_board_3(Field& field) {
        _lock.lock();
        auto cage = field.get_cage(position.x, position.y);
        cage.second++;
        if (cage.second == 28) {
            cage.second = 0;
        }
        if (field.f[cage.first][cage.second] == 0) {
            direction = 3;
        }
        directionPrev = 3;
        _lock.unlock();
    }
    void key_board_2(Field& field) {
        _lock.lock();
        auto cage = field.get_cage(position.x, position.y);
        cage.first++;
        if (field.f[cage.first][cage.second] == 0) {
            direction = 2;
        }
        directionPrev = 2;
        _lock.unlock();
    }
    void key_board_4(Field& field) {
        _lock.lock();
        auto cage = field.get_cage(position.x, position.y);
        cage.first--;
        if (field.f[cage.first][cage.second] == 0) {
            direction = 4;
        }
        directionPrev = 4;
        _lock.unlock();
    }

    int x() {
        return position.x;
    }
    int y() {
        return position.y;
    }
};

