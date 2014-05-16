#ifndef SIXENSE_UTILS_INTERFACES_HPP
#define SIXENSE_UTILS_INTERFACES_HPP

#include <sixense.h>
#include <sixense_math.hpp>

namespace sixenseUtils {

	class IButtonStates {
	public:

		typedef enum {
			DIR_UP,
			DIR_DOWN,
			DIR_LEFT,
			DIR_RIGHT,
			DIR_CW,
			DIR_CCW,
			DIR_FORWARD,
			DIR_BACKWARD
		} Direction;

		typedef enum {
			ACTION_BUTTON_PRESS,
			ACTION_JOYSTICK_MOVE,
			ACTION_TRIGGER_PRESS,
			ACTION_TILT_GESTURE,
			ACTION_POINT_GESTURE, 
			ACTION_VELOCITY_GESTURE
		} ActionType;

		virtual void update( sixenseControllerData *cd )=0;
		virtual bool buttonJustPressed( unsigned short which_button )=0;
		virtual bool buttonJustReleased( unsigned short which_button )=0;

		virtual void setTriggerThreshold( float thresh )=0;
		virtual bool triggerJustPressed()=0;
		virtual bool triggerJustReleased()=0;

		virtual void setStickThreshold( float thresh )=0;
		virtual bool stickJustPressed( Direction which )=0;
		virtual bool stickJustReleased( Direction which )=0;

		virtual void setAbsoluteTiltAngleThresholdInDeg( float thresh )=0;
		virtual void setRelativeTiltAngleThresholdInDeg( float thresh )=0;

		// Relative tilts are an orientation change relative to the orientation last time setRelativeOrigin() was called
		virtual void setRelativeOrigin()=0;
		virtual void startPointGesture()=0;
		virtual void stopPointGesture()=0;
		virtual bool relativeTiltJustStarted( Direction which )=0;
		virtual bool relativeTiltJustStopped( Direction which )=0;

		// Absolute gestures are just relative to the world
		virtual bool absoluteTiltJustStarted( Direction which )=0;
		virtual bool absoluteTiltJustStopped( Direction which )=0;

		virtual bool justStarted( ActionType action, int arg )=0;
		virtual bool justStopped( ActionType action, int arg )=0;

	};

	class IDerivatives {
	public:
		virtual void update( sixenseControllerData *cd )=0;

		virtual sixenseMath::Vector3 getPosition()=0;
		virtual sixenseMath::Vector3 getVelocity()=0;
		virtual sixenseMath::Vector3 getAcceleration()=0;

		virtual sixenseMath::Vector3 getRSquared()=0;

		virtual void setOffset( sixenseMath::Vector3 offset )=0;
		virtual sixenseMath::Vector3 getOffset()=0;
	};

	class IFPSViewAngles {

	public:
		virtual void setGame( const char* game_name )=0;

		typedef enum {
			MOUSELOOK,
			FREE_AIM_TWO_CONTROLLER,
			FREE_AIM_ONE_CONTROLLER,
			DUAL_ANALOG
		} fps_mode;

		virtual void setMode( fps_mode mode )=0;
		virtual fps_mode getMode()=0;

		virtual int update( sixenseControllerData *left_cd, sixenseControllerData *right_cd, float frametime_in_ms=0.0f )=0;

		virtual sixenseMath::Vector3 getViewAngles()=0; // The final view angles, ie the feet plus the aiming from the controller
		virtual sixenseMath::Vector3 getViewAngleOffset()=0; // The negative controller aim, used to keep the camera stationary in metroid mode

		virtual sixenseMath::Vector3 getSpinSpeed()=0;

		virtual void forceViewAngles( fps_mode mode, sixenseMath::Vector3 )=0; // Used to initialize the view direction when switching into stick spin mode

		virtual void setFeetAnglesMetroid( sixenseMath::Vector3 angles )=0;
		virtual sixenseMath::Vector3 getFeetAnglesMetroid()=0;

		virtual float getTestVal()=0;

		// Parameters to control the different modes
		typedef enum {
			CONTROLLER_ANGLE_MODE,

			AIM_1TO1_HEADING_MULTIPLIER,
			AIM_1TO1_PITCH_MULTIPLIER,
			AIM_1TO1_RATCHET_VERTICAL,

			AIM_METROID_HEADING_MULTIPLIER,
			AIM_METROID_PITCH_MULTIPLIER,
			AIM_METROID_DEAD_ZONE_RADIUS,
			AIM_METROID_ACCEL_BAND_SIZE,
			AIM_METROID_MAX_SPEED,
			AIM_METROID_AUTO_LEVEL_RATE,
			AIM_METROID_ACCEL_BAND_EXPONENT,
			AIM_METROID_SWITCH_BLEND_TIME_ENTER,
			AIM_METROID_SWITCH_BLEND_TIME_EXIT,

			FEET_ANGLES_OFFSET_STICK_SPIN_HORIZ_MULTIPLIER,
			FEET_ANGLES_OFFSET_STICK_SPIN_VERT_MULTIPLIER,
			FEET_ANGLES_OFFSET_STICK_SPIN_INVERT_PITCH,
			FEET_ANGLES_OFFSET_STICK_SPIN_EXPONENT,

			PITCH_CHANGE_BLEND_VAL,

			SPRING_VIEW_ENABLED,
			SPRING_VIEW_MIN_SPRING,
			SPRING_VIEW_MAX_SPRING,
			SPRING_VIEW_MIN_ANGLE,
			SPRING_VIEW_MAX_ANGLE,

			FEET_ANGLES_ALLOW_VERT_STICK_SPIN,

			AIM_METROID_ACCEL_BAND_POWER,

			HOLDING_TURN_SPEED,

			ROLL_CORRECTION_BLEND,
			EXIT_METROID_BLEND,

			LEFT_HANDED,

			LAST_FPS_VIEW_ANGLES_PARAM
		} fps_params;

		virtual void setParameter( fps_params param, float val )=0;
		virtual float getParameter( fps_params param )=0;

		virtual void setFov( float hfov, float vfov )=0;
		virtual void getFov( float *hfov, float *vfov )=0;

		virtual void setHoldingTurnSpeed( float horiz, float vert )=0;

		virtual void setRatcheting( bool )=0;
		virtual bool isRatcheting()=0;

		virtual void reset()=0;

		virtual void forceMetroidBlend( float blend_fraction )=0;
	};

	class IFPSEvents {

	public:
		typedef enum {
			WALK_LEFT,
			WALK_RIGHT,
			WALK_FORWARD,
			WALK_BACK,
			JUMP,
			USE,
			PRIMARY_FIRE,
			SECONDARY_FIRE,
			ZOOM,
			MELEE,
			LEAN_LEFT,
			LEAN_RIGHT,
			CROUCH,
			SPRINT,
			THROW_GRENADE,
			FLASHLIGHT,
			NIGHTVISION,
			RELOAD,
			NEXT_WEAPON,
			PREV_WEAPON,
			ESC_KEY,
			EQUIP_GRENADE,
			MEDPACK_SWITCH,
			GIVE,
			NEXT_PRIMARY_WEAPON,
			ONE_TO_ONE_CARRY,
			EQUIP_MELEE,
			EQUIP_MACHINEGUN,
			EQUIP_PISTOL,
			RATCHET,
			LAST_FPS_EVENT
		} fps_event;

		typedef enum {
			CONTROLLER_LEFT,
			CONTROLLER_RIGHT
		} fps_controller;

		typedef enum {
			ACTION_BUTTON_PRESS,
			ACTION_JOYSTICK_MOVE,
			ACTION_TRIGGER_PRESS,
			ACTION_TILT_GESTURE,
			ACTION_POINT_GESTURE, 
			ACTION_VELOCITY_GESTURE
		} fps_action;

		typedef enum {
			DIR_UP,
			DIR_DOWN,
			DIR_LEFT,
			DIR_RIGHT,
			DIR_FORWARD,
			DIR_BACKWARD,
			DIR_CW,
			DIR_CCW
		} fps_direction;

		virtual void setGame( const char* game_name )=0;

		virtual void setBinding( fps_event, fps_controller, fps_action, int param )=0;

		virtual void setPointGestureButton( int )=0;

		virtual bool isPointGestureActive()=0;

		virtual int update( sixenseControllerData *left_cd, sixenseControllerData *right_cd, float frametime_in_ms=0.0f )=0;

		virtual bool eventActive( fps_event event_query )=0;
		virtual bool eventStarted( fps_event event_query )=0;
		virtual bool eventStopped( fps_event event_query )=0;

		virtual sixenseMath::Vector3 getGrenadeThrowVelocity()=0;

		// Parameters to control the different modes
		typedef enum {
			MELEE_SENSITIVITY,
			WEAPON_SELECT_SENSITIVITY,
			CROUCH_SENSITIVITY,
			JUMP_SENSITIVITY,
			RELOAD_SENSITIVITY,
			THROW_SENSITIVITY,

			CONTROLLER_ANGLE_MODE,

			AUTO_ONE_TO_ONE_START_VEL,
			AUTO_ONE_TO_ONE_START_ACCEL,
			AUTO_ONE_TO_ONE_START_ACCEL_TIMER,
			AUTO_ONE_TO_ONE_START_ANGLE_THRESH,

			AUTO_ONE_TO_ONE_STOP_XY_DIST,
			AUTO_ONE_TO_ONE_STOP_Z_DIST,

			LEFT_HANDED,

			LAST_FPS_EVENTS_PARAM
		} fps_params;

		virtual void setParameter( fps_params param, float val )=0;
		virtual float getParameter( fps_params param )=0;


	};

	class IFPSPlayerMovement {
	public:
		virtual void setGame( const char* game_name )=0;

		virtual int update( sixenseControllerData *left_cd, sixenseControllerData *right_cd, float frametime_in_ms=0.0f )=0;

		virtual sixenseMath::Vector2 getWalkDir()=0;

		// Parameters to control the different modes
		typedef enum {
			DEAD_ZONE_PERCENT,
			EXPONENTIAL,
			USE_RIGHT_HAND,

			LEFT_HANDED,

			LAST_FPS_MOVEMENT_PARAM
		} fps_movement_params;

		virtual void setParameter( fps_movement_params param, float val )=0;
		virtual float getParameter( fps_movement_params param )=0;

	};

	class ILaserPointer {
	public:
		virtual void setScreenSize( sixenseMath::Vector2 width_and_height_in_mm )=0;
		virtual void setScreenCenterOffsetFromBase( sixenseMath::Vector3 offset_in_mm )=0;

		virtual sixenseMath::Vector2 getScreenSize()=0;
		virtual sixenseMath::Vector3 getScreenCenterOffsetFromBase()=0;

		virtual sixenseMath::Vector2 getIntersection( sixenseMath::Vector3 position, sixenseMath::Matrix3 rotation )=0;
	};

	class IMousePointer {
	public:
		virtual sixenseMath::Vector2 update( sixenseControllerData *cd )=0;

		virtual void setSensitivity( float sensitivity )=0;
		virtual void setAcceleration( float acceleration )=0;
		virtual void setSlideEnabled( bool slide_enabled )=0;
		virtual void setAspectRatio( float aspect_ratio )=0;

		virtual float getRollAngle()=0;

		virtual void setCenter()=0;
	};

	class IControllerManager {
	public:

		// Define the steps the manager goes through for the different modes
		typedef enum {

			SETUP_COMPLETE,

			// P1C1 = One player, 1 controller

			// One player one controller
			P1C1_START, //1
			P1C1_POWER_UP_0,
			P1C1_POWER_UP_DONE,
			P1C1_AIM_P1L,
			P1C1_DONE,
			P1C1_OUT_OF_RANGE,
			P1C1_IDLE, // 7

			// One player two controllers
			P1C2_START, // 8 
			P1C2_POWER_UP_0,
			P1C2_POWER_UP_1,
			P1C2_POWER_UP_DONE,
			P1C2_AIM_P1L, // 12
			P1C2_AIM_P1R,
			P1C2_DONE,
			P1C2_OUT_OF_RANGE,
			P1C2_IDLE // 16

		} setup_step;


		typedef enum {
			NO_SOUND,
			SUCCESS_BEEP,
			FAIL_BEEP
		} sound_type;

		typedef void (*setup_callback)( setup_step );

		typedef enum {
			ONE_PLAYER_ONE_CONTROLLER,
			ONE_PLAYER_TWO_CONTROLLER,
			TWO_PLAYER_ONE_CONTROLLER,
			TWO_PLAYER_TWO_CONTROLLER,
			THREE_PLAYER_ONE_CONTROLLER,
			THREE_PLAYER_TWO_CONTROLLER,
			FOUR_PLAYER_ONE_CONTROLLER,
			FOUR_PLAYER_TWO_CONTROLLER
		} game_type;

		typedef enum {
			P1L,
			P1R,
			P2L,
			P2R,
			P3L,
			P3R,
			P4L,
			P4R, 
			LAST_CONTROLLER_DESC
		} controller_desc;

		virtual void setGameType( game_type gt )=0;
		virtual game_type getGameType()=0;

		// Update the controller_manager. Should be called each frame.
		virtual void update( sixenseAllControllerData * )=0;

		// Get the controller index for the given description, ie player 1's left hand is controller index 3
		virtual int getIndex( controller_desc )=0;

		// Force the user to rebind the controllers to the player slots.
		virtual void rebind()=0;

		// Register a callback that will get called when the mode changes
		virtual void registerSetupCallback( setup_callback )=0;

		// Returns true if the application should show the menu system
		virtual bool isMenuVisible()=0;

		// Returns the filename of the recommended texture for this step
		virtual const char* getTextureFileName()=0;

		// Returns a string describing this step
		virtual const char* getStepString()=0;


		virtual IControllerManager::sound_type shouldPlaySound()=0;

		virtual setup_step getCurrentStep()=0;

	};

	class IMouseAndKeyboard {
	public:
		virtual void sendMouseClick( int click, int release ) = 0; // 0 = none, 1 = left, 2 = right
		virtual void sendMouseClick( int click, int release, int delay) = 0; // 0 = none, 1 = left, 2 = right
		virtual void sendMouseWheelMove( int dir ) = 0; // in 'clicks', + = forwards - = backwards
		virtual void sendKeyState( char key, int press, int release ) = 0;
		virtual void sendKeyState( char key, int press, int release, int delay ) = 0;
		virtual void releaseKey( char key ) = 0;
		virtual void releaseMouseButton( int release ) = 0;
		virtual void sendAbsoluteMouseMove( float x, float y ) = 0;
		virtual void sendRelativeMouseMove( float x, float y ) = 0;
		virtual void sendAbsoluteMouseMoveInPixels( int x, int y ) = 0;

		virtual void getMousePos( int *x, int *y ) = 0;
		virtual void getPrimaryScreenResolution( int *x, int *y ) = 0;

		virtual void sendMouseClick( float x, float y, int click, int release ) = 0;

		// This should be called once per 10ms or so...
		virtual void processQueue() = 0;
	};
}

#endif
