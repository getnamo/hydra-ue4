/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef EVENT_TRIGGERS_HPP
#define EVENT_TRIGGERS_HPP

#pragma warning(push)
#pragma warning( disable:4251 )

#include <sixense_math.hpp>

namespace sixenseUtils {

		// Classes for triggering events based on joystick or trigger positions

	// These are classes that call the supplied EventTriggerBase's 'trigger()' events when triggered. They can be 
	// created and bound to a switch.

	class EventTriggerBase {
	public:
		virtual void trigger() const = 0;
	};

	// Do nothing
	class NullEventTrigger : public EventTriggerBase {
	public:
		virtual void trigger() const {};
	};



	// These classes monitor a floating point value, and trigger their events when the value crosses
	// a threshold, both rising and falling.

	class EventSwitchBase { 
	public:
		EventSwitchBase( float thresh, const EventTriggerBase *positive_transition_event, const EventTriggerBase *negative_transition_event ) : _last_state(false), _thresh(thresh), _positive_transition_event(positive_transition_event), _negative_transition_event(negative_transition_event) {}
		~EventSwitchBase() {
			delete _positive_transition_event;
			delete _negative_transition_event;
		}
		void test( float val ) {
			if( val > _thresh  &&  !_last_state ) {
				if( _positive_transition_event ) {
					_positive_transition_event->trigger();
				}
				_last_state = true;
			}
			if( val < _thresh  &&  _last_state ) {
				if( _negative_transition_event ) {
					_negative_transition_event->trigger();
				}
				_last_state = false;
			}
		}

		bool getLastState( void ) {
			return _last_state;
		}

	protected:
		bool _last_state;
		float _thresh;
		const EventTriggerBase *_positive_transition_event;
		const EventTriggerBase *_negative_transition_event;

	private:
		EventSwitchBase() {}
	};

	class PlaneCrossEventSwitch : public EventSwitchBase {
	public:
		PlaneCrossEventSwitch( sixenseMath::Plane plane, float thresh, const EventTriggerBase *positive_transition_event, const EventTriggerBase *negative_transition_event ) : EventSwitchBase( thresh, positive_transition_event, negative_transition_event ),
			_plane(plane) {}

		void test( sixenseMath::Vector3 pt ) {
			EventSwitchBase::test( -1.0f * (float)_plane.whichSide( pt ) );
		}
	private:
		sixenseMath::Plane _plane;
	};

	class BinaryEventSwitch : public EventSwitchBase {
	public:
		BinaryEventSwitch( const EventTriggerBase *positive_transition_event, const EventTriggerBase *negative_transition_event ) : EventSwitchBase( 0.5f, positive_transition_event, negative_transition_event ) {}

		void test( bool val ) {
			EventSwitchBase::test( val ? 1.0f : 0.0f );
		}
	};

	class ValuatorEventSwitch : public EventSwitchBase {
	public:
		ValuatorEventSwitch( float thresh, const EventTriggerBase *positive_transition_event, const EventTriggerBase *negative_transition_event ) : EventSwitchBase( thresh, positive_transition_event, negative_transition_event ){}
		void test( float val ) {
			EventSwitchBase::test( val );
		}
	};


}
#pragma warning(pop)

#endif
