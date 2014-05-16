/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef SIXENSE_UTILS_MOUSE_POINTER_HPP
#define SIXENSE_UTILS_MOUSE_POINTER_HPP

#pragma warning(push)
#pragma warning( disable:4251 )

#include "sixense_utils/export.hpp"
#include "sixense_utils/interfaces.hpp"

using sixenseMath::Vector2;
using sixenseMath::Vector3;
using sixenseMath::Matrix3;

namespace sixenseUtils {

	class SIXENSE_UTILS_EXPORT MousePointer : public IMousePointer {

	public:
		MousePointer();
		sixenseMath::Vector2 update( sixenseControllerData *cd );

		void setSensitivity( float sensitivity );
		void setAcceleration( float acceleration );
		void setSlideEnabled( bool slide_enabled );
		void setAspectRatio( float aspect_ratio );
		void setLockRelativeToWindow(bool lock);
		void setLockRelativeToClientBounds(bool client);

		Vector2 getMouseOffset() { return mouse_offset; }
		void setMouseOffset( Vector2 offset ) { mouse_offset = offset; }
		float getRollAngle();

		void setCenter();
		void setWindowCenter();

	private:
		bool _slide_enabled;
		float _aspect_ratio;
		float _sensitivity;
		float _screen_width_in_mm;

		// velocity params
		float _min_vel, _max_vel;
		float _acceleration;
	
		// This offset is the position of the center of the virtual screen relative to the base
		Vector2 mouse_offset;

		// Keep track of the previous mouse pos so we can compute velocity
		Vector2 _last_mouse_pos;

		// Keep track of the last accel so we can filter it
		float _last_accel;

		float _roll_angle;

		bool _center_mouse_requested;
		bool _center_mouse_on_window;
		
		bool _lock_relative_to_window;
		bool _lock_relative_using_client_bounds;
	};
}

#pragma warning(pop)

#endif
