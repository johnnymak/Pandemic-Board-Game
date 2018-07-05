
#include "Observer.h"
#include "Subject.h"

// Constructor
Subject::Subject() {
	obv = new list<Observer*>;
}

// Destructor
Subject::~Subject() {
	delete obv;
}

// Attaching an Observer
void Subject::Attach(Observer* obv) {
	this->obv->push_back(obv);
}

// Detaching an Observer
void Subject::Detach(Observer* obv) {
	this->obv->remove(obv);
}


// Notifying an Observer of a change with message
void Subject::Notify(string message) {
	list<Observer*>::iterator i = this->obv->begin();
	for (; i != this->obv->end(); ++i)
		(*i)->Update(message);
}