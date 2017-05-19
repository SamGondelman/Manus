#include "Entity.h"

#include "view.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
#include "ConeMesh.h"
#include "CylinderMesh.h"

Entity::Entity(std::shared_ptr<btDiscreteDynamicsWorld> physWorld, ShapeType shapeType, btScalar mass,
               btVector3 pos, btVector3 scale, CS123SceneMaterial mat, btQuaternion rot, btVector3 vel) :
    m_draw(true), m_shapeType(shapeType), m_mat(mat)
{
    setupRigidBody(physWorld, mass, pos, scale, rot, vel);
}

Entity::Entity(std::shared_ptr<btDiscreteDynamicsWorld> physWorld, ShapeType shapeType, btScalar mass,
               btVector3 pos, btVector3 scale, btQuaternion rot, btVector3 vel) :
    m_draw(false), m_shapeType(shapeType)
{
    setupRigidBody(physWorld, mass, pos, scale, rot, vel);
}

void Entity::setupRigidBody(std::shared_ptr<btDiscreteDynamicsWorld> physWorld, btScalar& mass,
      btVector3& pos, btVector3& scale, btQuaternion& rot, btVector3& vel) {
    switch (m_shapeType) {
        case ShapeType::CUBE:
            m_collShape = std::make_unique<btBoxShape>(scale / 2.0f);
            break;
        case ShapeType::SPHERE:
            if (scale[0] == scale[1] && scale[0] == scale[2]) {
                m_collShape = std::make_unique<btSphereShape>(scale[0] / 2.0f);
            } else {
                btVector3 p(0, 0, 0);
                btScalar r(0.5f);
                m_collShape = std::make_unique<btMultiSphereShape>(&p, &r, 1);
                m_collShape->setLocalScaling(scale);
            }
            break;
        case ShapeType::CYLINDER:
            m_collShape = std::make_unique<btCylinderShape>(scale / 2.0f);
            break;
        case ShapeType::CONE:
            m_collShape = std::make_unique<btConeShape>(scale[0] / 2.0f, scale[1]);
            break;
        default:
            m_collShape = std::make_unique<btBoxShape>(scale / 2.0f);
            break;
    }

    m_scale = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

    btVector3 localInertia = btVector3(0,0,0);
    if (mass != 0.f) m_collShape->calculateLocalInertia(mass, localInertia);

    m_motionState = std::make_unique<btDefaultMotionState>(btTransform(rot, pos));
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,
                                                    m_motionState.get(),
                                                    m_collShape.get(),
                                                    localInertia);
    m_rigidBody = std::make_unique<btRigidBody>(rbInfo);
    m_rigidBody->setLinearVelocity(vel);

    physWorld->addRigidBody(m_rigidBody.get());
}

void Entity::getModelMatrix(glm::mat4& m) {
    btTransform t;
    m_rigidBody->getMotionState()->getWorldTransform(t);
    t.getOpenGLMatrix(glm::value_ptr(m));
    m = m * m_scale;
}

void Entity::setScale(const glm::mat4 &s) {
    m_scale = s;
    m_collShape->setLocalScaling(btVector3(s[0][0], s[1][1], s[2][2]));
}

void Entity::draw() {
    switch (m_shapeType) {
        case ShapeType::CUBE:
            View::m_cube->draw();
            break;
        case ShapeType::SPHERE:
            View::m_sphere->draw();
            break;
        case ShapeType::CYLINDER:
            View::m_cylinder->draw();
            break;
        case ShapeType::CONE:
            View::m_cone->draw();
            break;
        default:
            break;
    }
}
