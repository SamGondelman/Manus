#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <memory>

#include "gl/glew.h"
#include <GL/gl.h>
#include "glm/glm.hpp"

#include <string>
#include <vector>
#include <QPair>
#include <QStringList>

class View;
class FBO;
class CS123Shader;

class ParticleSystem {
public:
    ParticleSystem(int numParticles, std::string drawFrag, std::string drawVert, std::string updateFrag,
                   QStringList &extraAttachments = QStringList(),
                   std::string updateVert = ":/shaders/fullscreenQuad.vert");

    void update(float dt, bool active = true);
    void update(float dt, std::vector<QPair<std::string, glm::vec3>> &args, bool active = true);
    void render(glm::mat4 &V, glm::mat4 &P, void(*drawFunc)(int));

private:
    std::unique_ptr<CS123Shader> m_updateProgram;
    std::unique_ptr<CS123Shader> m_drawProgram;

    std::shared_ptr<FBO> m_FBO1;
    std::shared_ptr<FBO> m_FBO2;
    bool m_firstPass;
    bool m_evenPass;
    int m_numParticles;
    QStringList m_extraAttachments;
};

#endif // PARTICLESYSTEM_H
