#ifndef OBJECT_MAP_H
#define OBJECT_MAP_H

#include <iostream>
#include <windows.h>


#include <fstream>
#include <math.h>
#include <map>
#include <vector>
#include <GraphicRender.h>
#include <utility>
/*
This class will handle the translations 
and update the map

Each Radarposition value is one degree,
its corresponding 

The Map is circle composed of 18 segments 
(20 degree resolution). Each segment will have
20 readings of distance, the average of those
will be in the taken as the segment's distance

*/

#define HEADING_MAP_SIZE 360

class ObjectMap {
public:
	ObjectMap();

	void SetCurrentReading(int RadarVal, int RadarPos, float heading);
    void UpdateSegment();
	std::map< int, int> GetUpdatedMap() { return AvgObjectsDetected; };
	bool GetSegmentScannedState() { return SegmentIsScanned; };
	void SetSegmentScanState(bool state) { this->SegmentIsScanned = state; };

private:
    float HeadingConverted;
	std::pair <float, std::pair <int, int>> CurrentRadarReading; //first Heading, second radar value, pos

	std::pair <int, std::vector<std::pair <int, int>>> SegmentScanned;

	static std::map< int, std::vector<std::pair <int, int>>> ObjectsDetected; //first element: segment, second
                                                                              //is a vector of pairs Radarpos first, 
	                                                                          //radarval second.
	
	static bool SegmentIsScanned;
	static int CurrentSegment;

	static std::map< int, int> AvgObjectsDetected;

	std::vector<std::pair <int, int>> AveragePerSegment;
	std::vector<std::pair <int, int>> Segment;

	//Functions:
	int CalcAverage(std::vector<std::pair <int, int>> Seg);
};

#endif