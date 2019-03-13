//
//  RigidBodyPolygon.cpp
//  Shooter
//
//  Created by Khanh on 3/13/19.
//

#include "RigidBodyPolygon.h"
#include "../Defines/Templates.h"
#include "../Characters/Character.h"
#include "../Game.h"

unique_ptr<RigidBodyPolygon> RigidBodyPolygon::createRigidBodyPolygon(shared_ptr<Character> object, const vector<Vec2>& vertices)
{
    unique_ptr<RigidBodyPolygon> body = pointer::make_unique<RigidBodyPolygon>();
    body->init();   //init func haven't been used yet
    
    //create shape
    int countVertices = (int)vertices.size();
    b2Vec2* vertices_b2 = new b2Vec2[countVertices]();
    for(int i = 0; i < vertices.size(); i++)
    {
        Vec2 vertex = vertices[0];
        vertices_b2[i].Set(vertex.x, vertex.y);
    }
    
    b2PolygonShape shape;
    shape.Set(vertices_b2, countVertices);
    
    //create fixture
    b2FixtureDef fixture;
    fixture.shape = &shape;
    
    //create b2BodyDef
    auto objectPos = object->_sprite->getPosition();
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(objectPos.x, objectPos.y);
    bodyDef.angle =  CC_DEGREES_TO_RADIANS(object->_sprite->getRotation());
    
    body->_body = Game::getInstance()->getPhysicsWorld()->CreateBody(&bodyDef);
    body->_body->CreateFixture(&fixture);
    
    
    delete[] vertices_b2;
    return body;
}

void RigidBodyPolygon::init()
{
    RigidBody::init();
}

void RigidBodyPolygon::update(float dt)
{
    RigidBody::update(dt);
}
