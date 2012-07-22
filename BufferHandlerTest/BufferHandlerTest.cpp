// BufferHandlerTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define BOOST_TEST_MODULE BufferHandlerTest
#include <boost/test/unit_test.hpp>

#include "BufferHandler.h"

BOOST_AUTO_TEST_CASE(alignedAccessTest)
{
	unsigned char buffer[10] = { 0,1,2,3,4,5,6,7,8,9};

	DataHandler h = CreateBufferHandler(0,8,UnsignedIntegerLittleEndian);
}

