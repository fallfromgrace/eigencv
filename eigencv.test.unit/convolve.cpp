#include "stdafx.h"
#include "CppUnitTest.h"

#include "convolve.hpp"
#include <iostream>

#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace eigencv
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			auto m = Eigen::Matrix<double, 5, 5>::Zero().eval();
			m.coeffRef(3, 3) = 1;

			Eigen::Matrix3d k;
			k << 0, 1, 0,
				0, 0, 0,
				0, 0, 0;

			std::cout << m;

			auto result = convolve(m, k);

			std::cout << result;
		}

	};
}