// Cargame: Model definition for map.
// Copyright 2010 (C) Ralph Thomas

#ifndef MAP_MODEL_H
#define MAP_MODEL_H

struct Ground {
	Ground(short x, short y, int tile) : m_x(x), m_y(y), m_tile(tile), m_next(0) {}
	Ground(const Ground& rhs) : m_x(rhs.m_x), m_y(rhs.m_y), m_tile(rhs.m_tile), m_next(rhs.m_next) {}
	short m_x;
	short m_y;
	int m_tile;
	Ground* m_next;
};
struct Building {
	Building(short x, short y, short w, short h, int kind) : m_x(x), m_y(y), m_w(w), m_h(h), m_kind(kind), m_next(0) {}
	Building(const Building& rhs) : m_x(rhs.m_x), m_y(rhs.m_y), m_w(rhs.m_w), m_h(rhs.m_h), m_kind(rhs.m_kind), m_next(rhs.m_next) {}
	short m_x;
	short m_y;
	short m_w;
	short m_h;
	int m_kind;
	Building* m_next;
};

struct MapModel {
	MapModel(const char* mapName);
	virtual ~MapModel();

	const Ground* getGroundList() const { return m_ground; }
	const Building* getBuildingList() const { return m_building; }
  private:
	Ground* m_ground;
	Building* m_building;
};

#endif

