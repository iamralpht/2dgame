// Cargame: Camera Controller
// Copyright 2010 (C) Ralph Thomas

#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "CarModel.h"

struct CameraController {
	CameraController(CarModel* follow) : m_zoom(8), m_follow(follow) {}
	virtual ~CameraController();

	void step(int w, int h);
  private:
	float	m_zoom;
	CarModel* m_follow;
};

#endif

