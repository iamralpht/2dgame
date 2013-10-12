/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include <Box2D/Box2D.h>

#include <cstdio>
#include <GL/gl.h>
#include <GL/glut.h>

#include "CarModel.h"
#include "CarView.h"
#include "MapView.h"
#include "CameraController.h"
#include "KeyboardController.h"
#include "BoxMapView.h"
#include "DebugDraw.h"

b2World* world;
CameraController* camera = 0;
KeyboardController* keyboard = 0;
CarView* carview = 0;
#define NUM_CARS 10
CarView* cars[NUM_CARS];
CarModel* carModels[NUM_CARS + 1];
int width, height;
MapModel* mapModel = 0;
MapView* map = 0;

void Resize(int32_t w, int32_t h) {
	width = w; height = h;
	glViewport(0, 0, w, h);
	camera->step(w, h);
}
void Timer(int) {
	glutPostRedisplay();
	glutTimerFunc(16, Timer, 0);
}
void Traverse() {
	if (!map) {
		map = new MapView(mapModel);
		glClearColor(0.5f, 0.5f, 0.5f, 1);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera->step(width, height);
	keyboard->step();

    for (int i = 0; i < NUM_CARS + 1; i++)
        carModels[i]->update(1.0f/60.0f);

	world->Step(1.0f/60.0f, 10, 8);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	map->draw();
	glColor3f(1, 1, 1);
	// Draw the car.
	carview->draw();
	for (int i = 0; i < NUM_CARS; i++)
		cars[i]->draw();

    world->DrawDebugData();

	glutSwapBuffers();
}
// This is a simple example of building and running a simulation
// using Box2D. Here we create a large ground box and a small dynamic
// box.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 480);
	uint32_t mainWindow = glutCreateWindow("How's My Driving?");
	glutDisplayFunc(Traverse);
	glutReshapeFunc(Resize);
	// Define the size of the world. Simulation will still work
	// if bodies reach the end of the world, but it will be slower.
    /*
	b2AABB worldAABB;
	worldAABB.lowerBound.Set(-400.0f, -400.0f);
	worldAABB.upperBound.Set(400.0f, 400.0f);
    */

	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -0.0f);

	// Do we want to let bodies sleep?
	bool doSleep = true;

	// Construct a world object, which will hold and simulate the rigid bodies.
	world = new b2World(gravity);//, doSleep);

    DebugDraw* draw = new DebugDraw();
    draw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit);

    world->SetDebugDraw(draw);

	mapModel = new MapModel("Level1.txt");

	boxMapView(world, mapModel);

	CarModel* car = new CarModel(world, b2Vec2(10.0f, 10.0f), 0, 2.0f, 4.0f);
	carview = new CarView(car, 1);
	keyboard = new KeyboardController(car);
	camera = new CameraController(car);
	for (int i = 0; i < NUM_CARS; i++) {
        carModels[i] = new CarModel(world, b2Vec2(10.0f * i, 10.0f), 0, 2.0f, 4.0f);
		cars[i] = new CarView(carModels[i], 0);
	}
    carModels[NUM_CARS] = car;

	glutTimerFunc(1.0f / 60.0f, Timer, 0);
	glutMainLoop();

	return 0;
}
