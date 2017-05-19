# -------------------------------------------------
# Project created by QtCreator 2010-08-22T14:12:19
# -------------------------------------------------
QT += opengl xml
TARGET = manus
TEMPLATE = app

win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

SOURCES += \
    camera/Player.cpp \
    game/Entity.cpp \
    game/World.cpp \
    game/DemoWorld.cpp \
    game/LightWorld.cpp \
    game/WaterWorld.cpp \
    game/RockWorld.cpp \
    game/PhysicsWorld.cpp \
    gl/GLDebug.cpp \
    gl/datatype/VBOAttribMarker.cpp \
    gl/datatype/VBO.cpp \
    gl/datatype/IBO.cpp \
    gl/datatype/VAO.cpp \
    gl/datatype/FBO.cpp \
    gl/textures/Texture.cpp \
    gl/textures/Texture2D.cpp \
    gl/textures/TextureParameters.cpp \
    gl/textures/TextureParametersBuilder.cpp \
    gl/textures/RenderBuffer.cpp \
    gl/textures/DepthBuffer.cpp \
    gl/shaders/Shader.cpp \
    gl/shaders/CS123Shader.cpp \
    lib/ResourceLoader.cpp \
    particles/ParticleSystem.cpp \
    shapes/FullScreenQuad.cpp \
    shapes/SphereMesh.cpp \
    shapes/CubeMesh.cpp \
    shapes/ConeMesh.cpp \
    shapes/CylinderMesh.cpp \
    shapes/ParameterizedMesh.tpp \
    shapes/ParameterizedMesh.cpp \
    shapes/Mesh.cpp \
    ui/view.cpp \
    ui/viewformat.cpp \
    ui/mainwindow.cpp \
    glew-1.10.0/src/glew.c \
    main.cpp \
    game/Enemy.cpp \
    game/HangingEnemy.cpp \
    game/MarchingEnemy.cpp

HEADERS += \
    camera/Player.h \
    game/Entity.h \
    game/World.h \
    game/DemoWorld.h \
    game/LightWorld.h \
    game/WaterWorld.h \
    game/RockWorld.h \
    game/PhysicsWorld.h \
    gl/GLDebug.h \
    gl/datatype/VBOAttribMarker.h \
    gl/datatype/VBO.h \
    gl/datatype/IBO.h \
    gl/datatype/VAO.h \
    gl/datatype/FBO.h \
    gl/textures/Texture.h \
    gl/textures/Texture2D.h \
    gl/textures/TextureParameters.h \
    gl/textures/TextureParametersBuilder.h \
    gl/textures/RenderBuffer.h \
    gl/textures/DepthBuffer.h \
    gl/shaders/Shader.h \
    gl/shaders/ShaderAttribLocations.h \
    gl/shaders/CS123Shader.h \
    lib/ResourceLoader.h \
    lib/CS123SceneData.h \
    particles/ParticleSystem.h \
    shapes/FullScreenQuad.h \
    shapes/SphereMesh.h \
    shapes/CubeMesh.h \
    shapes/ConeMesh.h \
    shapes/CylinderMesh.h \
    shapes/ParameterizedMesh.h \
    shapes/Mesh.h \
    ui/view.h \
    ui/viewformat.h \
    ui/mainwindow.h \
    ui_mainwindow.h \
    glew-1.10.0/include/GL/glew.h \
    game/Enemy.h \
    game/HangingEnemy.h \
    game/MarchingEnemy.h

FORMS += ui/mainwindow.ui
INCLUDEPATH += camera game glm lib libraries libraries/bullet3/src libraries/openvr/headers particles shapes ui glew-1.10.0/include
DEPENDPATH += camera game glm lib libraries libraries/bullet3/src libraries/openvr/headers particles shapes ui glew-1.10.0/include

DEFINES += _USE_MATH_DEFINES
DEFINES += TIXML_USE_STL
DEFINES += GLM_SWIZZLE GLM_FORCE_RADIANS
OTHER_FILES += \
    images/* \
    shaders/*

RESOURCES += \
    resources.qrc

# Bullet (better way to do this????)
SOURCES += \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btDispatcher.cpp \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btDbvt.cpp \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btUnionFind.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btManifoldResult.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btGhostObject.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btCollisionWorld.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btCollisionObject.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btUniformScalingShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleMesh.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleCallback.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleBuffer.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTetrahedronShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btSphereShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btShapeHull.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btOptimizedBvh.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btMultiSphereShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btEmptyShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btCylinderShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvexShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvexInternalShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvexHullShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvex2dShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConeShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConcaveShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btCompoundShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btCollisionShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btCapsuleShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btBoxShape.cpp \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btBox2dShape.cpp \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_tri_collision.cpp \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_memory.cpp \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_contact.cpp \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_box_set.cpp \
    libraries/bullet3/src/BulletCollision/Gimpact/btTriangleShapeEx.cpp \
    libraries/bullet3/src/BulletCollision/Gimpact/btGImpactShape.cpp \
    libraries/bullet3/src/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp \
    libraries/bullet3/src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp \
    libraries/bullet3/src/BulletCollision/Gimpact/btGImpactBvh.cpp \
    libraries/bullet3/src/BulletCollision/Gimpact/btGenericPoolAllocator.cpp \
    libraries/bullet3/src/BulletCollision/Gimpact/btContactProcessing.cpp \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
    libraries/bullet3/src/BulletDynamics/Character/btKinematicCharacterController.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btContactConstraint.cpp \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp \
    libraries/bullet3/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp \
    libraries/bullet3/src/BulletDynamics/Dynamics/btRigidBody.cpp \
    libraries/bullet3/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp \
    libraries/bullet3/src/BulletDynamics/Vehicle/btWheelInfo.cpp \
    libraries/bullet3/src/BulletDynamics/Vehicle/btRaycastVehicle.cpp \
    libraries/bullet3/src/LinearMath/btSerializer.cpp \
    libraries/bullet3/src/LinearMath/btQuickprof.cpp \
    libraries/bullet3/src/LinearMath/btGeometryUtil.cpp \
    libraries/bullet3/src/LinearMath/btConvexHullComputer.cpp \
    libraries/bullet3/src/LinearMath/btConvexHull.cpp \
    libraries/bullet3/src/LinearMath/btAlignedAllocator.cpp \
    libraries/bullet3/src/LinearMath/btVector3.cpp

HEADERS += \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btQuantizedBvh.h \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btDispatcher.h \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btDbvt.h \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.h \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h \
    libraries/bullet3/src/BulletCollision/BroadphaseCollision/btAxisSweep3.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/SphereTriangleDetector.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btUnionFind.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btSimulationIslandManager.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btManifoldResult.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btGhostObject.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btCollisionWorld.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btCollisionObject.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btCollisionDispatcher.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btCollisionCreateFunc.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btCollisionConfiguration.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btBoxBoxDetector.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h \
    libraries/bullet3/src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btUniformScalingShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleMeshShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleMesh.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleInfoMap.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleCallback.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTriangleBuffer.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btTetrahedronShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btStridingMeshInterface.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btStaticPlaneShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btSphereShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btShapeHull.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btOptimizedBvh.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btMultiSphereShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btMinkowskiSumShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btMaterial.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btEmptyShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btCylinderShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvexShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvexPointCloudShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvexInternalShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvexHullShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConvex2dShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConeShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btConcaveShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btCompoundShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btCollisionShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btCollisionMargin.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btCapsuleShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btBoxShape.h \
    libraries/bullet3/src/BulletCollision/CollisionShapes/btBox2dShape.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_tri_collision.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_radixsort.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_memory.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_math.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_linear_math.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_hash_table.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_geometry.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_geom_types.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_contact.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_clip_polygon.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_box_set.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_box_collision.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_bitset.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_basic_geometry_operations.h \
    libraries/bullet3/src/BulletCollision/Gimpact/gim_array.h \
    libraries/bullet3/src/BulletCollision/Gimpact/btTriangleShapeEx.h \
    libraries/bullet3/src/BulletCollision/Gimpact/btQuantization.h \
    libraries/bullet3/src/BulletCollision/Gimpact/btGImpactShape.h \
    libraries/bullet3/src/BulletCollision/Gimpact/btGImpactQuantizedBvh.h \
    libraries/bullet3/src/BulletCollision/Gimpact/btGImpactMassUtil.h \
    libraries/bullet3/src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h \
    libraries/bullet3/src/BulletCollision/Gimpact/btGImpactBvh.h \
    libraries/bullet3/src/BulletCollision/Gimpact/btGeometryOperations.h \
    libraries/bullet3/src/BulletCollision/Gimpact/btGenericPoolAllocator.h \
    libraries/bullet3/src/BulletCollision/Gimpact/btContactProcessing.h \
    libraries/bullet3/src/BulletCollision/Gimpact/btClipPolygon.h \
    libraries/bullet3/src/BulletCollision/Gimpact/btBoxCollision.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btSimplexSolverInterface.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btPointCollector.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btManifoldPoint.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btConvexCast.h \
    libraries/bullet3/src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.h \
    libraries/bullet3/src/BulletDynamics/Character/btKinematicCharacterController.h \
    libraries/bullet3/src/BulletDynamics/Character/btCharacterControllerInterface.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btUniversalConstraint.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btTypedConstraint.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btSolverConstraint.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btSolverBody.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btSliderConstraint.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btJacobianEntry.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btHingeConstraint.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btHinge2Constraint.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btContactSolverInfo.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btContactConstraint.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btConstraintSolver.h \
    libraries/bullet3/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.h \
    libraries/bullet3/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.h \
    libraries/bullet3/src/BulletDynamics/Dynamics/btRigidBody.h \
    libraries/bullet3/src/BulletDynamics/Dynamics/btDynamicsWorld.h \
    libraries/bullet3/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h \
    libraries/bullet3/src/BulletDynamics/Dynamics/btActionInterface.h \
    libraries/bullet3/src/BulletDynamics/Vehicle/btWheelInfo.h \
    libraries/bullet3/src/BulletDynamics/Vehicle/btVehicleRaycaster.h \
    libraries/bullet3/src/BulletDynamics/Vehicle/btRaycastVehicle.h \
    libraries/bullet3/src/LinearMath/btVector3.h \
    libraries/bullet3/src/LinearMath/btTransformUtil.h \
    libraries/bullet3/src/LinearMath/btTransform.h \
    libraries/bullet3/src/LinearMath/btStackAlloc.h \
    libraries/bullet3/src/LinearMath/btSerializer.h \
    libraries/bullet3/src/LinearMath/btScalar.h \
    libraries/bullet3/src/LinearMath/btRandom.h \
    libraries/bullet3/src/LinearMath/btQuickprof.h \
    libraries/bullet3/src/LinearMath/btQuaternion.h \
    libraries/bullet3/src/LinearMath/btQuadWord.h \
    libraries/bullet3/src/LinearMath/btPoolAllocator.h \
    libraries/bullet3/src/LinearMath/btMotionState.h \
    libraries/bullet3/src/LinearMath/btMinMax.h \
    libraries/bullet3/src/LinearMath/btMatrix3x3.h \
    libraries/bullet3/src/LinearMath/btList.h \
    libraries/bullet3/src/LinearMath/btIDebugDraw.h \
    libraries/bullet3/src/LinearMath/btHashMap.h \
    libraries/bullet3/src/LinearMath/btGeometryUtil.h \
    libraries/bullet3/src/LinearMath/btDefaultMotionState.h \
    libraries/bullet3/src/LinearMath/btConvexHullComputer.h \
    libraries/bullet3/src/LinearMath/btConvexHull.h \
    libraries/bullet3/src/LinearMath/btAlignedObjectArray.h \
    libraries/bullet3/src/LinearMath/btAlignedAllocator.h \
    libraries/bullet3/src/LinearMath/btAabbUtil2.h

defineTest(copyToDestdir) {
    files = $$1
    for(FILE, files) {
        CONFIG(debug, debug|release) {
            DDIR = $${OUT_PWD}/debug
        } else {
            DDIR = $${OUT_PWD}/release
        }

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g
        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }
    export(QMAKE_POST_LINK)
}

win32 {
    contains(QT_ARCH, i386) {
        message("32 bit build")
            LIBS += -L$$PWD/libraries/openvr/lib/win32/ -lopenvr_api
            copyToDestdir($$PWD/libraries/openvr/bin/win32/openvr_api.dll)
    } else {
        message("64 bit build")
            LIBS += -L$$PWD/libraries/openvr/lib/win64/ -lopenvr_api
            copyToDestdir($$PWD/libraries/openvr/bin/win64/openvr_api.dll)
    }
}
