// Cargame: Car view; draws the car.
// Copyright 2010 (C) Ralph Thomas

#ifndef CAR_VIEW_H
#define CAR_VIEW_H

#include "CarModel.h"

struct CarView {
	CarView(CarModel* model, int car);
	virtual ~CarView();

	void draw();
  private:
	CarModel* m_model;
	int m_car;
};

#endif
