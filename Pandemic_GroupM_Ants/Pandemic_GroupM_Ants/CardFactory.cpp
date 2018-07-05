#include "CardFactory.h"

Card * CardFactory::CreateCard(ifstream& file, string type)
{
	if (type == "City")
	{
		int color;
		string city;
		file >> color >> ws;
		getline(file, city);
		Card *cityCard = new CityCard(city, (InfectType)color);
		return cityCard;
	}
	else if (type == "Event")
	{
		string name;
		string description;
		getline(file, name, ';');
		// Remove whitespace at start/end of name
		name.erase(name.begin(), std::find_if(name.begin(), name.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
		// Get description
		getline(file, description);
		// Remove whitespace at start/end of description
		description.erase(description.begin(), std::find_if(description.begin(), description.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
		Card *eventCard = new EventCard(name, description);
		return eventCard;
	}
	else if (type == "Infection")
	{
		int color;
		string city;
		file >> color >> ws;
		getline(file, city);

		Card *infectionCard = new InfectionCard(city, (InfectType)color);
		return infectionCard;
	}
	else if (type == "Role")
	{
		string title;
		string color;
		string description;

		getline(file, title);
		getline(file, color);
		getline(file, description);

		// Remove whitespace at start/end of title
		title.erase(title.begin(), std::find_if(title.begin(), title.end(), std::bind1st(std::not_equal_to<char>(), ' ')));

		Card *roleCard = new RoleCard(title, color, description);

		return roleCard;
	}

	else return new Card();
}
