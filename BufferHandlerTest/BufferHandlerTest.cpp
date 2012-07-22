#include "stdafx.h"

/*
Copyright (c) 2012, Tobias Langner
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.
*/

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

