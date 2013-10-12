// Cargame: Model definition for a map
// Copyright 2010 (C) Ralph Thomas

#include "MapModel.h"
#include <stdio.h>

MapModel::MapModel(const char* mapName) : m_ground(0), m_building(0) {
	Ground* lastGround = 0;
	Building* lastBuilding = 0;
	FILE* f = fopen(mapName, "r");
	if (!f) {
		fprintf(stderr, "Couldn't open map %s\n", mapName);
		return;
	}
	short x(0); short y(0); short w(0); short h(0); int type(0);
	while (true) {
		int rv = fscanf(f, "g %hd %hd %d\n", &x, &y, &type);
		if (rv == EOF) { fclose(f); return; }
		if (rv > 0) {
			Ground* g = new Ground(x, y, type);
			if (lastGround) lastGround->m_next = g;
			if (!m_ground) m_ground = g;
			lastGround = g;
		}
		rv = fscanf(f, "b %hd %hd %hd %hd %d\n", &x, &y, &w, &h, &type);
		if (rv == EOF) { fclose(f); return; }
		if (rv > 0) {
			Building* b = new Building(x, y, w, h, type);
			if (lastBuilding) lastBuilding->m_next = b;
			if (!m_building) m_building = b;
			lastBuilding = b;
		}
	}
}
MapModel::~MapModel() {
	Ground* g = m_ground;
	Building* b = m_building;
	while (g) {
		Ground* t = g->m_next;
		delete g;
		g = t;
	}
	m_ground = 0;
	while (b) {
		Building* t = b->m_next;
		delete b;
		b = t;
	}
}

