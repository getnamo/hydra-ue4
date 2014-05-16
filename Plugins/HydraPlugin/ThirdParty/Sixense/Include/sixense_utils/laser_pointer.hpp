/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef SIXENSE_PORT_UTILS_LASER_POINTER_HPP
#define SIXENSE_PORT_UTILS_LASER_POINTER_HPP

#pragma warning(push)
#pragma warning( disable:4251 )

#include "sixense_utils/export.hpp"
#include "sixense_utils/interfaces.hpp"

#include "sixense_math.hpp"

using sixenseMath::Vector2;
using sixenseMath::Vector3;
using sixenseMath::Matrix3;

namespace sixenseUtils {

	// LaserPointer computes a ray that shoots from the controller and intersects with the screen. 
	class SIXENSE_UTILS_EXPORT LaserPointer {

	public:
		LaserPointer();

		void setScreenSize( Vector2 width_and_height_in_mm );
		void setScreenCenterOffsetFromBase( Vector3 offset_in_mm );

		Vector2 getScreenSize();
		Vector3 getScreenCenterOffsetFromBase();

		Vector2 getIntersection( Vector3 position, Matrix3 rotation );

	private:
		Vector2 _width_and_height_in_mm;
		Vector3 _offset_in_mm;

	};


	// This class can be used to compute the required parameters for the LaserPointer by having the user aim the controller at the
	// corners of the screen.
	class SIXENSE_UTILS_EXPORT LaserPointerCalib {
	public:

		LaserPointerCalib() { _has_computed = false; }

		// Calibrate using 4 ray casts, bottom left and top right from 2 distinct positions in space
		bool compute( 
			Vector3 bottom_left_ray_from_point_A, 
			Vector3 top_right_ray_from_point_A, 
			Vector3 bottom_left_ray_from_point_B, 
			Vector3 top_right_ray_from_point_B );

		// Calibrate using 2 ray casts plus the screen size, bottom left and top right
		bool compute(
			float screen_width_in_mm, float screen_aspect, 
			Vector3 bottom_left_ray_from_point_A, 
			Vector3 top_right_ray_from_point_A );

		bool hasComputed() { return _has_computed; }
		Vector2 getScrenSize();
		Vector3 setScreenCenterOffsetFromBase();

	private:
		bool _has_computed;
		Vector2 _width_and_height_in_mm;
		Vector3 _offset_in_mm;
	};
};

#pragma warning(pop)

#endif
