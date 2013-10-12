// Cargame: Camera Controller
// Copyright 2010 (C) Ralph Thomas

#include <GL/gl.h>
#include <GL/glu.h>
#include "CameraController.h"

CameraController::~CameraController() {}
void CameraController::step(int w, int h) {
	// GL1 code.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float zoom = 20;
	float length = m_follow->velocity() * 0.05f;
	if (length > 1) zoom /= length;
	/*
	 * Now make the actual zoom level by doing some xeno's paradox curve.
	 */
	float z = m_zoom + (zoom - m_zoom) / 8.0f;
	m_zoom = z;

	float wf = w/(2*z);
	float hf = h/(2*z);
	gluOrtho2D(-wf, wf, -hf, hf);
	const b2Transform& xf = m_follow->transform();
	glTranslatef(-xf.p.x, -xf.p.y, 0);
}
