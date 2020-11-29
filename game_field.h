#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <set>
#include "bonus.h"

using namespace std;
class Field {
public:
	int sx = 0, sy = 0;
	int wid = 560, hei = 720;
	int siz = 20;
	int cnt = 244;
	int t_nothing;
	vector<vector<int>>f;
	vector<vector<Food>>table;
	set<pair<int, int>>redraw;
	void init_f() {
		f.resize(36);
		for (int i = 0; i < 36; i++) {
			f[i].resize(14);
		}
		f[3] = { 4, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 3 };
		f[4][0] = f[4][13] = 100;
		f[5] = { 100, 0, 4, 10, 10, 3, 0, 4, 10, 10, 10, 3, 0, 100 };
		f[6] = { 100, 0, 100, 0, 0, 100, 0, 100, 0, 0, 0, 100, 0, 100 };
		f[7] = { 100, 0, 2, 10, 10, 1, 0, 2, 10, 10, 10, 1, 0, 2 };
		f[8][0] = 100;
		f[9] = { 100, 0, 4, 10, 10, 3, 0, 4, 3, 0, 4, 10, 10, 10 };
		f[10] = { 100, 0, 2, 10, 10, 1, 0, 100, 100, 0, 2, 10, 10, 3 };
		f[11][0] = f[11][7] = f[11][8] = f[11][13] = 100;
		f[12] = { 2, 10, 10, 10, 10, 3, 0, 100, 2, 10, 10, 3, 0, 100 };
		f[13] = { 0, 0, 0, 0, 0, 100, 0, 100, 4, 10, 10, 1, 0, 2 };
		f[14][5] = f[14][7] = f[14][8] = 100;
		f[15][5] = f[15][7] = f[15][8] = 100; f[15][13] = 21; f[15][12] = f[15][11] = 10; f[15][10] = 4;
		f[16] = { 10, 10, 10, 10, 10, 1, 0, 2, 1, 0, 100, 0, 0, 0 };
		f[17][10] = 100;
		f[18] = { 10, 10, 10, 10, 10, 3, 0, 4, 3, 0, 100, 0, 0, 0 };
		f[19][5] = f[19][7] = f[19][8] = 100; f[19][13] = f[19][12] = f[19][11] = 10; f[19][10] = 2;
		f[20][5] = f[20][7] = f[20][8] = 100;
		f[21][5] = f[21][7] = f[21][8] = 100; f[21][13] = f[21][12] = f[21][11] = 10; f[21][10] = 4;
		f[22] = { 4, 10, 10, 10, 10, 1, 0, 2, 1, 0, 2, 10, 10, 3 };
		f[23][0] = f[23][13] = 100;
		f[24] = { 100, 0, 4, 10, 10, 3, 0, 4, 10, 10, 10, 3, 0, 100 };
		f[25] = { 100, 0, 2, 10, 3, 100, 0, 2, 10, 10, 10, 1, 0, 2 };
		f[26][0] = f[26][4] = f[26][5] = 100;
		f[27] = { 2, 10, 3, 0, 100, 100, 0, 4, 3, 0, 4, 10, 10, 10 };
		f[28] = { 4, 10, 1, 0, 2, 1, 0, 100, 100, 0, 2, 10, 10, 3 };
		f[29][0] = f[29][7] = f[29][8] = f[29][13] = 100;
		f[30] = { 100, 0, 4, 10, 10, 10, 10, 1, 2, 10, 10, 3, 0, 100 };
		f[31] = { 100, 0, 2, 10, 10, 10, 10, 10, 10, 10, 10, 1, 0, 2 };
		f[32][0] = 100;
		f[33] = { 2, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
		for (int i = 0; i < 36; i++) {
			for (int j = 13; j >= 0; j--) {
				int k = f[i][j];
				if (k == 1)k = 2;
				else if (k == 2)k = 1;
				else if (k == 3)k = 4;
				else if (k == 4)k = 3;
				f[i].push_back(k);
			}
		}

	}
	void show(sf::RenderWindow& win) {
		sf::Texture texture;
		sf::RectangleShape it(sf::Vector2f(560.f, 720.f));
		texture.loadFromFile("pol\\pol.png");
		texture.setSmooth(true);
		it.setTexture(&texture);
		win.draw(it);
		for (int i = 0; i < 36; i++) {
			for (int j = 0; j < 28; j++) {
				table[i][j].show(win, sx + j * siz, sy + i * siz, siz);
			}
		}
	}
	pair<int, int> get_cage(int x, int y) {
		return { (y - sy) / siz, (x - sx) / siz };
	}
	int stablex(int c) {
		int p = (c - sx) / siz;
		return siz * p + siz / 2;
	}
	int stabley(int c) {
		int p = (c - sy) / siz;
		return siz * p + siz / 2;
	}
	void init_bonus() {
		table.resize(36);
		for (int i = 0; i < 36; ++i) table[i].resize(28);
		for (int i = 4; i < 12; ++i) {
			int kol = 0;
			for (int j = 0; j < 28; ++j) {
				if (f[i][j] == 100)kol++;
				if (f[i][j] == 0 && kol % 2 == 1) table[i][j] = Food(1);
			}
		}
		for (int i = 23; i < 33; ++i) {
			for (int j = 0; j < 28; ++j) {
				if (f[i][j] == 0)table[i][j] = Food(1);
			}
		}
		for (int i = 12; i < 23; ++i) {
			table[i][6] = Food(1);
		}
		for (int i = 12; i < 23; ++i) {
			table[i][21] = Food(1);
		}
		table[26][13] = table[26][14] = Food(0);
		table[6][1] = table[6][26] = table[26][1] = table[26][26] = Food(2);
	}
	pair<int, bool> eat_bonus(int x, int y) {
		pair<int, int>c = get_cage(x, y);
		int u = table[c.first][c.second].award;
		if (table[c.first][c.second].type != 0) {
			cnt--;
			t_nothing = clock();
		}
		bool e = table[c.first][c.second].type == 2;
		table[c.first][c.second] = Food(0);
		return { u,e };
	}
};
