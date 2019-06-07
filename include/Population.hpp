#pragma once

#include <vector>
#include <array>
#include <utility> //std::pair

#include "Culture.hpp"
#include "Religion.hpp"

enum WorkClass {
    ARISTOCRATE, BUSINESSMAN, CELEBRITY, //upper class
	INGENEER, ARTIST, MERCHANT, BUREAUCRAT, ARTISAN, RELIGIOUS_PERSONNEL, //middle class
	LAND_WORKER, FACTORY_WORKER, LOWSKILLED_WORKER, //lower class
	SLAVE, IMMIGRANT, HOMELESS //outer class, can't be taxed or controlled
};

enum PoliticalIdeology {
	LIBERAL = 0,
	ANARCHIST,
	CONSERVATIVE,
	REACTIONNARY,
	SOCIALIST,
	COMMUNIST,
	NATIONALIST,
	FASCIST
};

enum Needs {
	FOOD = 0,
	DRINK,
	MEDICINAL,
	FURNITURE,
	CLOTHING,
    LUXE, //TODO : change the name
	ENTERTAINMENT,
	COMMUNICATION, //TODO : remove ?
	TRANSPORTATION //TODO : remove ?
};

class PopulationGroup {
public:
    PopulationGroup();
    ~PopulationGroup();

    unsigned short size; //times 10, maximum of 655k people
	Culture 			culture;
	Religion 			religion;
	WorkClass 			classs;
	unsigned char 		age_distribution[3]; // 0-20, 20-40, 40-60 and the complement of the sum is 60+
	unsigned char 		political_opinions[8]; // complement of the sum is no-opinion
	unsigned char 		needs[8];
	int 				savings; //might not be enough in case of strong inflation and upper class ? negative for dept
	unsigned int 		salary;
	unsigned char 		unrest;
	unsigned char 		litteracy;
	unsigned char 		education;
	unsigned char 		consciousness;
	unsigned char 		unemployment; //replace with int of employed and add a method to calculate unumployment ?
};

class Population
{
public:
	Population();
	~Population();
	
	unsigned int getSize();
	std::array<unsigned char, 15> getClassDistribution();
	std::vector<std::pair<Culture, char>> getCultureDistribution();  	//TODO
	std::vector<std::pair<Religion, char>> getReligionDistribution();  	//TODO
	std::array<unsigned char, 8> getPoliticalOpinions();
	std::array<unsigned char, 8> getNeeds();
	unsigned char getAvgUnrest(); 										//TODO
	unsigned char getAvgLitteracy(); 									//TODO
	unsigned char getAvgEducation(); 									//TODO
	unsigned char getAvgConsciousness(); 								//TODO
	unsigned char getAvgUnemployment(); 								//TODO
private:
	std::vector<PopulationGroup> populationsGroups;
};