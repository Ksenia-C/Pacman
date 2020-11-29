#pragma once
#include <iostream>
#include <Windows.h>

class Food {
public:
	unsigned int type = 0;
	unsigned int award = 0;
	Food() {}
	Food(unsigned int t) {
		if (t == 1) {
			type = 1;
			award = 10;
		} else if (t == 2) {
			type = 2;
			award = 50;
		} else {
			type = 0;
			award = 0;
		}
	}
	void show(sf::RenderWindow& win, const int X, const int Y, const int siz) {
		if (type == 0) return;
		if (type == 1) {
			sf::RectangleShape it(sf::Vector2f(6.f, 6.f));
			it.setFillColor(sf::Color(255, 255, 255));
			it.setPosition({ (float)X + siz / 4, (float)Y + siz / 4 });
			win.draw(it);
		} else if (type == 2) {
			sf::CircleShape it(10.f);
			it.setFillColor(sf::Color(255, 255, 255));
			it.setPosition({ (float)X - siz / 8, (float)Y - siz / 8 });
			win.draw(it);
		}
	}
};
