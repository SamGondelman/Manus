#include "view.h"

#include "viewformat.h"
#include "ResourceLoader.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "gl/shaders/CS123Shader.h"
#include "CS123SceneData.h"
#include "gl/datatype/FBO.h"
#include "gl/textures/Texture2D.h"
#include "gl/textures/TextureParametersBuilder.h"
#include "SphereMesh.h"
#include "CubeMesh.h"
#include "ConeMesh.h"
#include "CylinderMesh.h"
#include "gl/datatype/VAO.h"
#include "FullScreenQuad.h"
#include "Player.h"
#include "ParticleSystem.h"

#include "Entity.h"
#include "World.h"
#include "DemoWorld.h"
#include "LightWorld.h"
#include "WaterWorld.h"
#include "RockWorld.h"
#include "PhysicsWorld.h"

#include <QApplication>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>
#include <iostream>

#define M_2PI 6.28318530718

float View::m_globalTime = 0.0f;
std::unique_ptr<Player> View::m_player = nullptr;
std::unique_ptr<SphereMesh> View::m_sphere = nullptr;
std::unique_ptr<CubeMesh> View::m_cube = nullptr;
std::unique_ptr<ConeMesh> View::m_cone = nullptr;
std::unique_ptr<CylinderMesh> View::m_cylinder = nullptr;
unsigned int View::m_fullscreenQuadVAO = 0;
std::unordered_set<int> View::m_pressedKeys = std::unordered_set<int>();

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_fps(0.0f), m_rockTimeLeft(0.0f), m_rockTimeRight(0.0f),
    m_shield(nullptr), m_drawMode(DrawMode::DEFAULT), m_world(WORLD_DEMO),
    m_exposure(1.0f), m_useAdaptiveExposure(true)
{
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The update loop is implemented using a timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));

    // Print FPS
    connect(&m_FPStimer, SIGNAL(timeout()), this, SLOT(printFPS()));
}

View::~View()
{
    if (m_hmd) vr::VR_Shutdown();
    // Must remove shields here to ensure entity deletion happens in right order
    m_worlds[m_world]->getPhysWorld()->removeRigidBody(m_shield->m_rigidBody.get());
    glDeleteVertexArrays(1, &m_fullscreenQuadVAO);
}

void View::initializeGL() {
    ResourceLoader::initializeGlew();

    m_time.start();
    m_timer.start(1000 / 90);

    m_FPStimer.start(1000);

    // OpenGL setup
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glGenVertexArrays(1, &m_fullscreenQuadVAO);

    m_sphere = std::make_unique<SphereMesh>(10, 10);
    m_cube = std::make_unique<CubeMesh>(1);
    m_cone = std::make_unique<ConeMesh>(20, 20);
    m_cylinder = std::make_unique<CylinderMesh>(20, 20);
    m_lightSphere = std::make_unique<SphereMesh>(15, 15);
    m_fullscreenQuad = std::make_unique<FullScreenQuad>();

    // Shader setup
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/lighting.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/lighting.frag");
    m_lightingProgram = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

    vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/rock.vert");
    fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/rock.frag");
    m_rockProgram = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

    vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenQuad.vert");
    fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/bright.frag");
    m_brightProgram = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

    vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/shader.vert");
    fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/distortionStencil.frag");
    m_distortionStencilProgram = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

    vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenQuad.vert");
    fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/distortion.frag");
    m_distortionProgram = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

    vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenQuad.vert");
    fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/horizontalBlur.frag");
    m_horizontalBlurProgram = std::make_shared<CS123Shader>(vertexSource, fragmentSource);

    vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenQuad.vert");
    fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/verticalBlur.frag");
    m_verticalBlurProgram = std::make_shared<CS123Shader>(vertexSource, fragmentSource);

    vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenQuad.vert");
    fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/texture.frag");
    m_textureProgram = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

    vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenQuad.vert");
    fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/bloom.frag");
    m_bloomProgram = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

    // Player setup
    m_player = std::make_unique<Player>(m_eyeWidth, m_eyeHeight);

    m_lightParticlesLeft = std::make_shared<ParticleSystem>(5000,
                                                            ":/shaders/lightParticlesDraw.frag",
                                                            ":/shaders/lightParticlesDraw.vert",
                                                            ":/shaders/lightParticlesUpdate.frag");
    m_lightParticlesRight = std::make_shared<ParticleSystem>(5000,
                                                             ":/shaders/lightParticlesDraw.frag",
                                                             ":/shaders/lightParticlesDraw.vert",
                                                             ":/shaders/lightParticlesUpdate.frag");
    QStringList fireAttachments;
    fireAttachments.append(QString("initHandPosIn"));
    fireAttachments.append(QString("initHandDirIn"));
    m_fireParticlesLeft = std::make_shared<ParticleSystem>(1000,
                                                           ":/shaders/fireParticlesDraw.frag",
                                                           ":/shaders/fireParticlesDraw.vert",
                                                           ":/shaders/fireParticlesUpdate.frag",
                                                           fireAttachments);
    m_fireParticlesRight = std::make_shared<ParticleSystem>(1000,
                                                            ":/shaders/fireParticlesDraw.frag",
                                                            ":/shaders/fireParticlesDraw.vert",
                                                            ":/shaders/fireParticlesUpdate.frag",
                                                            fireAttachments);

    QImage shieldMapImage = QImage(":/images/shieldNormalMap.png");
    m_shieldMap = std::make_unique<Texture2D>(shieldMapImage.bits(),
                                              shieldMapImage.width(),
                                              shieldMapImage.height());
    // TODO: move these into Texture2D
    TextureParametersBuilder builder;
    builder.setFilter(TextureParameters::FILTER_METHOD::LINEAR);
    builder.setWrap(TextureParameters::WRAP_METHOD::REPEAT);
    TextureParameters parameters = builder.build();
    parameters.applyTo(*m_shieldMap);

    // World setup
    m_worlds.push_back(std::make_shared<DemoWorld>());
    m_worlds.push_back(std::make_shared<LightWorld>());
    m_worlds.push_back(std::make_shared<WaterWorld>());
    m_worlds.push_back(std::make_shared<RockWorld>());
    m_worlds.push_back(std::make_shared<PhysicsWorld>());
    switchWorld();

    initVR();

    m_player->setAspectRatio(m_eyeWidth, m_eyeHeight);

    // Initialize screen buffers
    // contains view space positions/normals and ambient/diffuse/specular colors
    // reused for distortion objects to take advantage of depth buffer
    m_deferredBuffer = std::make_unique<FBO>(5, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY, m_eyeWidth, m_eyeHeight,
                                             TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                             TextureParameters::FILTER_METHOD::LINEAR,
                                             GL_FLOAT);
    // collects lighting, contains whole scene minus distortion objects
    m_lightingBuffer = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE, m_eyeWidth, m_eyeHeight,
                                             TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                             TextureParameters::FILTER_METHOD::LINEAR,
                                             GL_FLOAT);
    // contains whole scene + distortion objects
    m_distortionBuffer = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE, m_eyeWidth, m_eyeHeight,
                                               TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                               TextureParameters::FILTER_METHOD::LINEAR,
                                               GL_FLOAT);

    // half-sized buffers for collecting/blurring bright areas
    m_vblurBuffer = std::make_shared<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE, m_eyeWidth/2, m_eyeHeight/2,
                                          TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                          TextureParameters::FILTER_METHOD::LINEAR,
                                          GL_FLOAT);
    m_hblurBuffer = std::make_shared<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE, m_eyeWidth/2, m_eyeHeight/2,
                                          TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                          TextureParameters::FILTER_METHOD::LINEAR,
                                          GL_FLOAT);
}

void View::initVR() {
    vr::EVRInitError error = vr::VRInitError_None;
    m_hmd = vr::VR_Init(&error, vr::VRApplication_Scene);

    if (error != vr::VRInitError_None) {
        m_hmd = nullptr;
        QString message = vr::VR_GetVRInitErrorAsEnglishDescription(error);
        qCritical() << message;
        QMessageBox::critical(this, "Unable to init VR", message);
        return;
    }

    // get eye matrices
    m_rightProjection = vrMatrixToQt(m_hmd->GetProjectionMatrix(vr::Eye_Right, m_player->getNear(), m_player->getFar()));
    m_rightPose = vrMatrixToQt(m_hmd->GetEyeToHeadTransform(vr::Eye_Right));

    m_leftProjection = vrMatrixToQt(m_hmd->GetProjectionMatrix(vr::Eye_Left, m_player->getNear(), m_player->getFar()));
    m_leftPose = vrMatrixToQt(m_hmd->GetEyeToHeadTransform(vr::Eye_Left));

    // setup frame buffers for eyes
    m_hmd->GetRecommendedRenderTargetSize(&m_eyeWidth, &m_eyeHeight);

    if (!vr::VRCompositor()) {
        QString message = "Compositor initialization failed. See log file for details";
        qCritical() << message;
        QMessageBox::critical(this, "Unable to init VR", message);
    }

    // VR buffers
    m_leftEyeBuffer = std::make_shared<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE, m_eyeWidth, m_eyeHeight,
                                            TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                            TextureParameters::FILTER_METHOD::LINEAR,
                                            GL_UNSIGNED_BYTE);
    m_rightEyeBuffer = std::make_shared<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE, m_eyeWidth, m_eyeHeight,
                                             TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                             TextureParameters::FILTER_METHOD::LINEAR,
                                             GL_UNSIGNED_BYTE);

    // Make sure hands are disabled initially
    m_trackedHandPoses[Hand::LEFT].bDeviceIsConnected = false;
    m_trackedHandPoses[Hand::RIGHT].bDeviceIsConnected = false;
}

void View::resizeGL(int w, int h) {

}

void View::paintGL() {
    renderEye(vr::Eye_Left);
    renderEye(vr::Eye_Right);

    vr::Texture_t left = { (void*)m_leftEyeBuffer->getColorAttachment(0).id(), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
    vr::Texture_t right = { (void*)m_rightEyeBuffer->getColorAttachment(0).id(), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
    vr::VRCompositor()->Submit(vr::Eye_Left, &left);
    vr::VRCompositor()->Submit(vr::Eye_Right, &right);
}

void View::renderEye(vr::EVREye eye) {
    // Draw to deferred buffer
    glDisable(GL_BLEND);

    auto eyeBuffer = eye == vr::Eye_Left ? m_leftEyeBuffer : m_rightEyeBuffer;
    auto worldProgram = m_worlds[m_world]->getWorldProgram();

    worldProgram->bind();
    m_deferredBuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 V = (eye == vr::Eye_Right ? m_leftPose : m_rightPose) * m_hmdPose;
    worldProgram->setUniform("V", V);
    glm::mat4 P = eye == vr::Eye_Left ? m_leftProjection : m_rightProjection;
    worldProgram->setUniform("P", P);

    if (m_drawMode != DrawMode::LIGHTS) {
        m_worlds[m_world]->drawGeometry();
        drawHands(V, P);
        drawParticles(m_dt, V, P);
        drawRocks(V, P);
    } else {
        // Draw point lights as geometry
        for (auto& light : m_worlds[m_world]->getLights()) {
            if (light.type == LightType::LIGHT_POINT) {
                glm::mat4 M = glm::translate(light.pos) * glm::scale(glm::vec3(2.0f * light.radius));
                worldProgram->setUniform("M", M);
                CS123SceneMaterial mat;
                mat.cAmbient = glm::vec4(light.col, 1);
                worldProgram->applyMaterial(mat);
                m_lightSphere->draw();
            }
        }
    }

    m_deferredBuffer->unbind();
    worldProgram->unbind();

    // Drawing to screen
    if (m_drawMode == DrawMode::POSITION || m_drawMode == DrawMode::NORMAL || m_drawMode == DrawMode::AMBIENT ||
            m_drawMode == DrawMode::LIGHTS) {
        // No lighting
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_deferredBuffer->getId());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, eyeBuffer->getId());
        int offset = m_drawMode != DrawMode::LIGHTS ? m_drawMode : DrawMode::AMBIENT;
        glReadBuffer(GL_COLOR_ATTACHMENT0 + offset);
        glBlitFramebuffer(0, 0, m_eyeWidth, m_eyeHeight, 0, 0, m_eyeWidth, m_eyeHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    } else {
        // Lighting
        m_lightingProgram->bind();
        m_lightingBuffer->bind();
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glClear(GL_COLOR_BUFFER_BIT);

        bool useLighting = m_drawMode > DrawMode::LIGHTS;

        // Ambient term
        if (m_drawMode != DrawMode::DIFFUSE && m_drawMode != DrawMode::SPECULAR) {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_deferredBuffer->getId());
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_lightingBuffer->getId());
            glReadBuffer(GL_COLOR_ATTACHMENT0 + DrawMode::AMBIENT);
            glBlitFramebuffer(0, 0, m_eyeWidth, m_eyeHeight, 0, 0, m_eyeWidth, m_eyeHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        }

        // Diffuse and/or specular terms
        m_lightingProgram->setUniform("useDiffuse",(useLighting || m_drawMode == DrawMode::DIFFUSE) ? 1.0f : 0.0f);
        m_lightingProgram->setUniform("useSpecular", (useLighting || m_drawMode == DrawMode::SPECULAR) ? 1.0f : 0.0f);
        m_lightingProgram->setUniform("screenSize", glm::vec2(m_eyeWidth, m_eyeHeight));
        m_lightingProgram->setUniform("V", V);
        m_lightingProgram->setUniform("P", P);

        m_lightingProgram->setTexture("pos", m_deferredBuffer->getColorAttachment(0));
        m_lightingProgram->setTexture("nor", m_deferredBuffer->getColorAttachment(1));
        m_lightingProgram->setTexture("diff", m_deferredBuffer->getColorAttachment(3));
        m_lightingProgram->setTexture("spec", m_deferredBuffer->getColorAttachment(4));
        for (auto& light : m_worlds[m_world]->getLights()) {
            // Light uniforms
            m_lightingProgram->setUniform("light.type", static_cast<int>(light.type));
            m_lightingProgram->setUniform("light.col", light.col);
            if (light.type == LightType::LIGHT_POINT) {
                m_lightingProgram->setUniform("light.posDir", light.pos);
                m_lightingProgram->setUniform("light.func", light.func);
                m_lightingProgram->setUniform("light.radius", light.radius);
            } else if (light.type == LightType::LIGHT_DIRECTIONAL) {
                m_lightingProgram->setUniform("light.posDir", light.dir);
            }

            // Draw light
            glm::vec3 dist = light.pos - m_player->getEye();
            if (light.type == LightType::LIGHT_POINT && glm::dot(dist, dist) > light.radius * light.radius) {
                // Outside point light
                m_lightingProgram->setUniform("worldSpace", 1.0f);

                glm::mat4 M = glm::translate(light.pos) * glm::scale(glm::vec3(2.0f * light.radius));
                m_lightingProgram->setUniform("M", M);
                m_lightSphere->draw();
            } else {
                // Inside point light or directional light
                m_lightingProgram->setUniform("worldSpace", 0.0f);

                m_fullscreenQuad->draw();
            }
        }

        m_lightingBuffer->unbind();
        m_lightingProgram->unbind();
        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_BLEND);

        if (m_drawMode == DrawMode::DIFFUSE || m_drawMode == DrawMode::SPECULAR || m_drawMode == DrawMode::NO_HDR) {
            // For debugging, draw lighting buffer before HDR/bloom
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_lightingBuffer->getId());
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, eyeBuffer->getId());
            glReadBuffer(GL_COLOR_ATTACHMENT0);
            glBlitFramebuffer(0, 0, m_eyeWidth, m_eyeHeight, 0, 0, m_eyeWidth, m_eyeHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        } else {
            if (m_drawMode != DrawMode::NO_DISTORTION) {
                // Distortion objects
                // Render distortion objects with deferred buffer depth attachment to get distortion stencil
                m_distortionStencilProgram->bind();
                m_deferredBuffer->bind();
                glClear(GL_COLOR_BUFFER_BIT);
                m_distortionStencilProgram->setUniform("V", V);
                m_distortionStencilProgram->setUniform("P", P);
                drawDistortionObjects();
                m_deferredBuffer->unbind();
                m_distortionStencilProgram->unbind();

                // Combine distortion with lighting buffer
                m_distortionProgram->bind();
                m_distortionBuffer->bind();
                glClear(GL_COLOR_BUFFER_BIT);
                m_distortionProgram->setTexture("offsetMap", m_deferredBuffer->getColorAttachment(0));
                m_distortionProgram->setTexture("color", m_lightingBuffer->getColorAttachment(0));
                glBindVertexArray(m_fullscreenQuadVAO);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glBindVertexArray(0);
                m_distortionBuffer->unbind();
                m_distortionProgram->unbind();
            } else {
                glBindFramebuffer(GL_READ_FRAMEBUFFER, m_lightingBuffer->getId());
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_distortionBuffer->getId());
                glReadBuffer(GL_COLOR_ATTACHMENT0);
                glBlitFramebuffer(0, 0, m_eyeWidth, m_eyeHeight, 0, 0, m_eyeWidth, m_eyeHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
            }

            // HDR/bloom
            // Extract bright areas
            m_brightProgram->bind();
            m_vblurBuffer->bind();
            glClear(GL_COLOR_BUFFER_BIT);

            m_brightProgram->setTexture("color", m_distortionBuffer->getColorAttachment(0));

            glBindVertexArray(m_fullscreenQuadVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);

            m_vblurBuffer->unbind();
            m_brightProgram->unbind();

            if (m_drawMode == DrawMode::BRIGHT) {
                // Draw bright areas (already downscaled/upscaled but not blurred)
                m_textureProgram->bind();
                eyeBuffer->bind();
                glViewport(0, 0, m_eyeWidth, m_eyeHeight);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                m_textureProgram->setTexture("tex", m_vblurBuffer->getColorAttachment(0));

                glBindVertexArray(m_fullscreenQuadVAO);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glBindVertexArray(0);

                eyeBuffer->unbind();
                m_textureProgram->unbind();
            } else {
                // Blur bright areas
                bool horizontal = true;
                for (int i = 0; i < 2; i++) {
                    auto from = horizontal ? m_vblurBuffer : m_hblurBuffer;
                    auto to = !horizontal ? m_vblurBuffer : m_hblurBuffer;
                    auto program = horizontal ? m_horizontalBlurProgram : m_verticalBlurProgram;

                    program->bind();
                    to->bind();
                    glClear(GL_COLOR_BUFFER_BIT);

                    program->setTexture("tex", from->getColorAttachment(0));

                    glBindVertexArray(m_fullscreenQuadVAO);
                    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                    glBindVertexArray(0);

                    to->unbind();
                    program->unbind();

                    horizontal = !horizontal;
                }

                if (m_drawMode == DrawMode::BRIGHT_BLUR) {
                    // Draw blurred bright areas
                    m_textureProgram->bind();
                    eyeBuffer->bind();
                    glViewport(0, 0, m_eyeWidth, m_eyeHeight);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    m_textureProgram->setTexture("tex", m_vblurBuffer->getColorAttachment(0));

                    glBindVertexArray(m_fullscreenQuadVAO);
                    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                    glBindVertexArray(0);

                    eyeBuffer->unbind();
                    m_textureProgram->unbind();
                } else {
                    // Calculate adaptive exposure
                    if (m_useAdaptiveExposure) {
                        m_distortionBuffer->getColorAttachment(0).bind();
                        glGenerateMipmap(GL_TEXTURE_2D);
                        int highestMipMapLevel = std::floor(std::log2(std::max(m_eyeWidth, m_eyeHeight)));
                        float averageLuminance;
                        glGetTexImage(GL_TEXTURE_2D, highestMipMapLevel, GL_RGBA, GL_FLOAT, &averageLuminance);
                        m_distortionBuffer->getColorAttachment(0).unbind();

                        float averageLuminanceClamped = std::fmaxf(0.2f, std::fminf(averageLuminance, 0.8f));
                        float exposureAdjustmentRate = 0.1f;
                        m_exposure = m_exposure + (0.5f/averageLuminanceClamped - m_exposure) * exposureAdjustmentRate;
                    } else {
                        m_exposure = 1.0f;
                    }

                    // Recombine bloom, tonemapping
                    m_bloomProgram->bind();
                    eyeBuffer->bind();
                    glViewport(0, 0, m_eyeWidth, m_eyeHeight);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    m_bloomProgram->setTexture("color", m_distortionBuffer->getColorAttachment(0));
                    m_bloomProgram->setTexture("bloom", m_vblurBuffer->getColorAttachment(0));
                    m_bloomProgram->setUniform("exposure", m_exposure);

                    glBindVertexArray(m_fullscreenQuadVAO);
                    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                    glBindVertexArray(0);

                    eyeBuffer->unbind();
                    m_bloomProgram->unbind();
                }
            }
        }
    }
}

void View::drawHands(glm::mat4 &V, glm::mat4 &P) {
    auto program = m_worlds[WorldState::WORLD_DEMO]->getWorldProgram();
    program->bind();
    program->setUniform("V", V);
    program->setUniform("P", P);

    CS123SceneMaterial mat;
    switch (m_action) {
        case GRAB:
            mat.cAmbient = glm::vec4(0.2, 0.2, 0.2, 1);
            break;
        case LIGHT:
            mat.cAmbient = glm::vec4(10, 10, 10, 1);
            break;
        case FIRE:
            mat.cAmbient = glm::vec4(0.75, 0.2, 0, 1);
            break;
        case ROCK:
            mat.cAmbient = glm::vec4(0.137, 0, 0.118, 1);
            break;
        case WATER:
            mat.cAmbient = glm::vec4(0, 0.5, 0.5, 1);
            break;
    }

    mat.cDiffuse = glm::vec4(0.5, 0.5, 0.5, 1);
    mat.cSpecular = glm::vec4(0.5, 0.5, 0.5, 1);
    mat.shininess = 10.0;
    program->applyMaterial(mat);

    if (m_trackedHandPoses[Hand::LEFT].bPoseIsValid && m_trackedHandPoses[Hand::LEFT].bDeviceIsConnected) {
        program->setUniform("M", vrMatrixToQt(m_trackedHandPoses[Hand::LEFT].mDeviceToAbsoluteTracking) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.1, 0.25, 0.1)));
        m_cone->draw();
    }

    if (m_trackedHandPoses[Hand::RIGHT].bPoseIsValid && m_trackedHandPoses[Hand::RIGHT].bDeviceIsConnected) {
        program->setUniform("M", vrMatrixToQt(m_trackedHandPoses[Hand::RIGHT].mDeviceToAbsoluteTracking) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.1, 0.25, 0.1)));
        m_cone->draw();
    }

    program->unbind();
}

void drawCube(int num) {
    View::m_cube->draw(num);
}

void drawFire(int num) {
    glBindVertexArray(View::m_fullscreenQuadVAO);
    // Top cap
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 5, num);
    // Bottom cap
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 5, 5, num);
    glBindVertexArray(0);
}

void View::drawParticles(float dt, glm::mat4& V, glm::mat4& P) {
    // variable update rate doesn't work?
    dt = 1.0f/90.0f;

    // Bind the fullscreen VAO to update entire particle texture
    glBindVertexArray(m_fullscreenQuadVAO);

    // TODO: do this better
    std::vector<QPair<std::string, glm::vec3>> args(2);
    if (m_trackedHandPoses[Hand::LEFT].bPoseIsValid && m_trackedHandPoses[Hand::LEFT].bDeviceIsConnected) {
        glm::mat4 mat = vrMatrixToQt(m_trackedHandPoses[Hand::LEFT].mDeviceToAbsoluteTracking);
        args[0] = QPair<std::string, glm::vec3>("handPos", glm::vec3(mat[3]));
        args[1] = QPair<std::string, glm::vec3>("handDir", glm::normalize(glm::mat3(mat) * glm::vec3(0, 0, -1)));
    }
    m_lightParticlesLeft->update(dt, args, m_action == LIGHT && _axisStates[LEFT_TRIGGER] >= 1.0f);
    m_fireParticlesLeft->update(dt, args, m_action == FIRE && _axisStates[LEFT_TRIGGER] >= 1.0f);

    if (m_trackedHandPoses[Hand::RIGHT].bPoseIsValid && m_trackedHandPoses[Hand::RIGHT].bDeviceIsConnected) {
        glm::mat4 mat = vrMatrixToQt(m_trackedHandPoses[Hand::RIGHT].mDeviceToAbsoluteTracking);
        args[0] = QPair<std::string, glm::vec3>("handPos", glm::vec3(mat[3]));
        args[1] = QPair<std::string, glm::vec3>("handDir", glm::normalize(glm::mat3(mat) * glm::vec3(0, 0, -1)));
    }
    m_lightParticlesRight->update(dt, args, m_action == LIGHT && _axisStates[RIGHT_TRIGGER] >= 1.0f);
    m_fireParticlesRight->update(dt, args, m_action == FIRE && _axisStates[RIGHT_TRIGGER] >= 1.0f);
    glBindVertexArray(0);

    // Bind the deferred buffer to draw the particles
    m_deferredBuffer->bind();
    m_lightParticlesLeft->render(V, P, &drawCube);
    m_lightParticlesRight->render(V, P, &drawCube);
    m_fireParticlesLeft->render(V, P, &drawFire);
    m_fireParticlesRight->render(V, P, &drawFire);
}

void View::drawRocks(glm::mat4& V, glm::mat4& P) {
    glDisable(GL_CULL_FACE);
    m_rockProgram->bind();
    m_rockProgram->setUniform("V", V);
    m_rockProgram->setUniform("P", P);

    if (m_rockTimeLeft > 0.0f && m_trackedHandPoses[Hand::LEFT].bPoseIsValid && m_trackedHandPoses[Hand::LEFT].bDeviceIsConnected) {
        m_rockProgram->setUniform("time", m_rockTimeLeft);
        glm::mat4 M = vrMatrixToQt(m_trackedHandPoses[Hand::LEFT].mDeviceToAbsoluteTracking) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.1, 0.25, 0.1));
        m_rockProgram->setUniform("M", M);
        m_sphere->draw();
    }

    if (m_rockTimeRight > 0.0f && m_trackedHandPoses[Hand::RIGHT].bPoseIsValid && m_trackedHandPoses[Hand::RIGHT].bDeviceIsConnected) {
        m_rockProgram->setUniform("time", m_rockTimeRight);
        glm::mat4 M = vrMatrixToQt(m_trackedHandPoses[Hand::RIGHT].mDeviceToAbsoluteTracking) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.1, 0.25, 0.1));
        m_rockProgram->setUniform("M", M);
        m_cone->draw();
    }

    m_rockProgram->unbind();
    glEnable(GL_CULL_FACE);
}

void View::drawDistortionObjects() {
    m_distortionStencilProgram->setTexture("normalMap", *m_shieldMap);

    if (m_action == WATER && _axisStates[LEFT_TRIGGER] >= 1.0f && _axisStates[RIGHT_TRIGGER] >= 1.0f) {
        glm::mat4 M;
        m_shield->getModelMatrix(M);
        m_distortionStencilProgram->setUniform("M", M);
        m_shield->draw();
    }
}

void View::mousePressEvent(QMouseEvent *event) {

}

void View::mouseMoveEvent(QMouseEvent *event) {

}

void View::mouseReleaseEvent(QMouseEvent *event) {

}

void View::keyPressEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) m_pressedKeys.insert(event->key());

    if (event->key() == Qt::Key_Escape) QApplication::quit();

    if (event->key() == Qt::Key_1) m_drawMode = DrawMode::DEFAULT;
    else if (event->key() == Qt::Key_2) m_drawMode = DrawMode::POSITION;
    else if (event->key() == Qt::Key_3) m_drawMode = DrawMode::NORMAL;
    else if (event->key() == Qt::Key_4) m_drawMode = DrawMode::AMBIENT;
    else if (event->key() == Qt::Key_5) m_drawMode = DrawMode::LIGHTS;
    else if (event->key() == Qt::Key_6) m_drawMode = DrawMode::DIFFUSE;
    else if (event->key() == Qt::Key_7) m_drawMode = DrawMode::SPECULAR;
    else if (event->key() == Qt::Key_8) m_drawMode = DrawMode::NO_HDR;
    else if (event->key() == Qt::Key_9) m_drawMode = m_drawMode == DrawMode::BRIGHT ? DrawMode::BRIGHT_BLUR : DrawMode::BRIGHT;
    else if (event->key() == Qt::Key_0) m_drawMode = DrawMode::NO_DISTORTION;

    WorldState prevWorld = m_world;
    if (event->key() == Qt::Key_F1) m_world = WorldState::WORLD_DEMO;
    else if (event->key() == Qt::Key_F2) m_world = WorldState::WORLD_1;
    else if (event->key() == Qt::Key_F3) m_world = WorldState::WORLD_2;
    else if (event->key() == Qt::Key_F4) m_world = WorldState::WORLD_3;
    else if (event->key() == Qt::Key_F5) m_world = WorldState::WORLD_4;

    if (m_world != prevWorld) switchWorld(prevWorld);

    if (event->key() == Qt::Key_P) m_useAdaptiveExposure = !m_useAdaptiveExposure;
}

void View::keyReleaseEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) m_pressedKeys.erase(event->key());
}

void View::updatePoses() {
    vr::VRCompositor()->WaitGetPoses(m_trackedDevicePose, vr::k_unMaxTrackedDeviceCount, nullptr, 0);

    if (m_trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
        glm::mat4 h = vrMatrixToQt(m_trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking);
        m_hmdPose = glm::inverse(h);
        m_player->setEye(glm::vec3(h[3][0], h[3][1], h[3][2]));
    }

    if (m_hmd->IsTrackedDeviceConnected(m_hmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand))) {
        m_trackedHandPoses[Hand::LEFT] = m_trackedDevicePose[m_hmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand)];
    }
    if (m_hmd->IsTrackedDeviceConnected(m_hmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand))) {
        m_trackedHandPoses[Hand::RIGHT] = m_trackedDevicePose[m_hmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand)];
    }
}

void View::updateInputs() {
    _buttonStates.clear();

    if (m_hmd->IsTrackedDeviceConnected(m_hmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand))) {
        vr::VRControllerState_t state;
        if (m_hmd->GetControllerState(m_hmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand), &state, sizeof(state))) {
            handleInput(state, true);
        }
    }

    if (m_hmd->IsTrackedDeviceConnected(m_hmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand))) {
        vr::VRControllerState_t state;
        if (m_hmd->GetControllerState(m_hmd->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand), &state, sizeof(state))) {
            handleInput(state, false);
        }
    }
}

void View::handleInput(const vr::VRControllerState_t &state, bool isLeftHand) {
    // Buttons
    for (uint32_t button = 0; button < vr::k_EButton_Max; button++) {
        auto mask = vr::ButtonMaskFromId((vr::EVRButtonId)button);
        bool pressed = 0 != (state.ulButtonPressed & mask);
        bool touched = 0 != (state.ulButtonTouched & mask);
        if (pressed) {
            if (button == vr::k_EButton_ApplicationMenu) {
                _buttonStates.insert(isLeftHand ? LEFT_MENU : RIGHT_MENU);
            } else if (button == vr::k_EButton_Grip) {
                _buttonStates.insert(isLeftHand ? LEFT_GRIP : RIGHT_GRIP);
            } else if (button == vr::k_EButton_SteamVR_Touchpad) {
                _buttonStates.insert(isLeftHand ? LEFT_TOUCHPAD : RIGHT_TOUCHPAD);
            }
        }
        if (touched) {
             if (button == vr::k_EButton_SteamVR_Touchpad) {
                 _buttonStates.insert(isLeftHand ? LEFT_TOUCHPAD_TOUCH : RIGHT_TOUCHPAD_TOUCH);
            }
        }
    }

    // Axes
    for (uint32_t i = 0; i < vr::k_unControllerStateAxisCount; i++) {
        float x = state.rAxis[i].x;
        float y = state.rAxis[i].y;
        int axis = i + vr::k_EButton_Axis0;

        if (axis == vr::k_EButton_SteamVR_Touchpad) {
            _axisStates[isLeftHand ? LEFT_X : RIGHT_X] = x;
            _axisStates[isLeftHand ? LEFT_Y : RIGHT_Y] = y;
        } else if (axis == vr::k_EButton_SteamVR_Trigger) {
            _axisStates[isLeftHand ? LEFT_TRIGGER : RIGHT_TRIGGER] = x;
        }
    }
}

void View::updateRocks() {
    // Rock spawning
    const float ROCK_VEL = 20.0f;
    if (m_action == ROCK && _axisStates[LEFT_TRIGGER] >= 1.0f) {
        m_rockTimeLeft += m_dt;
        m_rockTimeLeft = std::min(1.75f, m_rockTimeLeft);
        if (m_rockTimeLeft == 1.75f) {
            CS123SceneMaterial mat;
            mat.cAmbient = glm::vec4(0.25f*glm::vec3(0.137, 0.094, 0.118), 1);
            mat.cDiffuse = glm::vec4(0.25f*glm::vec3(0.443, 0.263, 0.2), 1);
            mat.cSpecular = glm::vec4(0.25f*glm::vec3(0.773, 0.561, 0.419), 1);
            mat.shininess = 1.0f;

            glm::mat4 M = vrMatrixToQt(m_trackedHandPoses[Hand::LEFT].mDeviceToAbsoluteTracking);
            glm::vec3 pos = glm::vec3(M[3]);
            glm::quat rot = glm::quat_cast(glm::mat3(M * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0))));
            glm::vec3 dir = ROCK_VEL * glm::normalize(glm::quat_cast(glm::mat3(M)) * glm::vec3(0, 0, -1));

            m_worlds[m_world]->getEntities().emplace_back(m_worlds[m_world]->getPhysWorld(),
                                                          ShapeType::SPHERE, 1.0f, btVector3(pos.x, pos.y, pos.z),
                                                          btVector3(0.1, 0.25, 0.1), mat, btQuaternion(rot.x, rot.y, rot.z, rot.w),
                                                          btVector3(dir.x, dir.y, dir.z));
            m_rockTimeLeft = 0.0f;
            _axisStates[LEFT_TRIGGER] = 0.0f;
        }
    } else {
        m_rockTimeLeft = std::max(0.0f, m_rockTimeLeft - 5 * m_dt);
    }

    if (m_action == ROCK && _axisStates[RIGHT_TRIGGER] >= 1.0f) {
        m_rockTimeRight += m_dt;
        m_rockTimeRight = std::min(1.75f, m_rockTimeRight);
        if (m_rockTimeRight == 1.75f) {
            CS123SceneMaterial mat;
            mat.cAmbient = glm::vec4(0.25f*glm::vec3(0.137, 0.094, 0.118), 1);
            mat.cDiffuse = glm::vec4(0.25f*glm::vec3(0.443, 0.263, 0.2), 1);
            mat.cSpecular = glm::vec4(0.25f*glm::vec3(0.773, 0.561, 0.419), 1);
            mat.shininess = 1.0f;

            glm::mat4 M = vrMatrixToQt(m_trackedHandPoses[Hand::RIGHT].mDeviceToAbsoluteTracking);
            glm::vec3 pos = glm::vec3(M[3]);
            glm::quat rot = glm::normalize(glm::quat_cast(glm::mat3(M * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)))));
            glm::vec3 dir = ROCK_VEL * glm::normalize(glm::quat_cast(glm::mat3(M)) * glm::vec3(0, 0, -1));

            m_worlds[m_world]->getEntities().emplace_back(m_worlds[m_world]->getPhysWorld(),
                                                          ShapeType::CONE, 1.0f, btVector3(pos.x, pos.y, pos.z),
                                                          btVector3(0.1, 0.25, 0.1), mat, btQuaternion(rot.x, rot.y, rot.z, rot.w),
                                                          btVector3(dir.x, dir.y, dir.z));
            m_rockTimeRight = 0.0f;
            _axisStates[LEFT_TRIGGER] = 0.0f;
        }
    } else {
        m_rockTimeRight = std::max(0.0f, m_rockTimeRight - 5 * m_dt);
    }
}

void View::tick() {
    // Get the number of seconds since the last tick (variable update rate)
    m_dt = m_time.restart() * 0.001f;
    if (m_dt != 0.0f) m_fps = 0.02f / m_dt + 0.98f * m_fps;

    m_globalTime += m_dt;

    // VR updates
    updatePoses();
    updateInputs();

    // Action select
    if (_buttonStates.find(LEFT_TOUCHPAD) != _buttonStates.end()) {
        m_action = (ActionMode) ((int) (((std::atan2f(_axisStates[LEFT_Y], _axisStates[LEFT_X]) + M_PI) / M_2PI) * NUM_ACTION_MODES));
    } else if (_buttonStates.find(RIGHT_TOUCHPAD) != _buttonStates.end()) {
        m_action = (ActionMode) ((int) (((std::atan2f(_axisStates[RIGHT_Y], _axisStates[RIGHT_X]) + M_PI) / M_2PI) * NUM_ACTION_MODES));
    }

    // Rocks
    updateRocks();

    // Hand information
    glm::mat4 M1 = vrMatrixToQt(m_trackedHandPoses[Hand::RIGHT].mDeviceToAbsoluteTracking);
    glm::vec3 p1 = glm::vec3(M1[3]);
    glm::vec3 d1 = glm::quat(glm::mat3(M1)) * glm::vec3(0, 0, -1);
    glm::mat4 M2 = vrMatrixToQt(m_trackedHandPoses[Hand::LEFT].mDeviceToAbsoluteTracking);
    glm::vec3 p2 = glm::vec3(M2[3]);
    glm::vec3 d2 = glm::quat(glm::mat3(M2)) * glm::vec3(0, 0, -1);

    // Shield movement
    if (m_action == WATER && _axisStates[LEFT_TRIGGER] >= 1.0f && _axisStates[RIGHT_TRIGGER] >= 1.0f) {
        btTransform t;
        t.setIdentity();
        glm::vec3 o = (p1 + p2) / 2.0f;
        float l = glm::length(p1 - p2);
        t.setOrigin(btVector3(o.x, o.y, o.z));
        glm::quat r = glm::quat(glm::mat3(vrMatrixToQt(m_trackedHandPoses[Hand::RIGHT].mDeviceToAbsoluteTracking)));
        t.setRotation(btQuaternion(r.x, r.y, r.z, r.w));
        m_shield->setScale(glm::scale(glm::vec3(0.01f, l, l)));
        m_shield->m_rigidBody->setWorldTransform(t);
        m_shield->m_rigidBody->getMotionState()->setWorldTransform(t);
    } else {
        btTransform t;
        t.setIdentity();
        t.setOrigin(btVector3(0.0f, 10000.0f, 0.0f));
        m_shield->setScale(glm::scale(glm::vec3(1)));
        m_shield->m_rigidBody->setWorldTransform(t);
        m_shield->m_rigidBody->getMotionState()->setWorldTransform(t);
    }

    // Shooting damage entities
    const float LIGHT_TIME = 0.25f;
    const float FIRE_TIME = 0.1f;
    CS123SceneMaterial mat;
    mat.cAmbient = glm::vec4(1);
    mat.cDiffuse = glm::vec4(1);
    mat.cSpecular = glm::vec4(1);
    mat.shininess = 10.0f;
    if ((m_action == LIGHT || m_action == FIRE) && _axisStates[RIGHT_TRIGGER] >= 1.0f) {
        m_rightHandTimer += m_dt;
        if (m_action == LIGHT && m_rightHandTimer > LIGHT_TIME) {
            m_worlds[m_world]->getEntities().emplace_back(m_worlds[m_world]->getPhysWorld(), ShapeType::SPHERE, 1.0f,
                                                          btVector3(p1.x, p1.y, p1.z), btVector3(0.1f, 0.1f, 0.1f),
                                                          mat, btQuaternion(0, 0, 0, 1), btVector3(d1.x, d1.y, d1.z));
            m_worlds[m_world]->getEntities()[m_worlds[m_world]->getEntities().size() - 1].m_rigidBody->setGravity(btVector3(0, 0, 0));
            m_rightHandTimer = 0.0f;
        } else if (m_action == FIRE && m_rightHandTimer > FIRE_TIME) {
            m_worlds[m_world]->getEntities().emplace_back(m_worlds[m_world]->getPhysWorld(), ShapeType::SPHERE, 1.0f,
                                                          btVector3(p1.x, p1.y, p1.z), btVector3(0.1f, 0.1f, 0.1f),
                                                          mat, btQuaternion(0, 0, 0, 1), btVector3(d1.x, d1.y, d1.z));
            m_worlds[m_world]->getEntities()[m_worlds[m_world]->getEntities().size() - 1].m_rigidBody->setGravity(btVector3(0, 0, 0));
            m_rightHandTimer = 0.0f;
        }
    }
    if ((m_action == LIGHT || m_action == FIRE) && _axisStates[LEFT_TRIGGER] >= 1.0f) {
        m_leftHandTimer += m_dt;
        if (m_action == LIGHT && m_leftHandTimer > LIGHT_TIME) {
            m_worlds[m_world]->getEntities().emplace_back(m_worlds[m_world]->getPhysWorld(), ShapeType::SPHERE, 1.0f,
                                                          btVector3(p2.x, p2.y, p2.z), btVector3(0.1f, 0.1f, 0.1f),
                                                          mat, btQuaternion(0, 0, 0, 1), btVector3(d2.x, d2.y, d2.z));
            m_worlds[m_world]->getEntities()[m_worlds[m_world]->getEntities().size() - 1].m_rigidBody->setGravity(btVector3(0, 0, 0));
            m_leftHandTimer = 0.0f;
        } else if (m_action == FIRE && m_leftHandTimer > FIRE_TIME) {
            m_worlds[m_world]->getEntities().emplace_back(m_worlds[m_world]->getPhysWorld(), ShapeType::SPHERE, 1.0f,
                                                          btVector3(p2.x, p2.y, p2.z), btVector3(0.1f, 0.1f, 0.1f),
                                                          mat, btQuaternion(0, 0, 0, 1), btVector3(d2.x, d2.y, d2.z));
            m_worlds[m_world]->getEntities()[m_worlds[m_world]->getEntities().size() - 1].m_rigidBody->setGravity(btVector3(0, 0, 0));
            m_leftHandTimer = 0.0f;
        }
    }

    m_worlds[m_world]->update(m_dt);

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}

void View::switchWorld(WorldState prevWorld) {
    m_worlds[m_world]->makeCurrent();

    if (!m_shield) {
        m_shield = std::make_unique<Entity>(m_worlds[m_world]->getPhysWorld(), ShapeType::CUBE, 0.0f,
                                            btVector3(0.0f, 10000.0f, 0.0f), btVector3(1.0f, 1.0f, 1.0f));
        m_shield->m_rigidBody->setCollisionFlags(m_shield->m_rigidBody->getCollisionFlags() |
                                                 btCollisionObject::CF_KINEMATIC_OBJECT);
        m_shield->m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
    } else {
        m_worlds[prevWorld]->getPhysWorld()->removeRigidBody(m_shield->m_rigidBody.get());
        m_worlds[m_world]->getPhysWorld()->addRigidBody(m_shield->m_rigidBody.get());
    }
}

void View::printFPS() {
    std::cout << m_fps << std::endl;
}
