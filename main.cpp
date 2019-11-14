#include <iostream>
#include "energy.h"
using namespace std;
using namespace cv;

#define BLUE

int main()
{
	uint8_t s=1;

	Energy energy(s);

//	Mat src = imread("/home/luo/lena.jpg");

//	energy.run(src);

//	cout << "Success" << endl;

	VideoCapture capture;
	capture.open("/home/luo/fan.mp4");
//	capture.open(0);

	Mat src;

	while(capture.isOpened())
	{
		capture >> src;
		energy.run(src);
	//	waitKey(1);
	}

	return 0;
}
