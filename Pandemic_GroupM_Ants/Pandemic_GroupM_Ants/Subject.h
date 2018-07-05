#pragma once

#include "Observer.h"
#include <list>

using namespace std;

class Observer;

class Subject {
	public:

		//Default Constructor
		Subject();

		// Destructor
		virtual ~Subject();

		virtual void Attach(Observer* obv);		// Attaching an Observer
		virtual void Detach(Observer* obv);		// Detaching an Observer
		virtual void Notify(string message);	// Notify the Observers when there is a change with a message

	private:
		list<Observer*>* obv;					// List of Observers
};


