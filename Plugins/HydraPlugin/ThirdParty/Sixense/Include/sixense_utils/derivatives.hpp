/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef SIXENSE_UTILS_DERIVATIVES_HPP
#define SIXENSE_UTILS_DERIVATIVES_HPP

#pragma warning(push)
#pragma warning( disable:4251 )

#include "sixense.h"
#include "sixense_math.hpp"

#include <deque>

#include "sixense_utils/export.hpp"
#include "sixense_utils/interfaces.hpp"

namespace sixenseUtils {

	class SIXENSE_UTILS_EXPORT Derivatives : public IDerivatives {

	public:
		Derivatives( sixenseMath::Vector3 offset_vec = sixenseMath::Vector3() );

		void update( sixenseControllerData *cd );

		sixenseMath::Vector3 getPosition();
		sixenseMath::Vector3 getVelocity();
		sixenseMath::Vector3 getAcceleration();

		sixenseMath::Vector3 getRSquared();

		void setOffset( sixenseMath::Vector3 offset );
		sixenseMath::Vector3 getOffset();

	protected:
		std::deque<unsigned char> _last_n_sequence;

		std::deque<float> _last_n_times;

		std::deque<sixenseMath::Vector3> _last_n_pos;

		sixenseMath::Vector3 _pos;
		sixenseMath::Vector3 _vel;
		sixenseMath::Vector3 _accel;

		sixenseMath::Vector3 _r_squared;

		sixenseMath::Vector3 _offset_vec;

	};

}

#pragma warning(pop)

#endif
