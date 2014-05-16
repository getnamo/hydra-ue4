/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef CONTROLLER_MANAGER_HPP
#define CONTROLLER_MANAGER_HPP

#pragma warning(push)
#pragma warning( disable:4251 )

#include "sixense_utils/export.hpp"
#include "sixense_utils/interfaces.hpp"
#include "sixense_utils/sixense_utils_string.hpp"

#include <string>
#include <vector>
#include <map>

#include <sixense.h>

namespace sixenseUtils {

	class SIXENSE_UTILS_EXPORT ControllerManager : public IControllerManager {

	public:
		~ControllerManager();

		void setGameType( game_type gt );
		game_type getGameType();

		// Update the ControllerManager. Should be called each frame.
		void update( sixenseAllControllerData * );

		// Get the controller index for the given description, ie player 1's left hand is controller index 3
		int getIndex( controller_desc );

		// Force the user to rebind the controllers to the player slots.
		void rebind();

		// Register a callback that will get called when the mode changes
		void registerSetupCallback( setup_callback );

		// Returns true if the application should show the menu system
		bool isMenuVisible();

		// Returns the filename of the recommended texture for this step
		const char* getTextureFileName();

		// Returns a string describing this step
		const char* getStepString();


		ControllerManager::sound_type shouldPlaySound();

		static ControllerManager *getTheControllerManager();

		static controller_desc controllerDescFromString( sixense_utils_string str );

		setup_step getCurrentStep();

	protected:
		ControllerManager();

		game_type _game_type;

		// A pointer to the current step
		setup_step _current_step;

		// A list of maps, one map of steps for each of the game_types.
		std::vector< std::map< setup_step, class base_step*> >_steps;

		int _controller_map[LAST_CONTROLLER_DESC];

		setup_callback _callback;

	};

	// Singleton
	SIXENSE_UTILS_EXPORT ControllerManager *getTheControllerManager();

}

#pragma warning(pop)

#endif

