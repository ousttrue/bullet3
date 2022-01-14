ACTIVATION_STATE_DISABLE_SLEEPING: int
ACTIVATION_STATE_DISABLE_WAKEUP: int
ACTIVATION_STATE_ENABLE_SLEEPING: int
ACTIVATION_STATE_ENABLE_WAKEUP: int
ACTIVATION_STATE_SLEEP: int
ACTIVATION_STATE_WAKE_UP: int
AddFileIOAction: int
B3G_ALT: int
B3G_BACKSPACE: int
B3G_CONTROL: int
B3G_DELETE: int
B3G_DOWN_ARROW: int
B3G_END: int
B3G_F1: int
B3G_F10: int
B3G_F11: int
B3G_F12: int
B3G_F13: int
B3G_F14: int
B3G_F15: int
B3G_F2: int
B3G_F3: int
B3G_F4: int
B3G_F5: int
B3G_F6: int
B3G_F7: int
B3G_F8: int
B3G_F9: int
B3G_HOME: int
B3G_INSERT: int
B3G_LEFT_ARROW: int
B3G_PAGE_DOWN: int
B3G_PAGE_UP: int
B3G_RETURN: int
B3G_RIGHT_ARROW: int
B3G_SHIFT: int
B3G_SPACE: int
B3G_UP_ARROW: int
CNSFileIO: int
CONSTRAINT_SOLVER_LCP_DANTZIG: int
CONSTRAINT_SOLVER_LCP_PGS: int
CONSTRAINT_SOLVER_LCP_SI: int
CONTACT_RECOMPUTE_CLOSEST: int
CONTACT_REPORT_EXISTING: int
COV_ENABLE_DEPTH_BUFFER_PREVIEW: int
COV_ENABLE_GUI: int
COV_ENABLE_KEYBOARD_SHORTCUTS: int
COV_ENABLE_MOUSE_PICKING: int
COV_ENABLE_PLANAR_REFLECTION: int
COV_ENABLE_RENDERING: int
COV_ENABLE_RGB_BUFFER_PREVIEW: int
COV_ENABLE_SEGMENTATION_MARK_PREVIEW: int
COV_ENABLE_SHADOWS: int
COV_ENABLE_SINGLE_STEP_RENDERING: int
COV_ENABLE_TINY_RENDERER: int
COV_ENABLE_VR_PICKING: int
COV_ENABLE_VR_RENDER_CONTROLLERS: int
COV_ENABLE_VR_TELEPORTING: int
COV_ENABLE_WIREFRAME: int
COV_ENABLE_Y_AXIS_UP: int
DIRECT: int
ER_BULLET_HARDWARE_OPENGL: int
ER_NO_SEGMENTATION_MASK: int
ER_SEGMENTATION_MASK_OBJECT_AND_LINKINDEX: int
ER_TINY_RENDERER: int
ER_USE_PROJECTIVE_TEXTURE: int
GEOM_BOX: int
GEOM_CAPSULE: int
GEOM_CONCAVE_INTERNAL_EDGE: int
GEOM_CYLINDER: int
GEOM_FORCE_CONCAVE_TRIMESH: int
GEOM_HEIGHTFIELD: int
GEOM_MESH: int
GEOM_PLANE: int
GEOM_SPHERE: int
GRAPHICS_CLIENT: int
GRAPHICS_SERVER: int
GRAPHICS_SERVER_MAIN_THREAD: int
GRAPHICS_SERVER_TCP: int
GUI: int
GUI_MAIN_THREAD: int
GUI_SERVER: int
IK_DLS: int
IK_HAS_JOINT_DAMPING: int
IK_HAS_NULL_SPACE_VELOCITY: int
IK_HAS_TARGET_ORIENTATION: int
IK_HAS_TARGET_POSITION: int
IK_SDLS: int
JOINT_FEEDBACK_IN_JOINT_FRAME: int
JOINT_FEEDBACK_IN_WORLD_SPACE: int
JOINT_FIXED: int
JOINT_GEAR: int
JOINT_PLANAR: int
JOINT_POINT2POINT: int
JOINT_PRISMATIC: int
JOINT_REVOLUTE: int
JOINT_SPHERICAL: int
KEY_IS_DOWN: int
KEY_WAS_RELEASED: int
KEY_WAS_TRIGGERED: int
LINK_FRAME: int
MAX_RAY_INTERSECTION_BATCH_SIZE: int
MESH_DATA_SIMULATION_MESH: int
MJCF_COLORS_FROM_FILE: int
PD_CONTROL: int
POSITION_CONTROL: int
PosixFileIO: int
RESET_USE_DEFORMABLE_WORLD: int
RESET_USE_DISCRETE_DYNAMICS_WORLD: int
RESET_USE_SIMPLE_BROADPHASE: int
RemoveFileIOAction: int
SENSOR_FORCE_TORQUE: int
SHARED_MEMORY: int
SHARED_MEMORY_GUI: int
SHARED_MEMORY_KEY: int
SHARED_MEMORY_KEY2: int
SHARED_MEMORY_SERVER: int
STABLE_PD_CONTROL: int
STATE_LOGGING_ALL_COMMANDS: int
STATE_LOGGING_CONTACT_POINTS: int
STATE_LOGGING_CUSTOM_TIMER: int
STATE_LOGGING_GENERIC_ROBOT: int
STATE_LOGGING_MINITAUR: int
STATE_LOGGING_PROFILE_TIMINGS: int
STATE_LOGGING_VIDEO_MP4: int
STATE_LOGGING_VR_CONTROLLERS: int
STATE_LOG_JOINT_MOTOR_TORQUES: int
STATE_LOG_JOINT_TORQUES: int
STATE_LOG_JOINT_USER_TORQUES: int
STATE_REPLAY_ALL_COMMANDS: int
TCP: int
TORQUE_CONTROL: int
UDP: int
URDF_ENABLE_CACHED_GRAPHICS_SHAPES: int
URDF_ENABLE_SLEEPING: int
URDF_ENABLE_WAKEUP: int
URDF_GLOBAL_VELOCITIES_MB: int
URDF_GOOGLEY_UNDEFINED_COLORS: int
URDF_IGNORE_COLLISION_SHAPES: int
URDF_IGNORE_VISUAL_SHAPES: int
URDF_INITIALIZE_SAT_FEATURES: int
URDF_MAINTAIN_LINK_ORDER: int
URDF_MERGE_FIXED_LINKS: int
URDF_PRINT_URDF_INFO: int
URDF_USE_IMPLICIT_CYLINDER: int
URDF_USE_INERTIA_FROM_FILE: int
URDF_USE_MATERIAL_COLORS_FROM_MTL: int
URDF_USE_MATERIAL_TRANSPARANCY_FROM_MTL: int
URDF_USE_SELF_COLLISION: int
URDF_USE_SELF_COLLISION_EXCLUDE_ALL_PARENTS: int
URDF_USE_SELF_COLLISION_EXCLUDE_PARENT: int
URDF_USE_SELF_COLLISION_INCLUDE_PARENT: int
VELOCITY_CONTROL: int
VISUAL_SHAPE_DATA_TEXTURE_UNIQUE_IDS: int
VISUAL_SHAPE_DOUBLE_SIDED: int
VR_BUTTON_IS_DOWN: int
VR_BUTTON_WAS_RELEASED: int
VR_BUTTON_WAS_TRIGGERED: int
VR_CAMERA_TRACK_OBJECT_ORIENTATION: int
VR_DEVICE_CONTROLLER: int
VR_DEVICE_GENERIC_TRACKER: int
VR_DEVICE_HMD: int
VR_MAX_BUTTONS: int
VR_MAX_CONTROLLERS: int
WORLD_FRAME: int
ZipFileIO: int
def addUserData(*args):
     '''
     addUserData(bodyUniqueId, key, value, linkIndex=-1, visualShapeIndex=-1, physicsClientId=0)
     Adds or updates a user data entry. Returns user data identifier.
     '''
     ...
def addUserDebugLine(*args):
     '''
     Add a user debug draw line with lineFrom[3], lineTo[3], lineColorRGB[3], lineWidth, lifeTime. A lifeTime of 0 means permanent until removed. Returns a unique id for the user debug item.
     '''
     ...
def addUserDebugParameter(*args):
     '''
     Add a user debug parameter, such as a slider, that can be controlled using a GUI.
     '''
     ...
def addUserDebugText(*args):
     '''
     Add a user debug draw line with text, textPosition[3], textSize and lifeTime in seconds A lifeTime of 0 means permanent until removed. Returns a unique id for the user debug item.
     '''
     ...
def applyExternalForce(*args):
     '''
     for objectUniqueId, linkIndex (-1 for base/root link), apply a force [x,y,z] at the a position [x,y,z], flag to select FORCE_IN_LINK_FRAME or WORLD_FRAME coordinates
     '''
     ...
def applyExternalTorque(*args):
     '''
     for objectUniqueId, linkIndex (-1 for base/root link) apply a torque [x,y,z] in Cartesian coordinates, flag to select TORQUE_IN_LINK_FRAME or WORLD_FRAME coordinates
     '''
     ...
def calculateInverseDynamics(*args):
     '''
     Given an object id, joint positions, joint velocities and joint accelerations, compute the joint forces using Inverse Dynamics
     '''
     ...
def calculateInverseKinematics(*args):
     '''
     Inverse Kinematics bindings: Given an object id, current joint positions and target position for the end effector,compute the inverse kinematics and return the new joint state
     '''
     ...
def calculateInverseKinematics2(*args):
     '''
     Inverse Kinematics bindings: Given an object id, current joint positions and target positions for the end effectors,compute the inverse kinematics and return the new joint state
     '''
     ...
def calculateJacobian(*args):
     '''
     linearJacobian, angularJacobian = calculateJacobian(bodyUniqueId, linkIndex, localPosition, objPositions, objVelocities, objAccelerations, physicsClientId=0)
     Compute the jacobian for a specified local position on a body and its kinematics.
     Args:
       bodyIndex - a scalar defining the unique object id.
       linkIndex - a scalar identifying the link containing the local point.
       localPosition - a list of [x, y, z] of the coordinates defined in the link frame.
       objPositions - a list of the joint positions.
       objVelocities - a list of the joint velocities.
       objAccelerations - a list of the joint accelerations.
     Returns:
       linearJacobian - a list of the partial linear velocities of the jacobian.
       angularJacobian - a list of the partial angular velocities of the jacobian.
     '''
     ...
def calculateMassMatrix(*args):
     '''
     massMatrix = calculateMassMatrix(bodyUniqueId, objPositions, physicsClientId=0)
     Compute the mass matrix for an object and its chain of bodies.
     Args:
       bodyIndex - a scalar defining the unique object id.
       objPositions - a list of the joint positions.
     Returns:
       massMatrix - a list of lists of the mass matrix components.
     '''
     ...
def calculateVelocityQuaternion(*args):
     '''
     Compute the angular velocity given start and end quaternion and delta time.
     '''
     ...
def changeConstraint(*args):
     '''
     Change some parameters of an existing constraint, such as the child pivot or child frame orientation, using its unique id.
     '''
     ...
def changeDynamics(*args):
     '''
     change dynamics information such as mass, lateral friction coefficient.
     '''
     ...
def changeTexture(*args):
     '''
     Change a texture file.
     '''
     ...
def changeVisualShape(*args):
     '''
     Change part of the visual shape information for one object.
     '''
     ...
def computeDofCount(*args):
     '''
     computeDofCount returns the number of degrees of freedom, including 7 degrees of freedom for the base in case of floating base
     '''
     ...
def computeProjectionMatrix(*args):
     '''
     Compute a camera projection matrix from screen left/right/bottom/top/near/far values
     '''
     ...
def computeProjectionMatrixFOV(*args):
     '''
     Compute a camera projection matrix from fov, aspect ratio, near, far values
     '''
     ...
def computeViewMatrix(*args):
     '''
     Compute a camera viewmatrix from camera eye,  target position and up vector 
     '''
     ...
def computeViewMatrixFromYawPitchRoll(*args):
     '''
     Compute a camera viewmatrix from camera eye,  target position and up vector 
     '''
     ...
def configureDebugVisualizer(*args):
     '''
     For the 3D OpenGL Visualizer, enable/disable GUI, shadows.
     '''
     ...
def connect(*args):
     '''
     connect(method, key=SHARED_MEMORY_KEY, options='')
     connect(method, hostname='localhost', port=1234, options='')
     Connect to an existing physics server (using shared memory by default).
     '''
     ...
def createCollisionShape(*args):
     '''
     Create a collision shape. Returns a non-negative (int) unique id, if successfull, negative otherwise.
     '''
     ...
def createCollisionShapeArray(*args):
     '''
     Create collision shapes. Returns a non-negative (int) unique id, if successfull, negative otherwise.
     '''
     ...
def createConstraint(*args):
     '''
     Create a constraint between two bodies. Returns a (int) unique id, if successfull.
     '''
     ...
def createMultiBody(*args):
     '''
     Create a multi body. Returns a non-negative (int) unique id, if successfull, negative otherwise.
     '''
     ...
def createSoftBodyAnchor(*args):
     '''
     Create an anchor (attachment) between a soft body and a rigid or multi body.
     '''
     ...
def createVisualShape(*args):
     '''
     Create a visual shape. Returns a non-negative (int) unique id, if successfull, negative otherwise.
     '''
     ...
def createVisualShapeArray(*args):
     '''
     Create visual shapes. Returns a non-negative (int) unique id, if successfull, negative otherwise.
     '''
     ...
def disconnect(*args):
     '''
     disconnect(physicsClientId=0)
     Disconnect from the physics server.
     '''
     ...
def enableJointForceTorqueSensor(*args):
     '''
     Enable or disable a joint force/torque sensor measuring the joint reaction forces.
     '''
     ...
def executePluginCommand(*args):
     '''
     Execute a command, implemented in a plugin.
     '''
     ...
def getAABB(*args):
     '''
     Get the axis aligned bound box min and max coordinates in world space.
     '''
     ...
def getAPIVersion(*args):
     '''
     Get version of the API. Compatibility exists for connections using the same API version. Make sure both client and server use the same number of bits (32-bit or 64bit).
     '''
     ...
def getAxisAngleFromQuaternion(*args):
     '''
     Compute the quaternion from axis and angle representation.
     '''
     ...
def getAxisDifferenceQuaternion(*args):
     '''
     Compute the velocity axis difference from two quaternions.
     '''
     ...
def getBasePositionAndOrientation(*args):
     '''
     Get the world position and orientation of the base of the object. (x,y,z) position vector and (x,y,z,w) quaternion orientation.
     '''
     ...
def getBaseVelocity(*args):
     '''
     Get the linear and angular velocity of the base of the object  in world space coordinates. (x,y,z) linear velocity vector and (x,y,z) angular velocity vector.
     '''
     ...
def getBodyInfo(*args):
     '''
     Get the body info, given a body unique id.
     '''
     ...
def getBodyUniqueId(*args):
     '''
     getBodyUniqueId is used after connecting to server with existing bodies.Get the unique id of the body, given a integer range [0.. number of bodies).
     '''
     ...
def getCameraImage(*args):
     '''
     Render an image (given the pixel resolution width, height, camera viewMatrix , projectionMatrix, lightDirection, lightColor, lightDistance, shadow, lightAmbientCoeff, lightDiffuseCoeff, lightSpecularCoeff, and renderer), and return the 8-8-8bit RGB pixel data and floating point depth values
     '''
     ...
def getClosestPoints(*args):
     '''
     Compute the closest points between two objects, if the distance is below a given threshold.Input is two objects unique ids and distance threshold.
     '''
     ...
def getCollisionShapeData(*args):
     '''
     Return the collision shape information for one object.
     '''
     ...
def getConnectionInfo(*args):
     '''
     getConnectionInfo(physicsClientId=0)
     Return if a given client id is connected, and using what method.
     '''
     ...
def getConstraintInfo(*args):
     '''
     Get the user-created constraint info, given a constraint unique id.
     '''
     ...
def getConstraintState(*args):
     '''
     Get the user-created constraint state (applied forces), given a constraint unique id.
     '''
     ...
def getConstraintUniqueId(*args):
     '''
     Get the unique id of the constraint, given a integer index in range [0.. number of constraints).
     '''
     ...
def getContactPoints(*args):
     '''
     Return existing contact points after the stepSimulation command. Optional arguments one or two object unique ids, that need to be involved in the contact.
     '''
     ...
def getDebugVisualizerCamera(*args):
     '''
     Get information about the 3D visualizer camera, such as width, height, view matrix, projection matrix etc.
     '''
     ...
def getDifferenceQuaternion(*args):
     '''
     Compute the quaternion difference from two quaternions.
     '''
     ...
def getDynamicsInfo(*args):
     '''
     Get dynamics information such as mass, lateral friction coefficient.
     '''
     ...
def getEulerFromQuaternion(*args):
     '''
     Convert quaternion [x,y,z,w] to Euler [roll, pitch, yaw] as in URDF/SDF convention
     '''
     ...
def getJointInfo(*args):
     '''
     Get the name and type info for a joint on a body.
     '''
     ...
def getJointState(*args):
     '''
     Get the state (position, velocity etc) for a joint on a body.
     '''
     ...
def getJointStateMultiDof(*args):
     '''
     Get the state (position, velocity etc) for a joint on a body. (supports planar and spherical joints)
     '''
     ...
def getJointStates(*args):
     '''
     Get the state (position, velocity etc) for multiple joints on a body.
     '''
     ...
def getJointStatesMultiDof(*args):
     '''
     Get the states (position, velocity etc) for multiple joint on a body. (supports planar and spherical joints)
     '''
     ...
def getKeyboardEvents(*args):
     '''
     Get keyboard events, keycode and state (KEY_IS_DOWN, KEY_WAS_TRIGGERED, KEY_WAS_RELEASED)
     '''
     ...
def getLinkState(*args):
     '''
     position_linkcom_world, world_rotation_linkcom,
     position_linkcom_frame, frame_rotation_linkcom,
     position_frame_world, world_rotation_frame,
     linearVelocity_linkcom_world, angularVelocity_linkcom_world
       = getLinkState(objectUniqueId, linkIndex, computeLinkVelocity=0,
                      computeForwardKinematics=0, physicsClientId=0)
     Provides extra information such as the Cartesian world coordinates center of mass (COM) of the link, relative to the world reference frame.
     '''
     ...
def getLinkStates(*args):
     '''
     same as getLinkState except it takes a list of linkIndices
     '''
     ...
def getMatrixFromQuaternion(*args):
     '''
     Compute the 3x3 matrix from a quaternion, as a list of 9 values (row-major)
     '''
     ...
def getMeshData(*args):
     '''
     Get mesh data. Returns vertices etc from the mesh.
     '''
     ...
def getMouseEvents(*args):
     '''
     Get mouse events, event type and button state (KEY_IS_DOWN, KEY_WAS_TRIGGERED, KEY_WAS_RELEASED)
     '''
     ...
def getNumBodies(*args):
     '''
     Get the number of bodies in the simulation.
     '''
     ...
def getNumConstraints(*args):
     '''
     Get the number of user-created constraints in the simulation.
     '''
     ...
def getNumJoints(*args):
     '''
     Get the number of joints for an object.
     '''
     ...
def getNumUserData(*args):
     '''
     getNumUserData(bodyUniqueId physicsClientId=0)
     Retrieves the number of user data entries in a body.
     '''
     ...
def getOverlappingObjects(*args):
     '''
     Return all the objects that have overlap with a given axis-aligned bounding box volume (AABB).Input are two vectors defining the AABB in world space [min_x,min_y,min_z],[max_x,max_y,max_z].
     '''
     ...
def getPhysicsEngineParameters(*args):
     '''
     Get the current values of internal physics engine parameters
     '''
     ...
def getQuaternionFromAxisAngle(*args):
     '''
     Compute the quaternion from axis and angle representation.
     '''
     ...
def getQuaternionFromEuler(*args):
     '''
     Convert Euler [roll, pitch, yaw] as in URDF/SDF convention, to quaternion [x,y,z,w]
     '''
     ...
def getQuaternionSlerp(*args):
     '''
     Compute the spherical interpolation given a start and end quaternion and an interpolation value in range [0..1]
     '''
     ...
def getUserData(*args):
     '''
     getUserData(userDataId, physicsClientId=0)
     Returns the user data value.
     '''
     ...
def getUserDataId(*args):
     '''
     getUserDataId(bodyUniqueId, key, linkIndex=-1, visualShapeIndex=-1, physicsClientId=0)
     Retrieves the userDataId given the key and optionally link and visual shape index.
     '''
     ...
def getUserDataInfo(*args):
     '''
     getUserDataInfo(bodyUniqueId, userDataIndex, physicsClientId=0)
     Retrieves the key and the identifier of a user data as (userDataId, key, bodyUniqueId, linkIndex, visualShapeIndex).
     '''
     ...
def getVREvents(*args):
     '''
     Get Virtual Reality events, for example to track VR controllers position/buttons
     '''
     ...
def getVisualShapeData(*args):
     '''
     Return the visual shape information for one object.
     '''
     ...
def invertTransform(*args):
     '''
     Invert a transform, provided as [position], [quaternion].
     '''
     ...
def isConnected(*args):
     '''
     isConnected(physicsClientId=0)
     Return if a given client id is connected.
     '''
     ...
def isNumpyEnabled(*args):
     '''
     return True if PyBullet was compiled with NUMPY support. This makes the getCameraImage API faster
     '''
     ...
def loadBullet(*args):
     '''
     Load a world from a .bullet file.
     '''
     ...
def loadMJCF(*args):
     '''
     Load multibodies from an MJCF file.
     '''
     ...
def loadPlugin(*args):
     '''
     Load a plugin, could implement custom commands etc.
     '''
     ...
def loadSDF(*args):
     '''
     Load multibodies from an SDF file.
     '''
     ...
def loadSoftBody(*args):
     '''
     Load a softbody from an obj file.
     '''
     ...
def loadTexture(*args):
     '''
     Load texture file.
     '''
     ...
def loadURDF(*args):
     '''
     bodyUniqueId = loadURDF(fileName, basePosition=[0.,0.,0.], baseOrientation=[0.,0.,0.,1.], useMaximalCoordinates=0, useFixedBase=0, flags=0, globalScaling=1.0, physicsClientId=0)
     Create a multibody by loading a URDF file.
     '''
     ...
def multiplyTransforms(*args):
     '''
     Multiply two transform, provided as [position], [quaternion].
     '''
     ...
def performCollisionDetection(*args):
     '''
     performCollisionDetection(physicsClientId=0)
     Update AABBs, compute overlapping pairs and contact points. stepSimulation also includes this already.
     '''
     ...
def rayTest(*args):
     '''
     Cast a ray and return the first object hit, if any. Takes two arguments (from_position [x,y,z] and to_position [x,y,z] in Cartesian world coordinates
     '''
     ...
def rayTestBatch(*args):
     '''
     Cast a batch of rays and return the result for each of the rays (first object hit, if any. or -1) Takes two required arguments (list of from_positions [x,y,z] and a list of to_positions [x,y,z] in Cartesian world coordinates) and one optional argument numThreads to specify the number of threads to use to compute the ray intersections for the batch. Specify 0 to let Bullet decide, 1 (default) for single core execution, 2 or more to select the number of threads to use.
     '''
     ...
def readUserDebugParameter(*args):
     '''
     Read the current value of a user debug parameter, given the user debug item unique id.
     '''
     ...
def removeAllUserDebugItems(*args):
     '''
     remove all user debug draw items
     '''
     ...
def removeAllUserParameters(*args):
     '''
     remove all user debug parameters (sliders, buttons)
     '''
     ...
def removeBody(*args):
     '''
     Remove a body by its body unique id.
     '''
     ...
def removeCollisionShape(*args):
     '''
     Remove a collision shape. Only useful when the collision shape is not used in a body (to perform a getClosestPoint query).
     '''
     ...
def removeConstraint(*args):
     '''
     Remove a constraint using its unique id.
     '''
     ...
def removeState(*args):
     '''
     Remove a state created using saveState by its state unique id.
     '''
     ...
def removeUserData(*args):
     '''
     removeUserData(userDataId, physicsClientId=0)
     Removes a user data entry.
     '''
     ...
def removeUserDebugItem(*args):
     '''
     remove a user debug draw item, giving its unique id
     '''
     ...
def renderImage(*args):
     '''
     obsolete, please use getCameraImage and getViewProjectionMatrices instead
     '''
     ...
def resetBasePositionAndOrientation(*args):
     '''
     Reset the world position and orientation of the base of the object instantaneously, not through physics simulation. (x,y,z) position vector and (x,y,z,w) quaternion orientation.
     '''
     ...
def resetBaseVelocity(*args):
     '''
     Reset the linear and/or angular velocity of the base of the object  in world space coordinates. linearVelocity (x,y,z) and angularVelocity (x,y,z).
     '''
     ...
def resetDebugVisualizerCamera(*args):
     '''
     For the 3D OpenGL Visualizer, set the camera distance, yaw, pitch and target position.
     '''
     ...
def resetJointState(*args):
     '''
     resetJointState(objectUniqueId, jointIndex, targetValue, targetVelocity=0, physicsClientId=0)
     Reset the state (position, velocity etc) for a joint on a body instantaneously, not through physics simulation.
     '''
     ...
def resetJointStateMultiDof(*args):
     '''
     resetJointStateMultiDof(objectUniqueId, jointIndex, targetValue, targetVelocity=0, physicsClientId=0)
     Reset the state (position, velocity etc) for a joint on a body instantaneously, not through physics simulation.
     '''
     ...
def resetJointStatesMultiDof(*args):
     '''
     resetJointStatesMultiDof(objectUniqueId, jointIndices, targetValues, targetVelocities=0, physicsClientId=0)
     Reset the states (position, velocity etc) for multiple joints on a body instantaneously, not through physics simulation.
     '''
     ...
def resetMeshData(*args):
     '''
     Reset mesh data. Only implemented for deformable bodies.
     '''
     ...
def resetSimulation(*args):
     '''
     resetSimulation(physicsClientId=0)
     Reset the simulation: remove all objects and start from an empty world.
     '''
     ...
def resetVisualShapeData(*args):
     '''
     Obsolete method, kept for backward compatibility, use changeVisualShapeData instead.
     '''
     ...
def restoreState(*args):
     '''
     Restore the full state of an existing world.
     '''
     ...
def rotateVector(*args):
     '''
     Rotate a vector using a quaternion.
     '''
     ...
def saveBullet(*args):
     '''
     Save the full state of the world to a .bullet file.
     '''
     ...
def saveState(*args):
     '''
     Save the full state of the world to memory.
     '''
     ...
def saveWorld(*args):
     '''
     Save a approximate Python file to reproduce the current state of the world: saveWorld(filename). (very preliminary and approximately)
     '''
     ...
def setAdditionalSearchPath(*args):
     '''
     Set an additional search path, used to load URDF/SDF files.
     '''
     ...
def setCollisionFilterGroupMask(*args):
     '''
     Set the collision filter group and the mask for a body.
     '''
     ...
def setCollisionFilterPair(*args):
     '''
     Enable or disable collision detection between two object links.Input are two object unique ids and two link indices and an enumto enable or disable collisions.
     '''
     ...
def setDebugObjectColor(*args):
     '''
     Override the wireframe debug drawing color for a particular object unique id / link index.If you ommit the color, the custom color will be removed.
     '''
     ...
def setDefaultContactERP(*args):
     '''
     setDefaultContactERP(defaultContactERP, physicsClientId=0)
     Set the amount of contact penetration Error Recovery Paramater (ERP) in each time step. 		This is an tuning parameter to control resting contact stability. This value depends on the time step.
     '''
     ...
def setGravity(*args):
     '''
     setGravity(gravX, gravY, gravZ, physicsClientId=0)
     Set the gravity acceleration (x,y,z).
     '''
     ...
def setInternalSimFlags(*args):
     '''
     This is for experimental purposes, use at own risk, magic may or not happen
     '''
     ...
def setJointMotorControl(*args):
     '''
     This (obsolete) method cannot select non-zero physicsClientId, use setJointMotorControl2 instead.Set a single joint motor control mode and desired target value. There is no immediate state change, stepSimulation will process the motors.
     '''
     ...
def setJointMotorControl2(*args):
     '''
     Set a single joint motor control mode and desired target value. There is no immediate state change, stepSimulation will process the motors.
     '''
     ...
def setJointMotorControlArray(*args):
     '''
     Set an array of motors control mode and desired target value. There is no immediate state change, stepSimulation will process the motors.This is similar to setJointMotorControl2, with jointIndices as a list, and optional targetPositions, targetVelocities, forces, kds and kps as listsUsing setJointMotorControlArray has the benefit of lower calling overhead.
     '''
     ...
def setJointMotorControlMultiDof(*args):
     '''
     Set a single joint motor control mode and desired target value. There is no immediate state change, stepSimulation will process the motors.This method sets multi-degree-of-freedom motor such as the spherical joint motor.
     '''
     ...
def setJointMotorControlMultiDofArray(*args):
     '''
     Set control mode and desired target values for multiple motors. There is no immediate state change, stepSimulation will process the motors.This method sets multi-degree-of-freedom motor such as the spherical joint motor.
     '''
     ...
def setPhysicsEngineParameter(*args):
     '''
     Set some internal physics engine parameter, such as cfm or erp etc.
     '''
     ...
def setRealTimeSimulation(*args):
     '''
     setRealTimeSimulation(enableRealTimeSimulation, physicsClientId=0)
     Enable or disable real time simulation (using the real time clock, RTC) in the physics server. Expects one integer argument, 0 or 1
     '''
     ...
def setTimeOut(*args):
     '''
     Set the timeOut in seconds, used for most of the API calls.
     '''
     ...
def setTimeStep(*args):
     '''
     setTimeStep(timestep, physicsClientId=0)
     Set the amount of time to proceed at each call to stepSimulation. (unit is seconds, typically range is 0.01 or 0.001)
     '''
     ...
def setVRCameraState(*args):
     '''
     Set properties of the VR Camera such as its root transform for teleporting or to track objects (camera inside a vehicle for example).
     '''
     ...
def startStateLogging(*args):
     '''
     Start logging of state, such as robot base position, orientation, joint positions etc. Specify loggingType (STATE_LOGGING_MINITAUR, STATE_LOGGING_GENERIC_ROBOT, STATE_LOGGING_VR_CONTROLLERS, STATE_LOGGING_CONTACT_POINTS, etc), fileName, optional objectUniqueId, maxLogDof, bodyUniqueIdA, bodyUniqueIdB, linkIndexA, linkIndexB. Function returns int loggingUniqueId
     '''
     ...
def stepSimulation(*args):
     '''
     stepSimulation(physicsClientId=0)
     Step the simulation using forward dynamics.
     '''
     ...
def stopStateLogging(*args):
     '''
     Stop logging of robot state, given a loggingUniqueId.
     '''
     ...
def submitProfileTiming(*args):
     '''
     Add a custom profile timing that will be visible in performance profile recordings on the physics server.On the physics server (in GUI and VR mode) you can press 'p' to start and/or stop profile recordings
     '''
     ...
def syncBodyInfo(*args):
     '''
     syncBodyInfo(physicsClientId=0)
     Update body and constraint/joint information, in case other clients made changes.
     '''
     ...
def syncUserData(*args):
     '''
     syncUserData(bodyUniqueIds=[], physicsClientId=0)
     Update user data, in case other clients made changes.
     '''
     ...
def unloadPlugin(*args):
     '''
     Unload a plugin, given the pluginUniqueId.
     '''
     ...
def unsupportedChangeScaling(*args):
     '''
     Change the scaling of the base of an object.Warning: unsupported rudimentary feature that has many limitations.
     '''
     ...
def vhacd(*args):
     '''
     Compute volume hierarchical convex decomposition of an OBJ file.
     '''
     ...
