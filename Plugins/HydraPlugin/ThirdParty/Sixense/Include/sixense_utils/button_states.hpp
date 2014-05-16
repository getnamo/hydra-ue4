/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef SIXENSE_UTILS_BUTTON_STATES_HPP
#define SIXENSE_UTILS_BUTTON_STATES_HPP

#pragma warning(push)
#pragma warning( disable:4251 )

#include "sixense_utils/export.hpp"
#include "sixense_utils/interfaces.hpp"

#include <sixense.h>
#include <sixense_math.hpp>

namespace sixenseUtils {

	// LaserPointer computes a ray that shoots from the controller and intersects with the screen. 
	class SIXENSE_UTILS_EXPORT ButtonStates : public IButtonStates {

	public:

		ButtonStates();
		void update( sixenseControllerData *cd );
		bool buttonJustPressed( unsigned short which_button );
		bool buttonJustReleased( unsigned short which_button );

		void setTriggerThreshold( float thresh );
		bool triggerJustPressed();
		bool triggerJustReleased();

		void setStickThreshold( float thresh );
		bool stickJustPressed( Direction which );
		bool stickJustReleased( Direction which );

		void setAbsoluteTiltAngleThresholdInDeg( float thresh );
		void setRelativeTiltAngleThresholdInDeg( float thresh );

		// Relative tilts are an orientation change relative to the orientation last time setRelativeOrigin() was called
		void setRelativeOrigin();
		void startPointGesture();
		void stopPointGesture();
		bool relativeTiltJustStarted( Direction which );
		bool relativeTiltJustStopped( Direction which );

		// Absolute gestures are just relative to the world
		bool absoluteTiltJustStarted( Direction which );
		bool absoluteTiltJustStopped( Direction which );

		float _trigger_threshold;

		bool justStarted( ActionType action, int arg );
		bool justStopped( ActionType action, int arg );

	protected:
		void updateJoystick( sixenseControllerData *cd );
		void updateTrigger( sixenseControllerData *cd );
		void updateButtons( sixenseControllerData *cd );
		void updateTilt( sixenseControllerData *cd );

	private:

		// buttons
		unsigned short _just_pressed;
		unsigned short _just_released;
		unsigned short _last_buttons;

		// trigger
		bool _trigger_just_pressed;
		bool _trigger_just_released;
		float _last_trigger_state;

		// joystick
		float _joystick_threshold;
		float _last_joystick[2];
		bool _joystick_just_pressed[4], _joystick_just_released[4]; // 4 stick directions


		// tilt
		float _absolute_tilt_angle_threshold_in_deg, _relative_tilt_angle_threshold_in_deg;
		sixenseMath::Vector3 _last_point_vec;

		bool _point_gesture_active;

		bool _last_relative_tilt_states[6];
		bool _relative_tilt_just_started[6], _relative_tilt_just_stopped[6]; // 6 tilt directions

		bool _last_absolute_tilt_states[6];
		bool _absolute_tilt_just_started[6], _absolute_tilt_just_stopped[6]; // 6 tilt directions
		bool _should_zero_relative_origin;
		sixenseMath::Matrix3 _relative_origin;

	};

}

#pragma warning(pop)

#endif

