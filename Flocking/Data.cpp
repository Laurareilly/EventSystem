#include "Data.h"
#include <fstream>
#include <string>

Data* gpData = NULL;

Data::Data()
{

}

void Data::loadData()
{
	std::ifstream flockingData;

	flockingData.open("WeightsAndRadii.txt");

	if (flockingData.good())
	{
		std::string trash;

		flockingData >> trash;
		flockingData >> trash;
		flockingData >> mSeparationRadius;
		flockingData >> trash;
		flockingData >> mCohesionRadius;
		flockingData >> trash;
		flockingData >> mAlignRadius;

		flockingData >> trash;
		flockingData >> trash;
		flockingData >> mSeparationWeightData;
		flockingData >> trash;
		flockingData >> mCohesionWeightData;
		flockingData >> trash;
		flockingData >> mAlighWeightData;
	}

	flockingData.close();
}

void Data::saveData()
{
	std::ofstream flockingData;

	flockingData.open("WeightsAndRadii.txt");

	if (flockingData.good())
	{
		flockingData << "Radii\n";
		flockingData << "Separation\n";
		flockingData << mSeparationRadius << "\n";
		flockingData << "Cohesion\n";
		flockingData << mCohesionRadius << "\n";
		flockingData << "Align\n";
		flockingData << mAlignRadius << "\n";

		flockingData << "Weight\n";
		flockingData << "Separation\n";
		flockingData << mSeparationWeightData << "\n";
		flockingData << "Cohesion\n";
		flockingData << mCohesionWeightData << "\n";
		flockingData << "Align\n";
		flockingData << mAlighWeightData << "\n";
	}

	flockingData.close();
}

void Data::setCohesionWeight(float cohWeight)
{
	mCohesionWeightData += cohWeight;
}

void Data::setSeparationWeight(float sepWeight)
{
	mSeparationWeightData += sepWeight;
}

void Data::setAlignWeight(float alignWeight)
{
	mAlighWeightData += alignWeight;
}

void Data::setCohesionRadius(float cohRadius)
{
	mCohesionRadius += cohRadius;
}

void Data::setSeparationRadius(float sepRadius)
{
	mSeparationRadius += sepRadius;
}

void Data::setAlignRadius(float alignRadius)
{
	mAlignRadius += alignRadius;
}
