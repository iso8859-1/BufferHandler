#ifndef BUFFERHANDLER_H
#define BUFFERHANDLER_H

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
#include <cstring>
#include <stdexcept>
#include <cassert>
#include <boost/smart_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>


namespace BufferHandler
{

/**
enum indicating the datatype inside the buffer
*/
enum DataType
{
	SignedIntegerLittleEndian,
	UnsignedIntegerLittleEndian,
	SignedIntegerBigEndian,
	UnsignedIntegerBigEndian,
	FloatLittleEndian,
	FloatBigEndian
};

/**
Swaps the 2 bytes of a 16bit value. Uses the built-in intrinsic of Visual Studio if available.
Example: 0x0102 --> 0x0201
@param src value to be swapped
@return swapped value
*/
inline boost::uint16_t Swap16(boost::uint16_t src)
{
#if (_MSC_VER >= 1400)
	return _byteswap_ushort(src);
#else
	return src << 8 | src >> 8;
#endif
}

/**
Swaps the 4 bytes of a 32bit value. Uses the built-in intrinsic of Visual Studio if available.
Example: 0x01020304 --> 0x04030201
@param src value to be swapped
@return swapped value
*/
inline boost::uint32_t Swap32(boost::uint32_t src)
{
#if (_MSC_VER >= 1400)
	return _byteswap_ulong(src);
#else
	boost::uint32_t result = src << 16 | src >> 16;
	boost::uint32_t mask = 0xFF00FF00;
	result = ((result << 8) & mask) | ((result & mask) >> 8);
	return result;
#endif
}

/**
Swaps the 8 bytes of a 64bit value. Uses the built-in intrinsic of Visual Studio if available.
Example: 0x01020304050607 --> 0x00706054030201
@param src value to be swapped
@return swapped value
*/
inline boost::uint64_t Swap64(boost::uint64_t src)
{
#if (_MSC_VER >= 1400)
	return _byteswap_uint64(src);
#else
	boost::uint64_t result = src << 32 | src >> 32;
	boost::uint64_t mask = 0xFFFF0000FFFF0000;
	result = ((result << 16) & mask) | ((result & mask) >> 16);
	mask = 0xFF00FF00FF00FF00;
	result = ((result << 8) & mask) | ((result & mask) >> 8);
	return result;
#endif
}

/**
Interface class to read & write from a buffer at a specific location. The specific location is defined at creation
time, the buffer can be changed for each read/write. The specification consists of data type, position and size of
the data.

The default implementation throws std::logic_error on all methods to detect errors in the implementation.
*/
class DataHandler
{
public:
	/**
	Writes a 64bit unsigned integer to the slot specified at creation using \ref CreateBufferHandler.
	@param value value to be written.
	@param buffer buffer to be written to.
	@param bufferSize size of the buffer to be written to.
	*/
	virtual void WriteUI64(boost::uint64_t value, unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	/**
	Writes a 64bit integer to the slot specified at creation using \ref CreateBufferHandler.
	@param value value to be written.
	@param buffer buffer to be written to.
	@param bufferSize size of the buffer to be written to.
	*/
	virtual void WriteI64(boost::int64_t value, unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	/**
	Writes a 32bit unsigned integer to the slot specified at creation using \ref CreateBufferHandler.
	@param value value to be written.
	@param buffer buffer to be written to.
	@param bufferSize size of the buffer to be written to.
	*/
	virtual void WriteUI32(boost::uint32_t , unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	/**
	Writes a 32bit integer to the slot specified at creation using \ref CreateBufferHandler.
	@param value value to be written.
	@param buffer buffer to be written to.
	@param bufferSize size of the buffer to be written to.
	*/
	virtual void WriteI32(boost::int32_t , unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	/**
	Writes a 32bit float to the slot specified at creation using \ref CreateBufferHandler.
	@param value value to be written.
	@param buffer buffer to be written to.
	@param bufferSize size of the buffer to be written to.
	*/
	virtual void WriteF(float , unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	/**
	Writes a 64bit double to the slot specified at creation using \ref CreateBufferHandler.
	@param value value to be written.
	@param buffer buffer to be written to.
	@param bufferSize size of the buffer to be written to.
	*/
	virtual void WriteD(double , unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	/**
	Writes a 1bit boolean to the slot specified at creation using \ref CreateBufferHandler.
	@param value value to be written.
	@param buffer buffer to be written to.
	@param bufferSize size of the buffer to be written to.
	*/
	virtual void WriteB(bool , unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	

	/**
	Reads the value at the slot specified during creation using \ref CreateBufferHandler and converts it to a 64bit unsigned
	integer.
	@param buffer buffer to be read from
	@param bufferSize size of the buffer
	@return converted value from the buffer
	*/
	virtual boost::uint64_t ReadUI64(const unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	/**
	Reads the value at the slot specified during creation using \ref CreateBufferHandler and converts it to a 64bit 
	integer.
	@param buffer buffer to be read from
	@param bufferSize size of the buffer
	@return converted value from the buffer
	*/
	virtual boost::int64_t ReadI64(const unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	/**
	Reads the value at the slot specified during creation using \ref CreateBufferHandler and converts it to a 32bit unsigned
	integer.
	@param buffer buffer to be read from
	@param bufferSize size of the buffer
	@return converted value from the buffer
	*/
	virtual boost::uint32_t ReadUI32(const unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	/**
	Reads the value at the slot specified during creation using \ref CreateBufferHandler and converts it to a 32bit 
	integer.
	@param buffer buffer to be read from
	@param bufferSize size of the buffer
	@return converted value from the buffer
	*/
	virtual boost::int32_t ReadI32(const unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	/**
	Reads the value at the slot specified during creation using \ref CreateBufferHandler and converts it to a 32bit float
	@param buffer buffer to be read from
	@param bufferSize size of the buffer
	@return converted value from the buffer
	*/
	virtual float ReadF(const unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	/**
	Reads the value at the slot specified during creation using \ref CreateBufferHandler and converts it to a 64bit double
	@param buffer buffer to be read from
	@param bufferSize size of the buffer
	@return converted value from the buffer
	*/
	virtual double ReadD(const unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }
	/**
	Reads the value at the slot specified during creation using \ref CreateBufferHandler and converts it to a 1bit bool
	@param buffer buffer to be read from
	@param bufferSize size of the buffer
	@return converted value from the buffer
	*/
	virtual bool ReadB(const unsigned char* buffer, size_t bufferSize) const = 0 { throw std::logic_error("not implemented"); }	
};

/**
Factory method to create the appropriate reader/writer class. The fastest implementation for the given combination of
startbit, sizeInBits and DataType is chosen.

@param startbit first bit of the data inside of the buffer
@param sizeInBits number of bits for the data
@param DataType determines how the data is interpreted (Integer / Float, Little or Big Endian)
@return reader/writer for this field in the buffer
*/
boost::shared_ptr<DataHandler> CreateBufferHandler(unsigned int startbit, unsigned int sizeInBits, DataType type);

}

#endif