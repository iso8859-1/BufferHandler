#include "BufferHandler.h"

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

#include <cassert>
#include "boost\cstdint.hpp"



boost::shared_ptr<DataHandler> CreateAlignedDataHandler(unsigned int startbit, unsigned int sizeInBits, DataType type)
{
	assert(sizeInBits == 8 || sizeInBits == 16 || sizeInBits == 32 || sizeInBits ==64);
	assert(startbit % 8 == 0);
	switch(type)
	{
	case UnsignedIntegerLittleEndian:
		switch(sizeInBits)
		{
		case 8:
			return boost::shared_ptr<DataHandler>(new AlignedDataHandler<boost::uint8_t>(startbit));
		case 16:
			return boost::shared_ptr<DataHandler>(new AlignedDataHandler<boost::uint16_t>(startbit));
		case 32:
			return boost::shared_ptr<DataHandler>(new AlignedDataHandler<boost::uint32_t>(startbit));
		case 64:
			return boost::shared_ptr<DataHandler>(new AlignedDataHandler<boost::uint64_t>(startbit));
		default:
			throw std::logic_error("not valid");
		}
	case SignedIntegerLittleEndian:
		switch(sizeInBits)
		{
		case 8:
			return boost::shared_ptr<DataHandler>(new AlignedDataHandler<boost::int8_t>(startbit));
		case 16:
			return boost::shared_ptr<DataHandler>(new AlignedDataHandler<boost::int16_t>(startbit));
		case 32:
			return boost::shared_ptr<DataHandler>(new AlignedDataHandler<boost::int32_t>(startbit));
		case 64:
			return boost::shared_ptr<DataHandler>(new AlignedDataHandler<boost::int64_t>(startbit));
		default:
			throw std::logic_error("not valid");
		}
	case FloatLittleEndian:
		switch(sizeInBits)
		{
		case 32:
			return boost::shared_ptr<DataHandler>(new AlignedDataHandler<float>(startbit));
		case 64:
			return boost::shared_ptr<DataHandler>(new AlignedDataHandler<double>(startbit));
		default:
			throw std::logic_error("not valid");
		}
	default:
		throw std::logic_error("not valid");
	}
}

boost::shared_ptr<DataHandler> CreateBufferHandler(unsigned int startbit, unsigned int sizeInBits, DataType type)
{
	if (sizeInBits == 1)
	{
		return boost::shared_ptr<BitDataHandler>(new BitDataHandler(startbit));
	}
	try
	{
		return CreateAlignedDataHandler(startbit, sizeInBits, type);
	} 
	catch(const std::logic_error&)
	{
		//try next handler type
	}
	//default implementation - does only throw exceptions.
	return boost::shared_ptr<DataHandler>(new DataHandler());
}