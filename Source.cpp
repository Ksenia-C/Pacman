#include <time.h>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <set>
#include <iostream>
#include <Windows.h>
#include <thread>
#include <conio.h>
#include <vector>
#include <string>
#include "pacman.h"
#include "game_field.h"
#include "bonus.h"
#include "ghosts.h"
#include <SFML/Graphics/Text.hpp>
using namespace std;
/*
		4
		|
		|
		|
 1------0------3
		|
		|
		|
		2
*/

/*
   4 3
	O
   2 1
*/


#include <mutex>
//recursive_mutex _lock;

Pacman man;
Field field;
vector<Ghost> ghost(4);


vector<pair<int, int>>scatter = {
	{0, 25},
	{0, 3},
	{35, 27},
	{35, 0},
};
auto func_m = []() {
	while (field.cnt && man.lifes > 0) {
		if (man.lifeBreak)continue;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			man.key_board_1(field);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			man.key_board_3(field);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			man.key_board_2(field);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			man.key_board_4(field);
		}
	}
};
auto func_d = []() {
	sf::RenderWindow window(sf::VideoMode(field.wid, field.hei), "Pacman");
	window.setActive(false);
	sf::Font font;
	font.loadFromFile("times.ttf");
	int last_score = 0;
	field.show(window);
	while (window.isOpen() && field.cnt && man.lifes > 0) {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		field.show(window);
		man.draw(window);
		for (int i = 0; i < 4; i++) {
			if (!man.lifeBreak && ghost[i].pr(man.x(), man.y(), field)) {
				if (!ghost[i].ending && (!man.eatGhosts || !ghost[i].scar)) {
					man.end_l();
					for (int j = 0; j < 4; j++)ghost[i].mov = 0;
				} else {
					if (ghost[i].ending) continue;
					man.score += man.bonus_eat;
					man.bonus_eat *= 2;
					ghost[i].scar = 0;
					ghost[i].ending = 1;
					ghost[i].x = field.stablex(ghost[i].x);
					ghost[i].y = field.stablex(ghost[i].y);
					ghost[i].speed = 12;
					ghost[i].to_home(field);
				}
			}
		}
		if (!man.lifeBreak) {
			for (int i = 0; i < 4; i++) {
				ghost[i].show(window);
				ghost[i].sh();
				ghost[i].next_mov(field);
			}
		} else {
			if (man.lifes > 0) {
				sf::RectangleShape r(sf::Vector2f(310.f, 40.f));
				r.setPosition((float)field.wid / 4, (float)field.hei / 4);
				r.setFillColor(sf::Color::Black);
				r.setOutlineThickness(2);
				r.setOutlineColor(sf::Color::Cyan);
				sf::Text text("YOU'VE LOST YOUR LIFE", font);
				text.setStyle(sf::Text::Bold);
				text.setCharacterSize(24);
				text.setFillColor(sf::Color::Cyan);
				text.setPosition((float)field.wid / 4, (float)field.hei / 4);
				window.draw(r);
				window.draw(text);
			}
		}
		sf::Text sc("your score is " + to_string(man.score), font);
		sc.setCharacterSize(24);
		sc.setFillColor(sf::Color::White);
		sc.setPosition(0, 0);
		window.draw(sc);
		for (int i = 0; i < man.lifes; i++) {
			sf::CircleShape lifes(12.f);
			lifes.setPosition((float)field.siz * 2 * i, (float)field.hei - 3 * man.mySize);
			sf::Texture texture;
			texture.loadFromFile("pac\\man2.png");
			texture.setSmooth(true);
			lifes.setTexture(&texture);
			window.draw(lifes);
		}
		window.display();
		man.sh();
		pair<int, int> res = field.eat_bonus(man.x(), man.y());
		man.score += res.first;
		if (res.second) {
			_lock.lock();
			if (!man.eatGhosts)man.eatGhosts = true;
			for (int i = 0; i < 4; i++) {
				if (!ghost[i].ending) {
					ghost[i].routine = 0;
					ghost[i].scar = 1;
					ghost[i].speed = 4;
					ghost[i].t_scar = clock();
				}
			}
			man.bonus_eat = 200;
			man.time_e = clock();
			_lock.unlock();
		}
		man.next_mov(field);
		Sleep(40);
	}
	window.clear(sf::Color::Black);
	field.show(window);
	man.draw(window);
	sf::Text sc("your score is " + to_string(man.score), font);
	sc.setCharacterSize(24);
	sc.setFillColor(sf::Color::White);
	sc.setPosition(0, 0);
	window.draw(sc);
	sf::RectangleShape r(sf::Vector2f(170.f, 40.f));
	r.setPosition((float)field.wid / 3, (float)field.hei / 3);
	r.setFillColor(sf::Color::Black);
	r.setOutlineThickness(2);
	r.setOutlineColor(sf::Color::Cyan);
	sf::Text text;
	text.setFont(font);
	if (man.lifes == 0) {
		text.setString("GAME  OVER");
	} else {
		text.setString("YOU'VE WIN");
	}
	text.setStyle(sf::Text::Bold);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Cyan);
	text.setPosition((float)field.wid / 3, (float)field.hei / 3);
	window.draw(r);
	window.draw(text);
	window.display();
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
};


auto func_g = []() {
	int t_start = clock();
	int ind = 0;
	vector<int> t_t = { 7, 20, 7, 20, 5, 20, 5, 24 * 3600 };
	while (field.cnt && man.lifes > 0) {
		_lock.lock();
		if (man.lifeBreak) {
			ind = 0;
			t_start = clock();
		} else if (man.eatGhosts) {
			if (clock() - man.time_e > 8 * CLK_TCK) {
				man.eatGhosts = 0;
				for (int i = 0; i < 4; i++) {
					if (!ghost[i].ending) {
						ghost[i].routine = ind % 2 + 1;
						ghost[i].scar = 0; ghost[i].speed = 7;
					}
				}
				t_start = clock() - (man.time_e - t_start);
			}
		}
		if (!man.eatGhosts && clock() - t_start >= t_t[ind] * CLK_TCK) {
			ind++;
			t_start = clock();
		}
		for (int i = 0; i < 4; i++) {
			if (ghost[i].ending)continue;
			if (ghost[i].scar)continue;
			if (ghost[i].home != 0)continue;
			if (ghost[i].routine != 1 + ind % 2) {
				ghost[i].routine = 1 + ind % 2;
				//cout << "ghost " << i << " begin " << (1 + ind % 2)<<'\n';
			}
			if (ind % 2 == 0) {
				ghost[i].iw = scatter[i].first;
				ghost[i].jw = scatter[i].second;
			} else {
				ghost[i].chase(ghost[0], man, field);
			}
		}
		_lock.unlock();
	}
};
int main() {
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	srand(time(NULL));
	for (int i = 0; i < 4; i++) ghost[i].id = i;
	ghost[2].start = 30;
	ghost[3].start = 60;
	field.init_f();
	field.init_bonus();
	man.put_coord(14, 26, field);
	ghost[0].put_coord(14, 14, field);
	ghost[1].put_coord(14, 17, field);
	ghost[2].put_coord(12, 17, field);
	ghost[3].put_coord(16, 17, field);
	thread thread_for_draw(func_d);
	thread_for_draw.detach();
	thread thread_for_mov(func_m);
	thread_for_mov.detach();
	thread thread_for_ghost(func_g);
	thread_for_ghost.detach();
	while (field.cnt && man.lifes > 0) {
		if (man.lifeBreak) {
			Sleep(2500);
			ghost[2].start = 7;
			ghost[3].start = 17;
			man.put_coord(14, 26, field);
			ghost[0].put_coord(14, 14, field);
			ghost[1].put_coord(14, 17, field);
			ghost[2].put_coord(12, 17, field);
			ghost[3].put_coord(16, 17, field);
			man.lifeBreak = 0;
			for (int i = 0; i < 4; i++) {
				ghost[i].mov = 1;
			}
		}
	}
	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {}
}