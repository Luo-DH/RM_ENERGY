#include <iostream>
#include "energy.h"
using namespace std;
using namespace cv;
int main()
{
	Energy energy();

	Mat src = imread("/home/luo/lena.jpg");

	energy.run(src);

	cout << "Success" << endl;

	return 0;
}
