// eigencv.test.console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "eigencv\convolve.hpp"

#include <iostream>
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\eigen.hpp"
#include "chrono\stopwatch.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	auto r = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>::Random(1600, 1200).eval();
	Eigen::Matrix<double, 5, 5> k;
	k << 
		0.04, 0.04, 0.04, 0.04, 0.04,
		0.04, 0.04, 0.04, 0.04, 0.04,
		0.04, 0.04, 0.04, 0.04, 0.04,
		0.04, 0.04, 0.04, 0.04, 0.04,
		0.04, 0.04, 0.04, 0.04, 0.04;
	chrono::stopwatch sw;

	cv::Mat m;
	cv::Mat mm;
	cv::eigen2cv(r, m);
	sw.start();
	for (int i = 0; i < 10; i++)
	{
		cv::boxFilter(m, mm, -1, cv::Size(5, 5));
		m = mm;
	}
	sw.stop();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(sw.elapsed()).count() << std::endl;
	std::cout << mm.at<double>(100, 100) << std::endl << std::endl;

	sw.restart();
	for (int i = 0; i < 10; i++)
		r = eigencv::convolve<eigencv::border_none>(r, k);
	sw.stop();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(sw.elapsed()).count() << std::endl;
	std::cout << r.coeff(100, 100) << std::endl << std::endl;

	return 0;
}

