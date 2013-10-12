// CarModel
// Copyright 2013 (C) Ralph Thomas, Palo Alto, CA

#include "CarModel.h"

WheelModel::WheelModel(b2World* world, const b2Vec2& position, float width, float length,
               CarModel& car, Revolving revolving, Powered powered)
    : m_position(position)
    , m_car(car)
    , m_revolving(revolving)
    , m_powered(powered)
    , m_body(0)
{
    // Initialize the body.
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position = car.body()->GetWorldPoint(m_position);
    def.angle = car.body()->GetAngle();
    m_body = world->CreateBody(&def);

    // Initialize the shape.
    b2FixtureDef fixDef;
    fixDef.density = 1;
    fixDef.isSensor = true; // no collisions
    b2PolygonShape fixShape;
    fixShape.SetAsBox(width / 2.0f, length / 2.0f);
    fixDef.shape = &fixShape;
    m_body->CreateFixture(&fixDef);

    // Create joint to connect wheel to body
    if (m_revolving == IsRevolving) {
        b2RevoluteJointDef jointDef;
        jointDef.Initialize(car.body(), m_body, m_body->GetWorldCenter());
        jointDef.enableMotor = false; // we control the angle manually
        world->CreateJoint(&jointDef);
    } else {
        b2PrismaticJointDef jointDef;
        jointDef.Initialize(car.body(), m_body, m_body->GetWorldCenter(), b2Vec2(1.0f, 0.0f));
        jointDef.enableLimit = true;
        jointDef.lowerTranslation = jointDef.upperTranslation = 0;
        world->CreateJoint(&jointDef);
    }
}

CarModel::CarModel(b2World* world, const b2Vec2& position, float angle, float width, float height)
    : m_steering(SteerNone)
    , m_acceleration(AccelerateNone)
    , m_maxSteerAngle(15.0f)
    , m_maxSpeed(140.0f)
    , m_power(120.0f)
    , m_wheelAngle(0)
    , m_body(0)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = position;
    bodyDef.angle = angle;
    bodyDef.linearDamping = 0.15f;
    bodyDef.bullet = true;
    bodyDef.angularDamping = 0.3f;
    m_body = world->CreateBody(&bodyDef);

    b2FixtureDef fixDef;
    fixDef.density = 1.0f;
    fixDef.friction = 0.3f;
    fixDef.restitution = 0.4f;
    b2PolygonShape fixShape;
    fixShape.SetAsBox(width / 2.0f, height / 2.0f);
    fixDef.shape = &fixShape;
    m_body->CreateFixture(&fixDef);

    // Make some wheels. We could expose this so that we could have trikes or trucks or maybe bikes.

    m_wheels.push_back(new WheelModel(world, b2Vec2(-1.f, -1.2f), 0.4f, 0.8f, *this, WheelModel::IsNotRevolving, WheelModel::IsPowered));
    m_wheels.push_back(new WheelModel(world, b2Vec2(1.f, -1.2f), 0.4f, 0.8f, *this, WheelModel::IsNotRevolving, WheelModel::IsPowered));
    m_wheels.push_back(new WheelModel(world, b2Vec2(-1.f, 1.2f), 0.4f, 0.8f, *this, WheelModel::IsRevolving, WheelModel::IsNotPowered));
    m_wheels.push_back(new WheelModel(world, b2Vec2(1.f, 1.2f), 0.4f, 0.8f, *this, WheelModel::IsRevolving, WheelModel::IsNotPowered));
}

void CarModel::update(float msDuration) {
    // Kill sideways velocity
    for (wheels::iterator it = m_wheels.begin(); it != m_wheels.end(); ++it)
        (*it)->killSidewaysVelocity();

    // Set wheel angle
    float angleIncrement = (m_maxSteerAngle / 20.0f) * msDuration;
    switch (m_steering) {
    case SteerRight: m_wheelAngle = m_maxSteerAngle; break;//std::max(std::min(m_wheelAngle - angleIncrement, 0.0f), -m_maxSteerAngle); break;
    case SteerLeft: m_wheelAngle = -m_maxSteerAngle; break;//std::min(std::max(m_wheelAngle + angleIncrement, 0.0f), m_maxSteerAngle); break;
    case SteerNone: m_wheelAngle = 0; break;
    }

    // Update revolving wheel
    for (wheels::iterator it = m_wheels.begin(); it != m_wheels.end(); ++it)
        if ((*it)->isRevolving() == WheelModel::IsRevolving) (*it)->setAngle(m_wheelAngle);

    // Apply force to each wheel
    b2Vec2 base(0, 0);
    if (m_acceleration == Accelerate && getSpeedKMH() < m_maxSpeed) {
        base = b2Vec2(0, 1);
    } else if (m_acceleration == Brake) {
        // Braking but still moving forwards; more braking power
        if (getLocalVelocity().y > 0.0f) base = b2Vec2(0, -1.3f);
        // Reversing, less braking power.
        else base = b2Vec2(0, -0.7f);
    }

    // Multiply by engine power to get a force vector
    b2Vec2 forceV(m_power * base.x, m_power * base.y);

    for (wheels::iterator it = m_wheels.begin(); it != m_wheels.end(); ++it) {
        WheelModel* w = *it;
        if (w->isPowered() == WheelModel::IsNotPowered) continue;
        b2Body* wbody = w->body();
        b2Vec2 pos = wbody->GetWorldCenter();
        wbody->ApplyForce(wbody->GetWorldVector(forceV), pos);
    }
    
    // If we're going very slowly then stop to prevent endless slow sliding.
    if (getSpeedKMH() < 4.0f && (m_acceleration == AccelerateNone))
        setSpeed(0);
}

float CarModel::getSpeedKMH() {
    return (m_body->GetLinearVelocity().Length() / 1000.0f) * 3600.0f;
}

void CarModel::setSpeed(float speed) {
    b2Vec2 v = m_body->GetLinearVelocity();
    v.Normalize();
    v *= (speed * 1000.0f) / 3600.0f;
    m_body->SetLinearVelocity(v);
}

b2Vec2 CarModel::getLocalVelocity() {
    return m_body->GetLocalVector(m_body->GetLinearVelocityFromLocalPoint(b2Vec2(0, 0)));
}
