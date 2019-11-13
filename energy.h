#ifndef ENERGY_H
#define ENERGY_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "constant.h"
#include "param_struct_define.h"
#include "options.h"

class Energy
{
	public:
		Energy(uint8_t&color);
		~Energy();
		// 主函数确定大符模式还是小符模式
		bool is_big;
		bool is_small;

		void run(cv::Mat &src);

	private:
		EnergyPartParam energy_part_param_;//能力机关参数设置

		uint8_t&ally_color; // 我方颜色
		
		std::vector<cv::RotatedRect> fans; // 图像中所有的扇叶
		std::vector<cv::RotatedRect> armors; // 图像中可能的装甲版
		std::vector<cv::RotatedRect> flow_strip_fans;//可能的流动扇叶
		std::vector<cv::RotatedRect> target_armors;//可能的目标装甲版
		std::vector<cv::RotatedRect> flow_strips;//可能的流动条

		cv::Mat src_blue,src_red,src_green; // 通道分离中的三个通道

		void initEnergy();
		void initEnergyPartParam();// 能量机关参数初始化
		
		int findArmors(const cv::Mat &src); // 寻找途中所有装甲版

		void clearAll(); // 用于晴空各vector
		void initImage(cv::Mat &src);// 图像预处理
};


#endif
