#pragma once

#include "Subject.h"

class Subject;

class Observer {
	public:

		// Destructor
		virtual ~Observer();

		// Update Function
		// virtual void Update() = 0;
		virtual void Update(string message) = 0;

	protected:

		// Constructor
		Observer();
};