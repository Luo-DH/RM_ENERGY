#include <iostream>
#include "energy.h"
using namespace std;
using namespace cv;
int main()
{
	Energy energy=new Energy();

	Mat src = imread("/home/luo/lena.jpg");

	energy.run();

	cout << "Success" << endl;

	return 0;
}
