#pragma once

#include "CommonLibrary.h"
#include "Subject.h"

//MARKERS CLASS////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
class Markers : public Subject
{
	//Attributes///////////////////////////////////////
	int researchStations;
	int diseaseCubes[NUM_OF_DISEASES];
	int outbreakCounter;
	int infectionRatePosition;
	CureStatus cureMarkers[NUM_OF_DISEASES];

	//Serialize//////////////////
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & researchStations & diseaseCubes & outbreakCounter & cureMarkers & infectionRatePosition;
	}

public:
	//Constructors/////////////////////////////////////
	Markers();
	~Markers();

	//Get and Set///////////////////////////////////////
	int getNumOfResearchStations() { return this->researchStations; }
	bool useResearchStation();

	int getNumOfDiseaseCubes(InfectType color) { return this->diseaseCubes[color]; }
	bool useDiseaseCube(InfectType color);
	
	void addDiseaseCube(InfectType color);
	void increaseOutbreakCounter();
	int getOutbreakCounter() { return this->outbreakCounter; }
	void increaseInfectRate();

	int getInfectionRate() { return INFECTION_RATE[infectionRatePosition]; }

	CureStatus getCureMarker(InfectType infection) { return this->cureMarkers[infection]; }
	void cureDiseaseUpdateMarker(InfectType infection) { this->cureMarkers[infection] = cured; }
	void eradicateDiseaseUpdateMarker(InfectType infection) { this->cureMarkers[infection] = eradicated; }

};

