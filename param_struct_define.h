#ifndef PARAM_STRUCT_DEFINE_H
#define PARAM_STRUCT_DEFINE_H

#include <opencv2/opencv.hpp>
#include <vector>

using std::vector;

struct EnergyPartParam{

	int RED_GRAY_THRESH;
	int BLUE_GRAY_THRESH;

	long ARMOR_CONTOUR_AREA_MAX; //装甲版面积的最大值
	long ARMOR_CONTOUR_AREA_MIN; //装甲版面积的最小值
};

#endif
