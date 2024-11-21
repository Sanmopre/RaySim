
INCLUDE_DIRECTORIES( 
		${BULLET_PHYSICS_SOURCE_DIR}/src
		${BULLET_PHYSICS_SOURCE_DIR}/examples
		${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs
		${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/enet/include
		${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/clsocket/src
  )


SET(RobotSimulator_SRCS
        TwoJointMain.cpp
        ../SharedMemory/plugins/collisionFilterPlugin/collisionFilterPlugin.cpp
        ../SharedMemory/plugins/pdControlPlugin/pdControlPlugin.cpp
        ../SharedMemory/plugins/pdControlPlugin/pdControlPlugin.h
        ../SharedMemory/b3RobotSimulatorClientAPI_NoDirect.cpp
        ../SharedMemory/b3RobotSimulatorClientAPI_NoDirect.h
        ../SharedMemory/IKTrajectoryHelper.cpp
        ../SharedMemory/IKTrajectoryHelper.h
        ../ExampleBrowser/InProcessExampleBrowser.cpp
        ../SharedMemory/plugins/tinyRendererPlugin/tinyRendererPlugin.cpp
        ../SharedMemory/plugins/tinyRendererPlugin/TinyRendererVisualShapeConverter.cpp
        ../TinyRenderer/geometry.cpp
        ../TinyRenderer/model.cpp
        ../TinyRenderer/tgaimage.cpp
        ../TinyRenderer/our_gl.cpp
        ../TinyRenderer/TinyRenderer.cpp
        ../SharedMemory/InProcessMemory.cpp
        ../SharedMemory/PhysicsClient.cpp
        ../SharedMemory/PhysicsClient.h
        ../SharedMemory/PhysicsServer.cpp
        ../SharedMemory/PhysicsServer.h
        ../SharedMemory/GraphicsServerExample.cpp
        ../SharedMemory/GraphicsClientExample.cpp
        ../SharedMemory/RemoteGUIHelper.cpp
        ../SharedMemory/RemoteGUIHelperTCP.cpp
        ../SharedMemory/GraphicsServerExample.h
        ../SharedMemory/GraphicsClientExample.h
        ../SharedMemory/RemoteGUIHelper.h
        ../SharedMemory/GraphicsSharedMemoryCommands.h
        ../SharedMemory/GraphicsSharedMemoryPublic.h
        ../SharedMemory/PhysicsServerExample.cpp
        ../SharedMemory/PhysicsServerExampleBullet2.cpp
        ../SharedMemory/SharedMemoryInProcessPhysicsC_API.cpp
        ../SharedMemory/PhysicsServerSharedMemory.cpp
        ../SharedMemory/PhysicsServerSharedMemory.h
        ../SharedMemory/PhysicsDirect.cpp
        ../SharedMemory/PhysicsDirect.h
        ../SharedMemory/PhysicsDirectC_API.cpp
        ../SharedMemory/PhysicsDirectC_API.h
        ../SharedMemory/PhysicsServerCommandProcessor.cpp
        ../SharedMemory/PhysicsServerCommandProcessor.h
        ../SharedMemory/b3PluginManager.cpp

        ../SharedMemory/PhysicsClientSharedMemory.cpp
        ../SharedMemory/PhysicsClientSharedMemory.h
        ../SharedMemory/PhysicsClientSharedMemory_C_API.cpp
        ../SharedMemory/PhysicsClientSharedMemory_C_API.h
        ../SharedMemory/PhysicsClientC_API.cpp
        ../SharedMemory/PhysicsClientC_API.h
        ../SharedMemory/Win32SharedMemory.cpp
        ../SharedMemory/Win32SharedMemory.h
        ../SharedMemory/PosixSharedMemory.cpp
        ../SharedMemory/PosixSharedMemory.h
        ../Utils/b3ResourcePath.cpp
        ../Utils/b3ResourcePath.h
        ../Utils/RobotLoggingUtil.cpp
        ../Utils/RobotLoggingUtil.h
        ../ThirdPartyLibs/tinyxml2/tinyxml2.cpp
        ../ThirdPartyLibs/Wavefront/tiny_obj_loader.cpp
        ../ThirdPartyLibs/Wavefront/tiny_obj_loader.h
        ../ThirdPartyLibs/stb_image/stb_image.cpp
        ../Importers/ImportColladaDemo/LoadMeshFromCollada.cpp
        ../Importers/ImportObjDemo/LoadMeshFromObj.cpp
        ../Importers/ImportObjDemo/Wavefront2GLInstanceGraphicsShape.cpp
        ../Importers/ImportMJCFDemo/BulletMJCFImporter.cpp
        ../Importers/ImportURDFDemo/BulletUrdfImporter.cpp
        ../Importers/ImportURDFDemo/MyMultiBodyCreator.cpp
        ../Importers/ImportURDFDemo/URDF2Bullet.cpp
        ../Importers/ImportURDFDemo/UrdfParser.cpp
        ../Importers/ImportURDFDemo/urdfStringSplit.cpp
        ../Importers/ImportMeshUtility/b3ImportMeshUtility.cpp
        ../MultiThreading/b3PosixThreadSupport.cpp
        ../MultiThreading/b3Win32ThreadSupport.cpp
        ../MultiThreading/b3ThreadSupportInterface.cpp
		
)

IF(BUILD_CLSOCKET)
 ADD_DEFINITIONS(-DBT_ENABLE_CLSOCKET)
ENDIF(BUILD_CLSOCKET)

IF(WIN32)
	LINK_LIBRARIES(
		${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY}
	)
	IF(BUILD_ENET)
		ADD_DEFINITIONS(-DWIN32 -DBT_ENABLE_ENET)
	ENDIF(BUILD_ENET)
	IF(BUILD_CLSOCKET)
		ADD_DEFINITIONS(-DWIN32)
	ENDIF(BUILD_CLSOCKET)

ELSE(WIN32)
	IF(BUILD_ENET)
		ADD_DEFINITIONS(-DHAS_SOCKLEN_T -DBT_ENABLE_ENET)
	ENDIF(BUILD_ENET)

	IF(BUILD_CLSOCKET)	
		ADD_DEFINITIONS(${OSDEF})
	ENDIF(BUILD_CLSOCKET)
ENDIF(WIN32)


IF(BUILD_ENET)
	set(RobotSimulator_SRCS ${RobotSimulator_SRCS}
            ../SharedMemory/PhysicsClientUDP.cpp
            ../SharedMemory/PhysicsClientUDP_C_API.cpp
            ../SharedMemory/PhysicsClientUDP.h
            ../SharedMemory/PhysicsClientUDP_C_API.h
            ../ThirdPartyLibs/enet/win32.c
            ../ThirdPartyLibs/enet/unix.c
            ../ThirdPartyLibs/enet/callbacks.c
            ../ThirdPartyLibs/enet/compress.c
            ../ThirdPartyLibs/enet/host.c
            ../ThirdPartyLibs/enet/list.c
            ../ThirdPartyLibs/enet/packet.c
            ../ThirdPartyLibs/enet/peer.c
            ../ThirdPartyLibs/enet/protocol.c
	)
ENDIF(BUILD_ENET)

IF(BUILD_CLSOCKET)
	set(RobotSimulator_SRCS ${RobotSimulator_SRCS}
            ../SharedMemory/PhysicsClientTCP.cpp
            ../SharedMemory/PhysicsClientTCP.h
            ../SharedMemory/PhysicsClientTCP_C_API.cpp
            ../SharedMemory/PhysicsClientTCP_C_API.h
            ../ThirdPartyLibs/clsocket/src/SimpleSocket.cpp
            ../ThirdPartyLibs/clsocket/src/ActiveSocket.cpp
            ../ThirdPartyLibs/clsocket/src/PassiveSocket.cpp
	)
ENDIF()


#some code to support OpenGL and Glew cross platform
IF (WIN32)
	INCLUDE_DIRECTORIES(
			${BULLET_PHYSICS_SOURCE_DIR}/btgui/OpenGLWindow/GlewWindows
	)
	ADD_DEFINITIONS(-DGLEW_STATIC)
	LINK_LIBRARIES( ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} )
ELSE(WIN32)
	IF(APPLE)
		find_library(COCOA NAMES Cocoa)
		MESSAGE(${COCOA})
		link_libraries(${COCOA} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})

	ELSE(APPLE)
		INCLUDE_DIRECTORIES(
				${BULLET_PHYSICS_SOURCE_DIR}/btgui/OpenGLWindow/GlewWindows
		)
		ADD_DEFINITIONS("-DGLEW_INIT_OPENGL11_FUNCTIONS=1")
		ADD_DEFINITIONS("-DGLEW_STATIC")
		ADD_DEFINITIONS("-DGLEW_DYNAMIC_LOAD_ALL_GLX_FUNCTIONS=1")

		LINK_LIBRARIES(  pthread ${DL} )
	ENDIF(APPLE)
ENDIF(WIN32)


ADD_EXECUTABLE(App_TwoJoint  ${RobotSimulator_SRCS})

SET_TARGET_PROPERTIES(App_TwoJoint PROPERTIES VERSION ${BULLET_VERSION})
SET_TARGET_PROPERTIES(App_TwoJoint PROPERTIES DEBUG_POSTFIX "_d")


IF(WIN32)
	IF(BUILD_ENET OR BUILD_CLSOCKET)
		TARGET_LINK_LIBRARIES(App_TwoJoint ws2_32 )
	ENDIF(BUILD_ENET OR BUILD_CLSOCKET)
ENDIF(WIN32)



TARGET_LINK_LIBRARIES(App_TwoJoint BulletExampleBrowserLib BulletFileLoader BulletWorldImporter BulletSoftBody BulletDynamics BulletCollision BulletInverseDynamicsUtils BulletInverseDynamics LinearMath OpenGLWindow gwen BussIK Bullet3Common)


