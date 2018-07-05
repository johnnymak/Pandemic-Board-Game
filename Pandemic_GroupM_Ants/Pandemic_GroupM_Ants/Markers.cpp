#include "Markers.h"


//MARKERS CLASS/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Constructors///////////////////////////////////////////
Markers::Markers() : researchStations(NUM_OF_RESEARCH_STATIONS), outbreakCounter(0), infectionRatePosition(0)
{
	std::fill_n(this->diseaseCubes, NUM_OF_DISEASES, NUM_OF_DISEASE_CUBES_PER_TYPE);
	std::fill_n(this->cureMarkers, NUM_OF_DISEASES, notCured);
}

Markers::~Markers() {}


//Get and Set/////////////////////////////////////////////
bool Markers::useResearchStation()
{
	if (this->researchStations > 0) {
		this->researchStations--;
		Notify("Research Station Used");
		return true;
	}
	return false;
}

bool Markers::useDiseaseCube(InfectType color)
{
	if (this->diseaseCubes[color] > 0) {
		this->diseaseCubes[color]--;
		Notify("Disease Cube Used");
		return true;
	}
	return false;
}

// OBSERVER PATTERN: Add Disease Cube
void Markers::addDiseaseCube(InfectType color) {
	diseaseCubes[color]++;
	Notify("Disease Cube moved back into cube bank! ");
}

// OBSERVER PATTERN: Increase Outbreak Count
void Markers::increaseOutbreakCounter() {
	this->outbreakCounter++;
	Notify("Outbreak has been increased! ");
}

// OBSERVER PATTERN: Increas Infection Rate
void Markers::increaseInfectRate() {
	this->infectionRatePosition++;
	Notify("Infection Rate has been increased! ");
}