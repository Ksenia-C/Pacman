#pragma once
#ifndef _ghosts_h_
#define _ghosts_h_

#include <iostream>
#include <Windows.h>
#include<vector>
#include "game_field.h"
#include "constants.h"
using namespace std;

class Ghost {
public:
	int home = 0;
	int start = 0;
	int iw, jw;
	int x, y, siz = 12;
	int speed = Constants::pacmanSpeed;
	int id;
	int routine = 0;
	int mov = 1;
	int wave = 1;
	int ending = 0;
	int scar = 0;
	string name;
	int t_scar;
	void show(sf::RenderWindow& win) {
		if (mov == 0)return;
		if (ending) {
			name = "gho\\gh6_1.png";
			name[8] += mov - 1;
		} else if (scar) {
			if (scar % 2 || clock() - t_scar < 6 * CLK_TCK) {
				name = "gho\\gh4-1.png";
			} else {
				name = "gho\\gh5-1.png";
			}
			name[8] += wave % 2;
			scar++;
		} else {
			name = "gho\\gh0-1_1.png";
			name[6] += id; name[8] += wave % 2; name[10] += mov - 1;
		}
		sf::Texture texture;
		sf::CircleShape it(12.f);
		texture.loadFromFile(name);
		texture.setSmooth(true);
		it.setTexture(&texture);
		it.setPosition({ (float)x - siz, (float)y - siz });
		win.draw(it);

	}
	void put_coord(int i, int j, Field& f) {
		x = f.sx + i * f.siz;
		y = f.sy + j * f.siz + f.siz / 2;
	}
	void sh() {
		wave = (wave + 1) % 2;
	}
	void next_mov(Field& f) {
		int xt = f.stablex(x);
		int yt = f.stabley(y);
		pair<int, int>c = f.get_cage(x, y);
		if (c.first > 15 && c.first < 19 &&
			c.second>10 && c.second < 17) {
			if (home == 0)home = 1;
			ending = 0; scar = 0;
			speed = 7;
			if (244 - f.cnt >= start) {
				out_of_home(f);
			} else if (clock() - f.t_nothing >= 4 * CLK_TCK) {
				out_of_home(f);
				f.t_nothing = clock();
			} else {
				routine = 0;
			}
		} else {
			if (home > 0)routine = 0;
			home = 0;
		}

		if (home == 1 && c.first == iw &&
			c.second == jw) {
			iw -= 3; home = 2;
		}
		if (ending && c.first == iw &&
			c.second == jw) {
			iw += 3; home = 3;
		}
		if (home == 2) {
			y -= speed;
			mov = 4;
			return;
		}
		if (home == 3) {
			y += speed;
			mov = 2;
			return;
		}
		vector<int>des;
		if (mov == 1) {
			if (x - speed < xt && x >= xt) {
				if (c.second == 0) {
					x = f.siz * 27 + f.siz / 2 + f.sx;
					speed = 4;
					return;
				}
				if (f.f[c.first - 1][c.second] == 0)des.push_back(4);
				if (f.f[c.first][c.second - 1] == 0)des.push_back(1);
				if (f.f[c.first + 1][c.second] == 0)des.push_back(2);
			}
		} else if (mov == 3) {
			if (x + speed > xt&& x <= xt) {
				if (c.second == 27) {
					x = f.siz / 2 + f.sx;
					speed = 4;
					return;
				}
				if (f.f[c.first - 1][c.second] == 0)des.push_back(4);
				if (f.f[c.first + 1][c.second] == 0)des.push_back(2);
				if (f.f[c.first][c.second + 1] == 0)des.push_back(3);
			}
		} else if (mov == 2) {
			if (y + speed > yt&& y <= yt) {
				if (f.f[c.first][c.second - 1] == 0)des.push_back(1);
				if (f.f[c.first + 1][c.second] == 0)des.push_back(2);
				if (f.f[c.first][c.second + 1] == 0)des.push_back(3);
			}
		} else if (mov == 4) {
			if (y - speed < yt && y >= yt) {
				if (f.f[c.first - 1][c.second] == 0)des.push_back(4);
				if (f.f[c.first][c.second - 1] == 0)des.push_back(1);
				if (f.f[c.first][c.second + 1] == 0)des.push_back(3);
			}
		}
		vector<int>a = { 0, 1, 0, -1 };
		int mov_old = mov;
		if (des.size() == 0)goto to_mot;
		if (des.size() == 1) {
			mov = des[0];
		} else {
			if (routine == 0) {
				mov = des[rand() % des.size()];
			} else {
				int ind = -1;
				for (size_t i = 0; i < des.size(); i++) {
					if (ind == -1) {
						ind = i; continue;
					}
					if (abs(c.first + a[des[i] - 1] - iw) + abs(c.second + a[-des[i] + 4] - jw) <
						abs(c.first + a[des[ind] - 1] - iw) + abs(c.second + a[-des[ind] + 4] - jw)) {
						ind = i;
					}
				}
				mov = des[ind];
			}
		}
	to_mot:
		if (mov != mov_old) {
			x = xt, y = yt;
		}
		if (mov != 0) {
			x += a[4 - mov] * speed;
			y += a[mov - 1] * speed;
		}
	}
	void out_of_home(Field& field) {
		iw = 17; jw = 14;
		routine = 1;
	}
	void to_home(Field& field) {
		iw = 14; jw = 14;
		routine = 1;
	}
	void chase(Ghost& red, Pacman& man, Field& field) {
		if (ending)return;
		pair<int, int>c = field.get_cage(man.x(), man.y());
		if (id == 0) {
			iw = c.first;
			jw = c.second;
		} else if (id == 1) {
			if (man.direction == 1) {
				iw = c.first;
				jw = max(0, c.second - 4);
			} else if (man.direction == 2) {
				iw = min(35, c.first + 4);
				jw = c.second;
			} else if (man.direction == 3) {
				iw = c.first;
				jw = min(27, c.second + 4);
			} else if (man.direction == 4) {
				iw = max(0, c.first - 4);
				jw = c.second;
			} else {
				iw = c.first;
				jw = c.second;
			}
		} else if (id == 2) {
			if (man.direction == 1) {
				c.second = max(0, c.second - 2);
			} else if (man.direction == 2) {
				c.first = min(35, c.first + 2);
			} else if (man.direction == 3) {
				c.second = min(27, c.second + 2);
			} else if (man.direction == 4) {
				c.first = max(0, c.first - 2);
			}
			pair<int, int>cr = field.get_cage(red.x, red.y);
			iw = cr.first + 2 * (c.first - cr.first);
			jw = cr.second + 2 * (c.second - cr.second);
		} else if (id == 3) {
			pair<int, int>cm = field.get_cage(x, y);
			if (abs(cm.first - c.first) + abs(cm.second - c.second) > 8) {
				iw = c.first;
				jw = c.second;
			} else {
				iw = 35;
				jw = 0;
			}
		}
	}
	bool pr(int X, int Y, Field field) {
		pair<int, int>c = field.get_cage(X, Y);
		pair<int, int>cm = field.get_cage(x, y);
		return c.first == cm.first && c.second == cm.second;
	}
};
#endif