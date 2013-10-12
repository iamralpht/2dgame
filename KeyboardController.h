// Cargame: Keyboard Controller
// Copyright 2010 (C) Ralph Thomas

#ifndef KEYBOARD_CONTROLLER_H
#define KEYBOARD_CONTROLLER_H

#include "CarModel.h"

struct KeyboardController {
	KeyboardController(CarModel* model);
	virtual ~KeyboardController();

	void step();
  private:
	CarModel* m_model;
};
	

#endif

