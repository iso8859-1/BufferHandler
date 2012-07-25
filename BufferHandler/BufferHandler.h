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

#pragma warning( push )
#pragma warning( disable : 4800) //disable warning "forcing value to bool 'true' or 'false' (performance warning)

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

//Endian swap functions
inline boost::uint16_t Swap16(boost::uint16_t src)
{
	return src << 8 | src >> 8;
}
inline boost::uint32_t Swap32(boost::uint32_t src)
{
	boost::uint32_t result = src << 16 | src >> 16;
	boost::uint32_t mask = 0xFF00FF00;
	result = ((result << 8) & mask) | ((result & mask) >> 8);
	return result;
}
inline boost::uint64_t Swap64(boost::uint64_t src)
{
	boost::uint64_t result = src << 32 | src >> 32;
	boost::uint64_t mask = 0xFFFF0000FFFF0000;
	result = ((result << 16) & mask) | ((result & mask) >> 16);
	mask = 0xFF00FF00FF00FF00;
	result = ((result << 8) & mask) | ((result & mask) >> 8);
	return result;
}

template<typename SwapSize>
struct SwapPolicyNone
{
	inline static SwapSize Swap(SwapSize src) { return src; }
};

template<typename SwapSize>
struct SwapPolicySwap
{
	inline static SwapSize Swap(SwapSize src) { throw std::logic_error("swaping not implemented"); }
};

template<>
struct SwapPolicySwap<boost::uint16_t>
{
	inline static boost::uint16_t Swap(boost::uint16_t src) { return Swap16(src); }
};

template<>
struct SwapPolicySwap<boost::uint32_t>
{
	inline static boost::uint32_t Swap(boost::uint32_t src) { return Swap32(src); }
};

template<>
struct SwapPolicySwap<boost::uint64_t>
{
	inline static boost::uint64_t Swap(boost::uint64_t src) { return Swap64(src); }
};

template<>
struct SwapPolicySwap<boost::uint8_t>
{
	inline static boost::uint8_t Swap(boost::uint8_t src) { return src; }
};


/**
Interface class to read & write from a buffer
*/
class DataHandler
{
public:
	virtual void WriteULL(unsigned long long value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual void WriteLL(long long value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual void WriteUL(unsigned long value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual void WriteL(long value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual void WriteF(float value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual void WriteD(double value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual void WriteB(bool value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	
	virtual unsigned long long ReadULL(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual long long ReadLL(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual unsigned long ReadUL(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual long ReadL(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual float ReadF(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual double ReadD(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual bool ReadB(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }	
};

template <typename T, typename intermediateType, typename swapPolicy>
class AlignedDataHandler : public DataHandler
{
	BOOST_STATIC_ASSERT(sizeof(T)==sizeof(intermediateType));

	unsigned int m_startByteOffset;

	T ReadData(unsigned char* buffer, size_t bufferSize);
	void WriteData(T value, unsigned char* buffer, size_t bufferSize);
public:
	AlignedDataHandler(unsigned int startBit) : m_startByteOffset(startBit / 8) 
	{
		assert(startBit % 8 == 0);
	}
	virtual ~AlignedDataHandler(){}

	virtual void WriteULL(unsigned long long value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteLL(long long value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteUL(unsigned long value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteL(long value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteF(float value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteD(double value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteB(bool value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }
	
	virtual unsigned long long ReadULL(unsigned char* buffer, size_t bufferSize) { return static_cast<unsigned long long>(ReadData(buffer, bufferSize)); }
	virtual long long ReadLL(unsigned char* buffer, size_t bufferSize) { return static_cast<long long>(ReadData(buffer, bufferSize)); }
	virtual unsigned long ReadUL(unsigned char* buffer, size_t bufferSize){ return static_cast<unsigned long>(ReadData(buffer, bufferSize)); }
	virtual long ReadL(unsigned char* buffer, size_t bufferSize) { return static_cast<long>(ReadData(buffer, bufferSize)); }
	virtual float ReadF(unsigned char* buffer, size_t bufferSize) { return static_cast<float>(ReadData(buffer, bufferSize)); }
	virtual double ReadD(unsigned char* buffer, size_t bufferSize) { return static_cast<double>(ReadData(buffer, bufferSize)); }
	virtual bool ReadB(unsigned char* buffer, size_t bufferSize) { return static_cast<bool>(ReadData(buffer, bufferSize)); }
};

class BitDataHandler : public DataHandler
{
	unsigned int m_startByteOffset;
	unsigned int m_bitOffsetInsideByte;
	unsigned char m_readMask;
	unsigned char m_writeMask;

	bool ReadBit(unsigned char* buffer, size_t bufferSize)
	{
		return ((*reinterpret_cast<unsigned char*>(buffer+m_startByteOffset)) & m_readMask) != 0;
	}
	void WriteBit(bool value, unsigned char* buffer, size_t bufferSize)
	{
		if (value)
		{
			*reinterpret_cast<unsigned char*>(buffer+m_startByteOffset) |= m_readMask;
		}
		else
		{
			*reinterpret_cast<unsigned char*>(buffer+m_startByteOffset) &= m_writeMask;
		}
	}

public:
	BitDataHandler(unsigned int startBit) 
		: m_startByteOffset(startBit / 8)
		, m_bitOffsetInsideByte(startBit % 8)
		, m_readMask(1<<m_bitOffsetInsideByte)
		, m_writeMask(~m_readMask)
	{}

	virtual ~BitDataHandler(){}

	virtual void WriteULL(unsigned long long value, unsigned char* buffer, size_t bufferSize) { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	virtual void WriteLL(long long value, unsigned char* buffer, size_t bufferSize) { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	virtual void WriteUL(unsigned long value, unsigned char* buffer, size_t bufferSize) { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	virtual void WriteL(long value, unsigned char* buffer, size_t bufferSize) { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	virtual void WriteF(float value, unsigned char* buffer, size_t bufferSize) { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	virtual void WriteD(double value, unsigned char* buffer, size_t bufferSize) { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	virtual void WriteB(bool value, unsigned char* buffer, size_t bufferSize) { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	
	virtual unsigned long long ReadULL(unsigned char* buffer, size_t bufferSize) { return static_cast<unsigned long long>(ReadBit(buffer, bufferSize)); }
	virtual long long ReadLL(unsigned char* buffer, size_t bufferSize) { return static_cast<long long>(ReadBit(buffer, bufferSize)); }
	virtual unsigned long ReadUL(unsigned char* buffer, size_t bufferSize){ return static_cast<unsigned long>(ReadBit(buffer, bufferSize)); }
	virtual long ReadL(unsigned char* buffer, size_t bufferSize) { return static_cast<long>(ReadBit(buffer, bufferSize)); }
	virtual float ReadF(unsigned char* buffer, size_t bufferSize) { return static_cast<float>(ReadBit(buffer, bufferSize)); }
	virtual double ReadD(unsigned char* buffer, size_t bufferSize) { return static_cast<double>(ReadBit(buffer, bufferSize)); }
	virtual bool ReadB(unsigned char* buffer, size_t bufferSize) { return static_cast<bool>(ReadBit(buffer, bufferSize)); }
};

/**
	This is the slowest possible implementation for reading. Reads using memcopy - this should always work. Should be taken as seldom as possible.
*/
template<typename internalBufferType>
class GenericIntegerHandler : public DataHandler
{
private:
	unsigned int m_byteOffset;
	unsigned int m_bitOffset;
	unsigned int m_bytesToCopy;

	internalBufferType m_mask;

	internalBufferType Read(unsigned char* buffer, size_t bufferSize);
	void Write(internalBufferType value, unsigned char* buffer, size_t bufferSize);

public:
	GenericIntegerHandler(unsigned int startBit, unsigned int bitSize);
	virtual ~GenericIntegerHandler() {}

	virtual void WriteULL(unsigned long long value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual void WriteLL(long long value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual void WriteUL(unsigned long value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual void WriteL(long value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual void WriteF(float value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual void WriteD(double value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual void WriteB(bool value, unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	
	virtual unsigned long long ReadULL(unsigned char* buffer, size_t bufferSize) { return static_cast<unsigned long long>(Read(buffer,bufferSize)); }
	virtual long long ReadLL(unsigned char* buffer, size_t bufferSize) { return static_cast<long long>(Read(buffer,bufferSize)); }
	virtual unsigned long ReadUL(unsigned char* buffer, size_t bufferSize) { return static_cast<unsigned long>(Read(buffer,bufferSize)); }
	virtual long ReadL(unsigned char* buffer, size_t bufferSize) { return static_cast<long>(Read(buffer,bufferSize)); }
	virtual float ReadF(unsigned char* buffer, size_t bufferSize) { return static_cast<float>(Read(buffer,bufferSize)); }
	virtual double ReadD(unsigned char* buffer, size_t bufferSize) { return static_cast<double>(Read(buffer,bufferSize));}
	virtual bool ReadB(unsigned char* buffer, size_t bufferSize) { return static_cast<bool>(Read(buffer,bufferSize)); }
};

/**
Factory method to create the appropriate reader/writer class
*/
boost::shared_ptr<DataHandler> CreateBufferHandler(unsigned int startbit, unsigned int sizeInBits, DataType type);

//************************** Implementation Section ***********************************************************

template <typename T, typename intermediateType, typename swapPolicy>
void AlignedDataHandler<T,intermediateType,swapPolicy>::WriteData(T value, unsigned char* buffer, size_t bufferSize)
{
	assert(m_startByteOffset + sizeof(T) - 1 < bufferSize);
	intermediateType tmp = *reinterpret_cast<intermediateType*>(&value);
	intermediateType swappedIfNeeded = swapPolicy::Swap(tmp);
	//this works as long as the intermediateType has the same width as T because we just want the pattern at that location
	*reinterpret_cast<intermediateType*>(buffer+m_startByteOffset)=swappedIfNeeded;
}

template <typename T, typename intermediateType, typename swapPolicy>
T AlignedDataHandler<T,intermediateType,swapPolicy>::ReadData(unsigned char* buffer, size_t bufferSize)
{
	assert(m_startByteOffset + sizeof(T) - 1 < bufferSize);
	intermediateType tmp = *reinterpret_cast<intermediateType*>(buffer+m_startByteOffset);
	intermediateType result = swapPolicy::Swap(tmp);
	return *reinterpret_cast<T*>(&result);
}

template<typename internalBufferType>
GenericIntegerHandler<internalBufferType>::GenericIntegerHandler(unsigned int startBit, unsigned int bitSize)
	: m_byteOffset(startBit / 8)
	, m_bitOffset(startBit % 8)
	, m_bytesToCopy( (bitSize+7)/8 )
{
	assert(m_bytesToCopy <= sizeof(internalBufferType));
}

#pragma warning( pop )

#endif