// Cargame: convert the map to box2d objects.
// Copyright 2010 (C) Ralph Thomas

#include "BoxMapView.h"

void boxMapView(b2World* world, MapModel* model) {
	const Building* b = model->getBuildingList();
	while (b) {
		b2BodyDef bodyDef;
		bodyDef.position.Set(b->m_x*4 + b->m_w*2, b->m_y*4 + b->m_h*2);
		b2Body* body = world->CreateBody(&bodyDef);
		b2PolygonShape shape;
		shape.SetAsBox(b->m_w*2, b->m_h*2);
        b2FixtureDef shapeDef;
        shapeDef.shape = &shape;
		body->CreateFixture(&shapeDef);
		b = b->m_next;
	}
}
