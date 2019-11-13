#include <iostream>
#include "energy.h"
using namespace std;
using namespace cv;
int main()
{
	uint8_t s=1;

	Energy energy(s);

	Mat src = imread("/home/luo/lena.jpg");

	energy.run(src);

	cout << "Success" << endl;

	return 0;
}
