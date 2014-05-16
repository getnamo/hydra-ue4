/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#include "sixense_utils/export.hpp"

namespace sixenseUtils {

	class SIXENSE_UTILS_EXPORT Time {
	public:
		static double getTimeInMilliseconds();

		// Return the number of milliseconds since this was last called with the given token
		static double getElapsedMilliseconds( int token );
	};

}

