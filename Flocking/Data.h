#pragma once

#include "Trackable.h"

class Data : public Trackable
{
public:
	Data();
	~Data() {};

	void loadData();
	void saveData();

	void setCohesionWeight(float cohWeight);
	void setSeparationWeight(float sepWeight);
	void setAlignWeight(float alignWeight);

	float getSeparationWeight() { return mSeparationWeightData; }
	float getCohesionWeight() { return mCohesionWeightData; }
	float getAlighnWeight() { return mAlighWeightData; }

	void setCohesionRadius(float cohRadius);
	void setSeparationRadius(float sepRadius);
	void setAlignRadius(float alignRadius);

	float getCohesionRadius() { return mCohesionRadius; }
	float getSeparationRadius() { return mSeparationRadius; }
	float getAlignRadius() { return mAlignRadius; }

private:
	float mCohesionWeightData;
	float mSeparationWeightData;
	float mAlighWeightData;

	float mCohesionRadius;
	float mSeparationRadius;
	float mAlignRadius;
};

extern Data* gpData;