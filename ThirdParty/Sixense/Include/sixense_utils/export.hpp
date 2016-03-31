/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef SIXENSE_UTILS_EXPORT_HPP
#define SIXENSE_UTILS_EXPORT_HPP

#if defined(WIN32)
  #ifdef SIXENSE_UTILS_STATIC_LIB
    #define SIXENSE_UTILS_EXPORT 
  #else
		#ifdef BUILDING_SIXENSE_UTILS
			#define SIXENSE_UTILS_EXPORT __declspec(dllexport)
		#else
			#define SIXENSE_UTILS_EXPORT __declspec(dllimport)
		#endif
	#endif
#else
  #define SIXENSE_UTILS_EXPORT 
#endif

#endif
