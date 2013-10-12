// Cargame Map View
// Copyright 2010 (C) Ralph Thomas

#include "MapView.h"
#include "LoadPng.h"
#include <GL/gl.h>
#include <stdio.h>

float* tile_map = 0;

const float wonk = 0.5f / 256.0f;
MapView::MapView(MapModel* m) : m_model(m) {
	if (!tile_map) {
		const float tileo = 64.0f / 256.0f;
		const float tiles = 64.0f / 256.0f;

		tile_map = new float[4 * 4 * 16];
		float* t = new float[4];
		int idx = 0;
		for (int tile = 0; tile < 16; tile++) {
			t[0] = tiles * (tile % 4) + wonk;
			t[1] = tiles * (tile / 4) + wonk;
			t[2] = t[0] + tiles - 2 * wonk;
			t[3] = t[1] + tiles - 2 * wonk;
			for (int rot = 0; rot < 4; rot++) {
				tile_map[idx++] = t[(rot + 0) % 4];
				tile_map[idx++] = t[(rot + 1) % 4];
				tile_map[idx++] = t[(rot + 2) % 4];
				tile_map[idx++] = t[(rot + 3) % 4];
			}
		}
	}
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	PngFile tiles;
	if (tiles.read("tiles.png")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int format = (tiles.rowBytes() / tiles.width()) == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, tiles.width(), tiles.height(), 0, format, GL_UNSIGNED_BYTE, tiles.data());
	}
	glDisable(GL_TEXTURE_2D);
}
MapView::~MapView() {
	
}
inline void drawTile(int x, int y, int idx) {
	float* tx = tile_map + idx * 4;
	x *= 4;
	y *= 4;
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(tx[0], tx[1]);
		glVertex2i(x, y);
		glTexCoord2f(tx[2], tx[1]);
		glVertex2i(x+4, y);
		glTexCoord2f(tx[2], tx[3]);
		glVertex2i(x+4, y+4);
		glTexCoord2f(tx[0], tx[3]);
		glVertex2i(x, y+4);
	glEnd();
}
void MapView::draw() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glColor3f(1, 1, 1);
	const Ground* g = m_model->getGroundList();
	while (g) {
		drawTile(g->m_x, g->m_y, g->m_tile);
		g = g->m_next;
	}
	glDisable(GL_TEXTURE_2D);
	const Building* b = m_model->getBuildingList();
	while (b) {
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.4f, 0.4f, 0.4f);
		glVertex2i(b->m_x*4, b->m_y*4);
		glVertex2i(b->m_x*4 + b->m_w*4, b->m_y*4);
		glVertex2i(b->m_x*4 + b->m_w*4, b->m_y*4 + b->m_h*4);
		glVertex2i(b->m_x*4, b->m_y*4 + b->m_h*4);
		glEnd();
		b = b->m_next;
	}
}
