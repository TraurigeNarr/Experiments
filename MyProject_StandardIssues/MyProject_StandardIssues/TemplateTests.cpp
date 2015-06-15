#include "stdafx.h"

#include "TemplatesTest.h"

#include <iostream>

namespace Templates
	{

	void TestFunction()
		{
		Test<float>::RetFunc(1, 2);		
		char test[4] { 0xa, 0, 0, 0 };
		int res = ReadType<int>(test, 0);
		std::cout << "result = " << res << std::endl;
		}

	}