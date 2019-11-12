#include "energy.h"

using namespace cv;
using namespace std;

Energy::Energy(uint8_t&color):ally_color(color),
			src_blue(SRC_HEIGHT,SRC_WIDTH,CV_8UC1),
			src_red(SRC_HEIGHT, SRC_WIDTH,CV_8UC1){
	initEnergy();
	initEnergyPartParam();
	
	is_big = false;
	is_small = false;

}

Energy::~Energy() = default;

void Energy::initEnergy()
{


	fans.clear();
	armors.clear();
	flow_strip_fans.clear();
	target_armors.clear();
	flow_strips.clear();
}

void Energy::initEnergyPartParam()
{
	energy_part_param_.RED_GRAY_THRESH = 180;
	energy_part_param_.BLUE_GRAY_THRESH = 100;

}

//---------------------------------------
//此函数为能量机关主控制流函数
//---------------------------------------
void Energy::run(cv::Mat &src)
{
	clearAll();
	initImage(src);

}

void Energy::clearAll()
{
	fans.clear();
	armors.clear();
	flow_strip_fans.clear();
	target_armors.clear();
	flow_strips.clear();

}

void Energy::initImage(cv::Mat &src)
{
	if(src.type() == CV_8UC3)
	{
		cvtColor(src, src, COLOR_BGR2GRAY);
	}

	threshold(src, src, energy_part_param_.RED_GRAY_THRESH, 255, THRESH_BINARY);
	if(show_process) imshow("bin", src);
	if(show_energy || show_process) waitKey(1);


}
