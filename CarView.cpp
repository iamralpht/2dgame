// Car View, GL1
// Copyright 2010 (C) Ralph Thomas

#include <GL/gl.h>
#include "LoadPng.h"
#include "CarView.h"

GLuint s_car_texture = 0;

CarView::CarView(CarModel* model, int car) : m_model(model), m_car(car) {
	if (s_car_texture) return;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &s_car_texture);
	glBindTexture(GL_TEXTURE_2D, s_car_texture);
	PngFile cars;
	if (cars.read("cars.png")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int format = (cars.rowBytes() / cars.width()) == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, cars.width(), cars.height(), 0, format, GL_UNSIGNED_BYTE, cars.data());
	}
	glDisable(GL_TEXTURE_2D);
}
CarView::~CarView() {}

void CarView::draw() {
	const b2Transform& xf = m_model->transform();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, s_car_texture);
	glPushMatrix();
	glTranslatef(xf.p.x, xf.p.y, 0);
	glRotatef(xf.q.GetAngle() * 180.0f / M_PI, 0, 0, 1);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(m_car * 0.5f, 0.0f);
	glVertex2i(-1, -2);
	glTexCoord2f(m_car * 0.5f + 0.5f, 0.0f);
	glVertex2i(1, -2);
	glTexCoord2f(m_car * 0.5f + 0.5f, 1.0f);
	glVertex2i(1, 2);
	glTexCoord2f(m_car * 0.5f, 1.0f);
	glVertex2i(-1, 2);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
