// New car model
// Copyright 2013 (C) Ralph Thomas, Palo Alto, CA

#ifndef CAR_MODEL_2_H
#define CAR_MODEL_2_H

#include <Box2D/Box2D.h>
#include <vector>

struct WheelModel;

struct CarModel {
    enum Steering { SteerNone = 0, SteerRight, SteerLeft };
    enum Acceleration { AccelerateNone = 0, Accelerate, Brake };

    CarModel(b2World* world, const b2Vec2& position, float angle, float width, float height);

    void update(float msDuration);
	float velocity() const { return m_body->GetLinearVelocity().Length(); }
    b2Transform transform() const { return m_body->GetTransform(); }

    void setAcceleration(Acceleration a) { m_acceleration = a; }
    void setSteering(Steering s) { m_steering = s; }

    b2Body* body() const { return m_body; }
private:
    Steering m_steering;
    Acceleration m_acceleration;
    float m_maxSteerAngle;
    float m_maxSpeed;
    float m_power;
    float m_wheelAngle;
    b2Body* m_body;
    typedef std::vector<WheelModel*> wheels;
    wheels m_wheels;

    float getSpeedKMH();
    void setSpeed(float);
    b2Vec2 getLocalVelocity();
};

struct WheelModel {
    enum Revolving { IsNotRevolving = 0, IsRevolving };
    enum Powered { IsNotPowered = 0, IsPowered };

    WheelModel(b2World* world, const b2Vec2& position, float width, float length,
               CarModel& car, Revolving revolving, Powered powered);

    void setAngle(float angle) { m_body->SetTransform(m_body->GetPosition(), m_car.body()->GetAngle() + angle); }
    b2Vec2 getLocalVelocity() { return m_car.body()->GetLinearVelocityFromLocalPoint(m_position); }
    b2Vec2 getDirectionVector() {
        return b2Mul(b2Rot(m_body->GetAngle()), (getLocalVelocity().y > 0) ? b2Vec2(0, 1) : b2Vec2(0, -1));
    }
    b2Vec2 getKillVelocityVector() {
        b2Vec2 velocity = m_body->GetLinearVelocity();
        b2Vec2 sidewaysAxis = getDirectionVector();
        float dot = b2Dot(velocity, sidewaysAxis);
        return b2Vec2(sidewaysAxis.x * dot, sidewaysAxis.y * dot);
    }
    void killSidewaysVelocity() { m_body->SetLinearVelocity(getKillVelocityVector()); }

    Powered isPowered() const { return m_powered; }
    Revolving isRevolving() const { return m_revolving; }

    b2Body* body() { return m_body; }
private:
    b2Vec2 m_position;
    CarModel& m_car;
    Revolving m_revolving;
    Powered m_powered;
    b2Body* m_body;
};

#endif

