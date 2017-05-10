#include "ParticleSystem.h"

#include <gl/datatype/FBO.h>
#include "gl/shaders/CS123Shader.h"
#include "ResourceLoader.h"
#include "view.h"

ParticleSystem::ParticleSystem(int numParticles, std::string drawFrag, std::string drawVert,
                               std::string updateFrag, QStringList &extraAttachments, std::string updateVert) :
    m_FBO1(nullptr), m_FBO2(nullptr),
    m_firstPass(true), m_evenPass(true),
    m_numParticles(numParticles), m_extraAttachments(extraAttachments)
{
    std::string vertexSource = ResourceLoader::loadResourceFileToString(drawVert);
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(drawFrag);
    m_drawProgram = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

    vertexSource = ResourceLoader::loadResourceFileToString(updateVert);
    fragmentSource = ResourceLoader::loadResourceFileToString(updateFrag);
    m_updateProgram = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

    m_FBO1 = std::make_shared<FBO>(extraAttachments.length() + 2, FBO::DEPTH_STENCIL_ATTACHMENT::NONE, m_numParticles, 1,
                                   TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                   TextureParameters::FILTER_METHOD::NEAREST, GL_FLOAT);
    m_FBO2 = std::make_shared<FBO>(extraAttachments.length() + 2, FBO::DEPTH_STENCIL_ATTACHMENT::NONE, m_numParticles, 1,
                                   TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                   TextureParameters::FILTER_METHOD::NEAREST, GL_FLOAT);
}

// Same as below but without args
void ParticleSystem::update(float dt, bool active) {
    auto prevFBO = m_evenPass ? m_FBO1 : m_FBO2;
    auto nextFBO = !m_evenPass ? m_FBO1 : m_FBO2;
    float firstPass = m_firstPass ? 1.0f : 0.0f;

    // Update
    m_updateProgram->bind();
    nextFBO->bind();

    // Setup update uniforms
    m_updateProgram->setUniform("active", active ? 1.0f : 0.0f);
    m_updateProgram->setUniform("dt", dt);
    m_updateProgram->setUniform("firstPass", firstPass);
    m_updateProgram->setUniform("numParticles", m_numParticles);
    m_updateProgram->setTexture("prevPos", prevFBO->getColorAttachment(0));
    m_updateProgram->setTexture("prevVel", prevFBO->getColorAttachment(1));
    for (int i = 0; i < m_extraAttachments.length(); i++) {
        m_updateProgram->setTexture(m_extraAttachments[i].toStdString(), prevFBO->getColorAttachment(2 + i));
    }

    // Draw fullscreen quad
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    nextFBO->unbind();
    m_updateProgram->unbind();
}

void ParticleSystem::update(float dt, std::vector<QPair<std::string, glm::vec3>> &args, bool active) {
    auto prevFBO = m_evenPass ? m_FBO1 : m_FBO2;
    auto nextFBO = !m_evenPass ? m_FBO1 : m_FBO2;
    float firstPass = m_firstPass ? 1.0f : 0.0f;

    // Update
    m_updateProgram->bind();
    nextFBO->bind();

    // Setup update uniforms
    for (auto &a : args) m_updateProgram->setUniform(a.first, a.second);
    m_updateProgram->setUniform("active", active ? 1.0f : 0.0f);
    m_updateProgram->setUniform("dt", dt);
    m_updateProgram->setUniform("firstPass", firstPass);
    m_updateProgram->setUniform("numParticles", m_numParticles);
    m_updateProgram->setTexture("prevPos", prevFBO->getColorAttachment(0));
    m_updateProgram->setTexture("prevVel", prevFBO->getColorAttachment(1));
    for (int i = 0; i < m_extraAttachments.length(); i++) {
        m_updateProgram->setTexture(m_extraAttachments[i].toStdString(), prevFBO->getColorAttachment(2 + i));
    }

    // Draw fullscreen quad
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    nextFBO->unbind();
    m_updateProgram->unbind();
}

void ParticleSystem::render(glm::mat4& V, glm::mat4& P, void(*drawFunc)(int)) {
    auto nextFBO = !m_evenPass ? m_FBO1 : m_FBO2;

    // Draw particles
    m_drawProgram->bind();

    // Setup draw uniforms
    m_drawProgram->setTexture("pos", nextFBO->getColorAttachment(0));
    m_drawProgram->setTexture("vel", nextFBO->getColorAttachment(1));
    m_drawProgram->setUniform("numParticles", m_numParticles);
    m_drawProgram->setUniform("V", V);
    m_drawProgram->setUniform("P", P);

    // drawFunc is in charge of drawing particle vertices
    drawFunc(m_numParticles);

    m_firstPass = false;
    m_evenPass = !m_evenPass;
}
