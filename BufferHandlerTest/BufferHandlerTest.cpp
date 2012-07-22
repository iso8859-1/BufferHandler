// BufferHandlerTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define BOOST_TEST_MODULE BufferHandlerTest
#include <boost/test/unit_test.hpp>

#include <boost/smart_ptr.hpp>
#include "BufferHandler.h"

BOOST_AUTO_TEST_CASE(accessLowerBoundaryTest8UILE)
{
	unsigned char buffer[10] = {0,1,2,3,4,5,6,7,8,9};
	{
		auto h = CreateBufferHandler(0,8,UnsignedIntegerLittleEndian);
		auto value = h->ReadL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 0);
	}
}

BOOST_AUTO_TEST_CASE(accessLowerBoundaryTest8SILE)
{
	unsigned char buffer[10] = {0,1,2,3,4,5,6,7,8,9};
	{
		auto h = CreateBufferHandler(0,8,SignedIntegerLittleEndian);
		auto value = h->ReadL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 0);
	}
}

BOOST_AUTO_TEST_CASE(accessLowerBoundaryTest16UILE)
{
	unsigned char buffer[10] = {0,1,2,3,4,5,6,7,8,9};
	{
		auto h = CreateBufferHandler(0,16,UnsignedIntegerLittleEndian);
		auto value = h->ReadL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 0x0100);
	}
}

BOOST_AUTO_TEST_CASE(accessLowerBoundaryTest16SILE)
{
	unsigned char buffer[10] = {0,1,2,3,4,5,6,7,8,9};
	{
		auto h = CreateBufferHandler(0,16,SignedIntegerLittleEndian);
		auto value = h->ReadL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 0x0100);
	}
}

BOOST_AUTO_TEST_CASE(accessLowerBoundaryTest32UILE)
{
	unsigned char buffer[10] = {0,1,2,3,4,5,6,7,8,9};
	{
		auto h = CreateBufferHandler(0,32,UnsignedIntegerLittleEndian);
		auto value = h->ReadL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 0x03020100);
	}
}

BOOST_AUTO_TEST_CASE(accessLowerBoundaryTest32SILE)
{
	unsigned char buffer[10] = {0,1,2,3,4,5,6,7,8,9};
	{
		auto h = CreateBufferHandler(0,32,SignedIntegerLittleEndian);
		auto value = h->ReadL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 0x03020100);
	}
}

BOOST_AUTO_TEST_CASE(accessLowerBoundaryTest64UILE)
{
	unsigned char buffer[10] = {0,1,2,3,4,5,6,7,8,9};
	{
		auto h = CreateBufferHandler(0,64,UnsignedIntegerLittleEndian);
		auto value = h->ReadLL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 0x0706050403020100);
	}
}

BOOST_AUTO_TEST_CASE(accessLowerBoundaryTest64SILE)
{
	unsigned char buffer[10] = {0,1,2,3,4,5,6,7,8,9};
	{
		auto h = CreateBufferHandler(0,64,SignedIntegerLittleEndian);
		auto value = h->ReadLL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 0x0706050403020100);
	}
}

BOOST_AUTO_TEST_CASE(alignedAccessTestUILE8Bit)
{
	unsigned char buffer[10] = { 0,1,2,3,4,5,6,7,8,9};
	auto h = CreateBufferHandler(8,8,UnsignedIntegerLittleEndian);

	{
		auto value = h->ReadLL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 1);
	}
	{
		auto value = h->ReadL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 1);
	}
	{
		auto value = h->ReadULL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 1);
	}
	{
		auto value = h->ReadUL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 1);
	}
	{
		auto value = h->ReadF(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 1);
	}
	{
		auto value = h->ReadD(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == 1);
	}
}

BOOST_AUTO_TEST_CASE(alignedAccessTestSILE8Bit)
{
	unsigned char buffer[10] = { 0,0xff,2,3,4,5,6,7,8,9};
	auto h = CreateBufferHandler(8,8,SignedIntegerLittleEndian);
	auto h2 = CreateBufferHandler(16,8,SignedIntegerLittleEndian);

	{
		auto value = h->ReadLL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == -1);
	}
	{
		auto value = h->ReadL(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == -1);
	}
	{
		auto value = h2->ReadULL(&buffer[0],sizeof(buffer)); //use h2 since behavior at h1 is undefined.
		BOOST_CHECK(value == 2);
	}
	{
		auto value = h2->ReadUL(&buffer[0],sizeof(buffer)); //use h2 since behavior at h1 is undefined.
		BOOST_CHECK(value == 2);
	}
	{
		auto value = h->ReadF(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == -1);
	}
	{
		auto value = h->ReadD(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == -1);
	}
}

