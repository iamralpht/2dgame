// Cargame Map View -- Draws the map
// Copyright 2010 (C) Ralph Thomas

#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include "MapModel.h"

struct MapView {
	MapView(MapModel* model);
	virtual ~MapView();

	void draw();
  private:
	MapModel* m_model;
	unsigned int m_texture;
};

#endif

