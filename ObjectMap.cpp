
#include <fstream>
#include <math.h>

#include <vector>

#include <ObjectMap.h>

std::map< int, std::vector<std::pair <int, int>>> ObjectMap::ObjectsDetected;
std::map< int, int>  ObjectMap::AvgObjectsDetected;

bool ObjectMap::SegmentIsScanned;
int ObjectMap::CurrentSegment;

ObjectMap::ObjectMap()
{
	
		
}

void ObjectMap::SetCurrentReading(int RadarVal, int RadarPos, float heading)
{
	if (heading > 180)
		HeadingConverted = heading - 360;
	else
		HeadingConverted = heading;

	CurrentRadarReading.first = HeadingConverted;

	CurrentRadarReading.second.first = RadarVal;
	CurrentRadarReading.second.second = RadarPos;
}

void ObjectMap::UpdateSegment()
{

	int CurrentPos = CurrentRadarReading.second.second;

	int AngleSegment = 0; //Between 0-180 in multiples of 20;

	if (CurrentPos < 10) //20
		AngleSegment = 0; //20
	else
	{
		int Rem = CurrentPos % 10; //20

		AngleSegment = ((CurrentPos - Rem) / 10); //20

		AngleSegment *= 10;  //20



	}

	/*
	if (CurrentPos < 20)
	{
		std::pair <int, int> Reading;
		Reading.first = CurrentRadarReading.second.first; //Value
		Reading.second = CurrentRadarReading.second.second; //position

		Segment.push_back(Reading);
	}

	if (CurrentPos > AngleSegment && CurrentPos < AngleSegment + 20)
	{
		std::pair <int, int> Reading;
		Reading.first = CurrentRadarReading.second.first; //Value
		Reading.second = CurrentRadarReading.second.second; //position

		Segment.push_back(Reading);
	}
	*/
	
	std::pair <int, int> Reading;
	Reading.first = CurrentRadarReading.second.first; //Value
	Reading.second = CurrentRadarReading.second.second; //position
	Segment.push_back(Reading);

	if (Segment.size() > 9)  //19
	{
		/*
		if (ObjectsDetected.find(AngleSegment) == ObjectsDetected.end())
			ObjectsDetected.insert({ AngleSegment, Segment });
		else
			ObjectsDetected.insert_or_assign(AngleSegment, Segment);
         */

		int AvgCalc = CalcAverage(Segment);

		

		if (AvgObjectsDetected.find(AngleSegment) == AvgObjectsDetected.end())
		{
			AvgObjectsDetected.insert({ AngleSegment, AvgCalc });
			//std::cout << "\n Map insert first";
		}
		else
		{
			AvgObjectsDetected.insert_or_assign(AngleSegment, AvgCalc);
			//std::cout << "\n Map insert";
		}

		//std::cout << "\n Segment scanned " << AngleSegment << " " << AvgCalc<<" "<< AvgObjectsDetected.size();
		SegmentIsScanned = 1;

		Segment.clear();
	}

}

///Private functions

int ObjectMap::CalcAverage(std::vector<std::pair <int, int>> Seg)
{
	int Total = 0,
		Avg = 0;

	for (int i = 0; i < Seg.size(); i++)
	{
		Total += Seg[i].first;
	}

	Avg = (Total / Seg.size());

	return Avg;
}
