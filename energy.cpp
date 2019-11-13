#include "energy.h"

using namespace cv;
using namespace std;

Energy::Energy(uint8_t &color):ally_color(color),
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

	energy_part_param_.ARMOR_CONTOUR_AREA_MAX = 500;
	energy_part_param_.ARMOR_CONTOUR_AREA_MIN = 180;
	energy_part_param_.ARMOR_CONTOUR_LENGTH_MAX = 50;
	energy_part_param_.ARMOR_CONTOUR_LENGTH_MIN = 10;
	energy_part_param_.ARMOR_CONTOUR_WIDTH_MAX = 30;
	energy_part_param_.ARMOR_CONTOUR_WIDTH_MIN = 0;
	energy_part_param_.ARMOR_CONTOUR_HW_RATIO_MAX = 3;
	energy_part_param_.ARMOR_CONTOUR_HW_RATIO_MIN = 1;
}

//---------------------------------------
//此函数为能量机关主控制流函数
//---------------------------------------
void Energy::run(cv::Mat &src)
{
	cv::Mat src_clone = src.clone();
	clearAll();
	src = initImage(src);

	if (show_process) imshow("bin", src);
	if (findArmors(src)) return;
	cout << findArmors(src);
	if (show_energy) showArmors("made", src_clone);

}

void Energy::showArmors(std::string windows_name, const cv::Mat &src)
{
	if(src.empty()) return;
	static Mat image2show;

	if(src.type() == CV_8UC1)
	{
		cvtColor(src, image2show, COLOR_GRAY2RGB);
	}
	else if (src.type()==CV_8UC3)
	{
		image2show = src.clone();
	}
	for(const auto &armor: armors)
	{
		Point2f vertices[4];
		armor.points(vertices);
		for (int i = 0;i<4;i++)
		{
			line(image2show, vertices[i], vertices[(i+1)%4], Scalar(0,0,255),2);
		}
	}
	imshow(windows_name, image2show);
	waitKey(1);
}

void Energy::clearAll()
{
	fans.clear();
	armors.clear();
	flow_strip_fans.clear();
	target_armors.clear();
	flow_strips.clear();

}

Mat Energy::initImage(cv::Mat &src)
{
	/*
	if(src.type() == CV_8UC3)
	{
		cvtColor(src, src, COLOR_BGR2GRAY);
	}
	*/
	vector<cv::Mat> imgChannels;
	split(src, imgChannels);

#ifdef RED
Mat midImage = imgChannels.at(2) - imgChannels.at(0);
#endif
#ifndef RED
Mat midImage = imgChannels.at(0) - imgChannels.at(2);
#endif

//	threshold(src, src, energy_part_param_.BLUE_GRAY_THRESH, 255, THRESH_BINARY);
	threshold(midImage, midImage, 100, 255, CV_THRESH_BINARY);
	if(show_process) imshow("bin", midImage);
	if(show_energy || show_process) waitKey(1);

	return midImage;
}

int Energy::findArmors(const cv::Mat &src)
/*{
	if(src.empty())
	{
		std::cout << "empty!" << std::endl;
		return 0;	
	}

	static Mat src_bin;
	src_bin = src.clone();

	std::vector<vector<Point> > armor_contours;
	std::vector<vector<Point> > armor_contours_external;

	ArmorStruct(src_bin); //图形膨胀
	findContours(src_bin, armor_contours,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
	if(show_process) imshow("armor struct", src_bin);

	findContours(src_bin, armor_contours_external, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	for(int i=0; i<armor_contours_external.size();i++)
	{
		unsigned long external_contour_size = armor_contours_external[i].size();
		for(int j=0;j<armor_contours.size();j++)
		{
			unsigned long all_size = armor_contours[j].size();
			if(external_contour_size == all_size)
			{
				swap(armor_contours[j], armor_contours[armor_contours.size()-1]);
				armor_contours.pop_back();
				break;
			}

		}
	
	}

	for(auto &armor_contour : armor_contours)
	{
		if(1)
		//if(!isValidArmorContour(armor_contour))
		{
			continue;
		}
		armors.emplace_back(cv::minAreaRect(armor_contour));
	}
	cout << static_cast<int>(armors.size()) << endl;
	cout << "??" << endl;
	return static_cast<int>(armors.size());
}
*/
{
    if (src.empty())                // 如果没有图像
    {
        cout << "empty!" << endl;
        return 0;
    }
    static Mat src_bin;     
    src_bin = src.clone();

    std::vector<vector<Point> > armor_contours; // 装甲板轮廓
    std::vector<vector<Point> > armor_contours_external;//用总轮廓减去外轮廓，只保留内轮廓，除去流动条的影响。

    ArmorStruct(src_bin);//图像膨胀，防止图像断开并更方便寻找
    findContours(src_bin, armor_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    if (show_process)imshow("armor struct", src_bin);

    findContours(src_bin, armor_contours_external, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    for (int i = 0; i < armor_contours_external.size(); i++)//去除外轮廓
    {
        unsigned long external_contour_size = armor_contours_external[i].size();
        for (int j = 0; j < armor_contours.size(); j++) 
        {
            unsigned long all_size = armor_contours[j].size();
            if (external_contour_size == all_size) 
            {
                swap(armor_contours[j], armor_contours[armor_contours.size() - 1]);
                armor_contours.pop_back();
                break;
            }
        }
    }

    for (auto &armor_contour : armor_contours) 
    {
        if (!isValidArmorContour(armor_contour)) // 此函数用于判断找到的矩形候选区是否为装甲板
        {
            continue;
        }
        armors.emplace_back(cv::minAreaRect(armor_contour));
    }

//    if (show_info) 
//    {
//        if (armors.size() < 1)cout << "no armors!" << endl;
//    }

    return static_cast<int>(armors.size());
}



void Energy::ArmorStruct(cv::Mat &src)
{
	Mat element_dilate_1 = getStructuringElement(MORPH_RECT, Size(5,5));
	Mat element_erode_1 = getStructuringElement(MORPH_RECT, Size(2,2));
	Mat element_dilate_2 = getStructuringElement(MORPH_RECT, Size(3,3));
	Mat element_erode_2 = getStructuringElement(MORPH_RECT, Size(2,2));
	Mat element_dilate_3 = getStructuringElement(MORPH_RECT, Size(3,3));
	Mat element_erode_3 = getStructuringElement(MORPH_RECT, Size(1,1));

	dilate(src, src, element_dilate_1);
	erode(src, src, element_erode_1);
}


bool Energy::isValidArmorContour(const vector<cv::Point>&armor_contour)
{
	double cur_contour_area = contourArea(armor_contour);
	if(cur_contour_area>energy_part_param_.ARMOR_CONTOUR_AREA_MAX ||
	   cur_contour_area<energy_part_param_.ARMOR_CONTOUR_AREA_MIN)
	{
		return false;
	}
	RotatedRect cur_rect = minAreaRect(armor_contour);
	Size2f cur_size = cur_rect.size;
	float length = cur_size.height > cur_size.width?cur_size.height:cur_size.width;
	float width = cur_size.height<cur_size.width?cur_size.height:cur_size.width;
	if(length<energy_part_param_.ARMOR_CONTOUR_LENGTH_MIN ||
	   width <energy_part_param_.ARMOR_CONTOUR_WIDTH_MIN ||
	   length>energy_part_param_.ARMOR_CONTOUR_LENGTH_MAX ||
	   width >energy_part_param_.ARMOR_CONTOUR_WIDTH_MAX)
	{
		return false;
	}

	float length_width_ratio = length/width;
	if(length_width_ratio>energy_part_param_.ARMOR_CONTOUR_HW_RATIO_MAX||
	   length_width_ratio<energy_part_param_.ARMOR_CONTOUR_HW_RATIO_MIN)
	{
		return false;
	}
/*
	if(cur_contour_area/cur_size.area()<energy_part_param_.ARMOR_CONTOUR_AREA_RATIO_MIN)
	{
		return false;
	}
*/
	return true;
}
