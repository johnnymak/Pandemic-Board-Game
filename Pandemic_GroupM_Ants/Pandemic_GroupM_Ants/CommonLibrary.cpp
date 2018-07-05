#include "CommonLibrary.h"

//ENUMS////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
string infectTypeEnumToString(InfectType t) {
	switch (t) {
	case 0:
		return "Yellow";
	case 1:
		return "Red";
	case 2:
		return "Blue";
	case 3:
		return "Black";
	default:
		return "INVALID ENUM";
	}
}

string cureStatusEnumToString(CureStatus c)
{
	switch (c) {
	case notCured:
		return "Not Cured ";
	case cured:
		return "Cured     ";
	case eradicated:
		return "Eradicated";
	default:
		return "INVALID ENUM";
	}
}
