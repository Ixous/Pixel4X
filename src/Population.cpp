#include "Population.hpp"

#include <vector>
#include <array>
#include <utility> //std::pair
#include <map>

#include "Religion.hpp"
#include "Culture.hpp"



PopulationGroup::PopulationGroup() 
{
}
//#############################################################################################################################################################
PopulationGroup::~PopulationGroup() 
{
}
//#############################################################################################################################################################
Population::Population() 
{
}
//#############################################################################################################################################################
Population::~Population() 
{
}



unsigned int Population::getSize() {
	unsigned int size = 0;
	for (PopulationGroup t_popgroup : populationsGroups) {
		size += t_popgroup.size;
	}
	return size;
}
//#############################################################################################################################################################
std::vector<std::pair<Culture, char>> Population::getCultureDistribution() {
	std::map<Culture, int> t_cultureMap;
	// std::vector<std::pair<Culture, char>> cultureDistribution;
	// for (PopulationGroup t_popgroup : populationsGroups) {
	// 	size += t_popgroup.size;
	// }
	std::vector<std::pair<Culture, char>> cultureDistribution;
	return cultureDistribution;
}
//#############################################################################################################################################################
// std::vector<std::pair<Religion, char>> Population::getReligionDistribution() {
// 
// }
//#############################################################################################################################################################
std::array<unsigned char, 15> Population::getClassDistribution() {
	unsigned int t_classSizes[15] = {};
	unsigned int t_sum = 0;
	for (PopulationGroup t_popgroup : populationsGroups) { //TODO : change to a reference ??
		t_classSizes[t_popgroup.classs] += t_popgroup.size;
		t_sum += t_popgroup.size;
	}
	std::array<unsigned char, 15> classDistribution;
	for (int iii = 0; iii < 15; ++iii) {
		classDistribution[iii] = (255.0f*t_classSizes[iii])/(t_sum*1.0f);
	}
	return classDistribution;
}
//#############################################################################################################################################################
std::array<unsigned char, 8> Population::getPoliticalOpinions() {
	unsigned int t_politicalGroupsSizes[8] = {};
	unsigned int t_sum = 0;
	for (PopulationGroup t_popgroup : populationsGroups) {
		for (int iii = 0; iii < 8; ++iii) {
			t_politicalGroupsSizes[iii] += t_popgroup.size*(t_popgroup.political_opinions[iii]/255.0);
		}
		t_sum += t_popgroup.size;
	}
	std::array<unsigned char, 8> politicalOpinionDistribution;
	for (int iii = 0; iii < 8; ++iii) {
		politicalOpinionDistribution[iii] = (255.0f*t_politicalGroupsSizes[iii])/(t_sum*1.0f);
	}
	return politicalOpinionDistribution;
}
//#############################################################################################################################################################
std::array<unsigned char, 8> Population::getNeeds() {
	unsigned int t_needGroupsSizes[8] = {};
	unsigned int t_sum = 0;
	for (PopulationGroup t_popgroup : populationsGroups) {
		for (int iii = 0; iii < 8; ++iii) {
			t_needGroupsSizes[iii] += t_popgroup.size*(t_popgroup.political_opinions[iii]/255.0);
		}
		t_sum += t_popgroup.size;
	}
	std::array<unsigned char, 8> needsDistribution;
	for (int iii = 0; iii < 8; ++iii) {
		needsDistribution[iii] = (255.0f*t_needGroupsSizes[iii])/(t_sum*1.0f);
	}
	return needsDistribution;
}
//#############################################################################################################################################################
// unsigned char Population::getAvgUnrest() {
// 
// }
//#############################################################################################################################################################
// unsigned char Population::getAvgLitteracy() {
// 
// }
//#############################################################################################################################################################
// unsigned char Population::getAvgEducation() {
// 
// }
//#############################################################################################################################################################
// unsigned char Population::getAvgConsciousness() {
// 
// }
//#############################################################################################################################################################
// unsigned char Population::getAvgUnemployment() {
// 
// }