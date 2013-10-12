// Cargame Car definition.
// Copyright 2010 (C) Ralph Thomas

// Based on work to Box2D by Mike Lewis; github.com/mikelikespie

/*
   BREAKUP:

    MyCar -> CarModel
    MyCar::Projection -> CameraView
    MyCar::Draw -> CarView
    Keyboard/Up -> KeyboardController
    AI? -> CarDriverController
    MapModel
    MapView
    MissionController (timer, score, objectives, etc.).
 */
#ifndef CAR_MODEL_H
#define CAR_MODEL_H

#include <Box2D/Box2D.h>
#include "CarModel2.h"
#include <stdio.h>

class CarModel
{
public:
	float32 wheelDims[2];

	float32 halfWidth;
	float32 halfHeight;
	float32 defaultDensity;
	float32 defaultFriction;
	float32 accelerateForce;

	b2Body* wheels[4];
	b2Body* chassis;
	b2World* world;
	b2Fixture* bodyFixture;

	b2RevoluteJoint *frontLeftJoint;

	bool frontWheelDrive;

	CarModel(b2World *_world, float32 width, float32 height, float32 x) :
		halfWidth(width/2.0f),
		halfHeight(height/2.0f),
		defaultDensity(20.0f),
		defaultFriction(0.68f),
		accelerateForce(2500.0f),
		world(_world),
		frontWheelDrive(true)
	{
		wheelDims[0] = .125;
		wheelDims[1] = .5;

		{
			b2BodyDef chassisBd;

			chassisBd.position = b2Vec2(0.0f, 0.0f);

			chassis = world->CreateBody(&chassisBd);

			b2PolygonShape chassisFixtureShape;

			chassisFixtureShape.SetAsBox(halfWidth, halfHeight);

            b2FixtureDef chassisFixture;

            chassisFixture.shape = &chassisFixtureShape;
			chassisFixture.density = defaultDensity;
			chassisFixture.friction = defaultFriction;

			bodyFixture = chassis->CreateFixture(&chassisFixture);
			//chassis->SetMassFromShapes();



		}

		{
			b2PolygonShape wheelShape;
			wheelShape.SetAsBox(wheelDims[0], wheelDims[1]);

            b2FixtureDef wheelDef;
            wheelDef.shape = &wheelShape;
			wheelDef.density = defaultDensity/4.0;
			wheelDef.friction = defaultFriction;

			wheels[0] = makeWheel(-1, 1, wheelDef);
			wheels[1] = makeWheel(1, 1, wheelDef);
			wheels[2] = makeWheel(-1, -1, wheelDef);
			wheels[3] = makeWheel(1, -1, wheelDef);
		}

		{
			//Make the thing that ties the wheels together.
			b2DistanceJointDef rackDef;

			b2Body *w0 = wheels[0];
			b2Body *w1 = wheels[1];

			float32 offset = wheelDims[1] / 2.0f * 2.0f;

			b2Vec2 pos0 = w0->GetWorldCenter();
			b2Vec2 pos1 = w1->GetWorldCenter();

			pos0.y += offset;
			pos1.y += offset;

			rackDef.Initialize(w0, w1, pos0, pos1);

			world->CreateJoint(&rackDef);
		}
		chassis->SetTransform(b2Vec2(x, 0), 0.f);
	}

	void Steer(float32 dir = 1.0)
	{
		frontLeftJoint->SetMotorSpeed(dir * 2.0);
	}

	void NormalizeSteering() {
		//frontLeftJoint->SetMotorSpeed(frontLeftJoint->GetMotorSpeed() * 0.8);
		//fprintf(stderr, "angle %f\n", frontLeftJoint->GetJointAngle());
		frontLeftJoint->SetMotorSpeed(-frontLeftJoint->GetJointAngle() * chassis->GetLinearVelocity().Length());
	}

	void Accelerate(float32 dir = 1.0)
	{
    fprintf(stderr, "accelerate %f\n", dir);
		// Which two wheels do we power?
		int offset = frontWheelDrive ? 0 : 2;

		for (int i = 0; i < 2; i++) {
			b2Body *wheel = wheels[i + offset];
			b2Vec2 force_vector = wheel->GetTransform().q.GetXAxis();
			force_vector *= accelerateForce * dir;
            fprintf(stderr, "Force vector: %f+%f\n", force_vector.x, force_vector.y);
			wheel->ApplyForce(force_vector, wheel->GetWorldCenter());
		}
	}

	const b2Transform& transform() const { return chassis->GetTransform(); }
	float velocity() const { return chassis->GetLinearVelocity().Length(); }
	virtual ~CarModel()
	{
	}

private:

	b2Body* makeWheel(float32 xmul, float32 ymul, b2FixtureDef &wheelDef)
	{
		b2BodyDef bd;
		b2Body *wheel;
		bd.position = b2Vec2(xmul * (halfWidth - wheelDims[0]), ymul * halfHeight);

		wheel = world->CreateBody(&bd);
		wheel->CreateFixture(&wheelDef);
		wheel->SetLinearDamping(0.01f);
		wheel->SetAngularDamping(5.0);
		//wheel->SetMassFromShapes();

		b2RevoluteJointDef jointDef;
		jointDef.Initialize(chassis, wheel, wheel->GetWorldCenter());

		// If we're doing the front wheels
		if (ymul > 0) {
			jointDef.lowerAngle = -0.25 * b2_pi;
			jointDef.upperAngle = 0.25 * b2_pi;
		}

		jointDef.enableLimit = true;

		b2RevoluteJoint *joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);

		if (xmul < 0 && ymul > 0)
		{

			frontLeftJoint = joint;
			frontLeftJoint->EnableMotor(true);
			frontLeftJoint->SetMaxMotorTorque(5000.0f);
		}
/*
		{
			b2FrictionJointDef jd;

			jd.Initialize(wheel, 0, b2Vec2(400.0,1.0));
			world->CreateJoint(&jd);
		}
*/
		return wheel;
	}

};

#endif

