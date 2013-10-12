// Cargame: Keyboard Controller
// Copyright 2010 (C) Ralph Thomas

#include "KeyboardController.h"
#include <GL/glut.h>

static bool keys_down[256];
static bool registered = false;
void Keyboard(int key, int x, int y) {
	if (key < 0 || key > 255) return;
	keys_down[key] = true;
}
void KeyboardUp(int key, int x, int y) {
	if (key < 0 || key > 255) return;
	keys_down[key] = false;
}
KeyboardController::KeyboardController(CarModel* model) : m_model(model) {
	if (registered) return;
	registered = true;
	glutSpecialFunc(Keyboard);
	glutSpecialUpFunc(KeyboardUp);
	for (int i = 0; i < 256; i++) keys_down[i] = false;
}
KeyboardController::~KeyboardController() {}
void KeyboardController::step() {
	if (keys_down[GLUT_KEY_LEFT]) m_model->setSteering(CarModel::SteerLeft);
	else if (keys_down[GLUT_KEY_RIGHT]) m_model->setSteering(CarModel::SteerRight);
	else m_model->setSteering(CarModel::SteerNone);
	if (keys_down[GLUT_KEY_UP]) m_model->setAcceleration(CarModel::Accelerate);
	else if (keys_down[GLUT_KEY_DOWN]) m_model->setAcceleration(CarModel::Brake);
    else m_model->setAcceleration(CarModel::AccelerateNone);

}
