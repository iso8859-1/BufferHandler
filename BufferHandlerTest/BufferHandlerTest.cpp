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

#pragma region EndianSwap Tests
BOOST_AUTO_TEST_CASE ( Swap16Test )
{
	boost::uint16_t testvalue = 0x0102;
	auto result = Swap16(testvalue);
	BOOST_CHECK(result == 0x0201);
	auto result2 = Swap16(result);
	BOOST_CHECK(result2 = testvalue);
}
BOOST_AUTO_TEST_CASE ( Swap32Test )
{
	boost::uint32_t testvalue = 0x01020304;
	auto result = Swap32(testvalue);
	BOOST_CHECK(result == 0x04030201);
	auto result2 = Swap32(result);
	BOOST_CHECK(result2 = testvalue);
}
BOOST_AUTO_TEST_CASE ( Swap64Test )
{
	boost::uint64_t testvalue = 0x0102030405060708;
	auto result = Swap64(testvalue);
	BOOST_CHECK(result == 0x0807060504030201);
	auto result2 = Swap64(result);
	BOOST_CHECK(result2 = testvalue);
}
#pragma endregion

#pragma region Policy Tests

BOOST_AUTO_TEST_CASE(EndianessPolicyNoSwapTest)
{
	{
		EndianessPolicyNoSwap<boost::uint32_t> policy(7,8);
		boost::uint32_t value = 0x7F80; // 111111110000000 //8 1s shifted by 7 to the left
		auto result = policy.Align(value);
		BOOST_CHECK(result == 0xFF);
	}
	{
		EndianessPolicyNoSwap<boost::uint32_t> policy(7,8);
		boost::uint32_t value = 0xFFFFFFFF; // 8 1s hidden inside 4 bytes of 1s
		auto result = policy.Align(value);
		result = policy.ApplyMask(value);
		BOOST_CHECK(result == 0xFF);
		result = policy.Swap(result);
		BOOST_CHECK(result == 0xFF);
	}
}

BOOST_AUTO_TEST_CASE(SignExtensionPolicyExtendTest)
{
	SignExtensionPolicyExtend<boost::uint32_t> policy(15);

	{
		boost::uint32_t value = 0x7FFF;
		auto extended = policy.Extend(value);
		BOOST_CHECK(extended == 0xFFFFFFFF);
	}

	{
		boost::uint32_t value = 0x3FFF;
		auto extended = policy.Extend(value);
		BOOST_CHECK(extended == 0x3FFF);
	}

	
}

BOOST_AUTO_TEST_CASE(SignExtensionPolicyNoneTest)
{
	SignExtensionPolicyNone<boost::uint32_t> policy(15);

	{
		boost::uint32_t value = 0x7FFF;
		auto extended = policy.Extend(value);
		BOOST_CHECK(extended == 0x7FFF);
	}

	{
		boost::uint32_t value = 0x3FFF;
		auto extended = policy.Extend(value);
		BOOST_CHECK(extended == 0x3FFF);
	}
}


#pragma endregion

#pragma region Reading Tests
#pragma region Lower Boundary Tests
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
#pragma endregion
#pragma region aligned access tests
BOOST_AUTO_TEST_CASE(genericAlignedAccessTest)
{
	unsigned char buffer[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	for (int i=8; i<=64; i*=2)
	{
		for (unsigned int offset = 0; offset<sizeof(buffer)*8-i+1; offset+=8)
		{
			auto h = CreateBufferHandler(offset, i, UnsignedIntegerLittleEndian);
			{
				auto value = h->ReadULL(&buffer[0],sizeof(buffer));
				unsigned long long expected = 0;
				for (int counter = i; counter > 0; counter -=8)
				{
					expected = (expected << 8) + buffer[(offset+counter)/8-1];
				}
				BOOST_CHECK(value == expected);
			}
			{
				auto value = h->ReadLL(&buffer[0],sizeof(buffer));
				long long expected = 0;
				for (int counter = i; counter > 0; counter -=8)
				{
					expected = (expected << 8) + buffer[(offset+counter)/8-1];
				}
				BOOST_CHECK(value == expected);
			}
			{
				auto value = h->ReadD(&buffer[0],sizeof(buffer));
				unsigned long long expected = 0;
				for (int counter = i; counter > 0; counter -=8)
				{
					expected = (expected << 8) + buffer[(offset+counter)/8-1];
				}
				BOOST_CHECK(value == expected);
			}
			if (i<=32)
			{
				{
				auto value = h->ReadUL(&buffer[0],sizeof(buffer));
				unsigned long long expected = 0;
				for (int counter = i; counter > 0; counter -=8)
				{
					expected = (expected << 8) + buffer[(offset+counter)/8-1];
				}
				BOOST_CHECK(value == expected);
				}
				{
					auto value = h->ReadL(&buffer[0],sizeof(buffer));
					long long expected = 0;
					for (int counter = i; counter > 0; counter -=8)
					{
						expected = (expected << 8) + buffer[(offset+counter)/8-1];
					}
					BOOST_CHECK(value == expected);
				}
			}
			if (i<=16)
			{
				{
					auto value = h->ReadF(&buffer[0],sizeof(buffer));
					long long expected = 0;
					for (int counter = i; counter > 0; counter -=8)
					{
						expected = (expected << 8) + buffer[(offset+counter)/8-1];
					}
					BOOST_CHECK(value == expected);
				}
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(genericAlignedAccessTestBigEndian)
{
	unsigned char buffer[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	for (int i=8; i<=64; i*=2)
	{
		for (unsigned int offset = 0; offset<sizeof(buffer)*8-i+1; offset+=8)
		{
			auto h = CreateBufferHandler(offset, i, UnsignedIntegerBigEndian);
			{
				auto value = h->ReadULL(&buffer[0],sizeof(buffer));
				unsigned long long expected = 0;
				for (int counter = 8; counter <= i; counter +=8)
				{
					expected = (expected << 8) + buffer[(offset+counter)/8-1];
				}
				BOOST_CHECK(value == expected);
			}
			{
				auto value = h->ReadLL(&buffer[0],sizeof(buffer));
				long long expected = 0;
				for (int counter = 8; counter <= i; counter +=8)
				{
					expected = (expected << 8) + buffer[(offset+counter)/8-1];
				}
				BOOST_CHECK(value == expected);
			}
			{
				auto value = h->ReadD(&buffer[0],sizeof(buffer));
				unsigned long long expected = 0;
				for (int counter = 8; counter <= i; counter +=8)
				{
					expected = (expected << 8) + buffer[(offset+counter)/8-1];
				}
				BOOST_CHECK(value == expected);
			}
			if (i<=32)
			{
				{
				auto value = h->ReadUL(&buffer[0],sizeof(buffer));
				unsigned long long expected = 0;
				for (int counter = 8; counter <= i; counter +=8)
				{
					expected = (expected << 8) + buffer[(offset+counter)/8-1];
				}
				BOOST_CHECK(value == expected);
				}
				{
					auto value = h->ReadL(&buffer[0],sizeof(buffer));
					long long expected = 0;
					for (int counter = 8; counter <= i; counter +=8)
					{
						expected = (expected << 8) + buffer[(offset+counter)/8-1];
					}
					BOOST_CHECK(value == expected);
				}
			}
			if (i<=16)
			{
				{
					auto value = h->ReadF(&buffer[0],sizeof(buffer));
					long long expected = 0;
					for (int counter = 8; counter <= i; counter +=8)
					{
						expected = (expected << 8) + buffer[(offset+counter)/8-1];
					}
					BOOST_CHECK(value == expected);
				}
			}
		}
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
#pragma endregion
#pragma region bit access tests
BOOST_AUTO_TEST_CASE( bitAccess )
{
	unsigned char buffer[10] = { 0,0xff,2,3,4,5,6,7,8,9};
	auto h = CreateBufferHandler(3,1,SignedIntegerLittleEndian);
	auto h2 = CreateBufferHandler(17,1,FloatLittleEndian);

	{
		auto value = h->ReadB(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == false);
	}
	{
		auto value = h2->ReadB(&buffer[0],sizeof(buffer));
		BOOST_CHECK(value == true);
	}
}
#pragma endregion
#pragma endregion

#pragma region Writing Tests
#pragma region Aligned Writing Test
BOOST_AUTO_TEST_CASE(alignedWritingTestUILE8Bit)
{
	unsigned char buffer[10] = { 0,1,2,3,4,5,6,7,8,9};
	auto h = CreateBufferHandler(8,8,UnsignedIntegerLittleEndian);

	{
		h->WriteLL(2,&buffer[0],sizeof(buffer));
		BOOST_CHECK(buffer[1] == 2);
	}
	{
		h->WriteL(3,&buffer[0],sizeof(buffer));
		BOOST_CHECK(buffer[1] == 3);
	}
	{
		h->WriteULL(4,&buffer[0],sizeof(buffer));
		BOOST_CHECK(buffer[1] == 4);
	}
	{
		h->WriteUL(5,&buffer[0],sizeof(buffer));
		BOOST_CHECK(buffer[1] == 5);
	}
	{
		h->WriteF(6,&buffer[0],sizeof(buffer));
		BOOST_CHECK(buffer[1] == 6);
	}
	{
		h->WriteD(7,&buffer[0],sizeof(buffer));
		BOOST_CHECK(buffer[1] == 7);
	}
}

BOOST_AUTO_TEST_CASE(alignedWritingTestSILE8Bit)
{
	unsigned char buffer[10] = { 0,-1,2,3,4,5,6,7,8,9};
	auto h = CreateBufferHandler(8,8,SignedIntegerLittleEndian);

	{
		h->WriteLL(-2,&buffer[0],sizeof(buffer));
		BOOST_CHECK(reinterpret_cast<char&>(buffer[1]) == -2);
	}
	{
		h->WriteL(-3,&buffer[0],sizeof(buffer));
		BOOST_CHECK(reinterpret_cast<char&>(buffer[1]) == -3);
	}
	{
		h->WriteF(-6,&buffer[0],sizeof(buffer));
		BOOST_CHECK(reinterpret_cast<char&>(buffer[1]) == -6);
	}
	{
		h->WriteD(-7,&buffer[0],sizeof(buffer));
		BOOST_CHECK(reinterpret_cast<char&>(buffer[1]) == -7);
	}
}
#pragma endregion
#pragma region Bit Writing Tests
BOOST_AUTO_TEST_CASE( bitWriting )
{
	unsigned char buffer[10] = {0,0,0xFF,0,0,0,0,0,0,0};
	auto h = CreateBufferHandler(3,1,SignedIntegerLittleEndian);
	auto h2 = CreateBufferHandler(17,1,FloatLittleEndian);

	{
		h->WriteB(true,&buffer[0],sizeof(buffer));
		BOOST_CHECK(buffer[0] == 8); //2^3
	}
	{
		h2->WriteB(false, &buffer[0],sizeof(buffer));
		BOOST_CHECK(buffer[2] == 0xFF-2);
	}
}
#pragma endregion
#pragma endregion
