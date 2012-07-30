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
time, the buffer can be changed for each read/write.

The default implementation throws std::logic_error on all methods to detect errors in the implementation.
*/
class DataHandler
{
public:

	virtual void WriteUI64(boost::uint64_t value, unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual void WriteI64(boost::int64_t value, unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual void WriteUI32(boost::uint32_t , unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual void WriteI32(boost::int32_t , unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual void WriteF(float , unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual void WriteD(double , unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual void WriteB(bool , unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	
	virtual boost::uint64_t ReadUI64(unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual boost::int64_t ReadI64(unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual boost::uint32_t ReadUI32(unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual boost::int32_t ReadI32(unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual float ReadF(unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual double ReadD(unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual bool ReadB(unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }	
};

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




template <typename T, typename intermediateType, typename swapPolicy>
class AlignedDataHandler : public DataHandler
{
	BOOST_STATIC_ASSERT(sizeof(T)==sizeof(intermediateType));

	unsigned int m_startByteOffset;

	T ReadData(unsigned char* buffer, size_t bufferSize) const;
	void WriteData(T value, unsigned char* buffer, size_t bufferSize) const;
public:
	AlignedDataHandler(unsigned int startBit) : m_startByteOffset(startBit / 8) 
	{
		assert(startBit % 8 == 0);
	}
	virtual ~AlignedDataHandler(){}

	virtual void WriteUI64(boost::uint64_t value, unsigned char* buffer, size_t bufferSize) const { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteI64(boost::int64_t value, unsigned char* buffer, size_t bufferSize) const { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteUI32(boost::uint32_t value, unsigned char* buffer, size_t bufferSize) const { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteI32(boost::int32_t value, unsigned char* buffer, size_t bufferSize) const { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteF(float value, unsigned char* buffer, size_t bufferSize) const { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteD(double value, unsigned char* buffer, size_t bufferSize) const { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteB(bool value, unsigned char* buffer, size_t bufferSize) const { WriteData(static_cast<T>(value), buffer, bufferSize); }
	
	virtual boost::uint64_t ReadUI64(unsigned char* buffer, size_t bufferSize) const { return static_cast<unsigned long long>(ReadData(buffer, bufferSize)); }
	virtual boost::int64_t ReadI64(unsigned char* buffer, size_t bufferSize) const { return static_cast<long long>(ReadData(buffer, bufferSize)); }
	virtual boost::uint32_t ReadUI32(unsigned char* buffer, size_t bufferSize) const { return static_cast<unsigned long>(ReadData(buffer, bufferSize)); }
	virtual boost::int32_t ReadI32(unsigned char* buffer, size_t bufferSize) const { return static_cast<long>(ReadData(buffer, bufferSize)); }
	virtual float ReadF(unsigned char* buffer, size_t bufferSize) const { return static_cast<float>(ReadData(buffer, bufferSize)); }
	virtual double ReadD(unsigned char* buffer, size_t bufferSize) const { return static_cast<double>(ReadData(buffer, bufferSize)); }
	virtual bool ReadB(unsigned char* buffer, size_t bufferSize) const { return static_cast<bool>(ReadData(buffer, bufferSize)); }
};

class ZeroDataHandler : public DataHandler
{
public:
	ZeroDataHandler() {}
	~ZeroDataHandler() {}

	virtual void WriteUI64(boost::uint64_t , unsigned char* , size_t ) const { }
	virtual void WriteI64(boost::int64_t , unsigned char* , size_t ) const  { }
	virtual void WriteUI32(boost::uint32_t , unsigned char* , size_t ) const  { }
	virtual void WriteI32(boost::int32_t , unsigned char* , size_t ) const  { }
	virtual void WriteF(float , unsigned char* , size_t ) const  { }
	virtual void WriteD(double , unsigned char* , size_t ) const  { }
	virtual void WriteB(bool , unsigned char* , size_t ) const  { }
	
	virtual boost::uint64_t ReadUI64(unsigned char* , size_t ) const { return static_cast<unsigned long long>(0); }
	virtual boost::int64_t ReadI64(unsigned char* , size_t ) const { return static_cast<long long>(0); }
	virtual boost::uint32_t ReadUI32(unsigned char* , size_t ) const { return static_cast<unsigned long>(0); }
	virtual boost::int32_t ReadI32(unsigned char* , size_t ) const { return static_cast<long>(0); }
	virtual float ReadF(unsigned char* , size_t ) const { return static_cast<float>(0); }
	virtual double ReadD(unsigned char* , size_t ) const { return static_cast<double>(0); }
	virtual bool ReadB(unsigned char* , size_t ) const { return static_cast<bool>(0); }
};

class BitDataHandler : public DataHandler
{
	unsigned int m_startByteOffset;
	unsigned int m_bitOffsetInsideByte;
	unsigned char m_readMask;
	unsigned char m_writeMask;

	bool ReadBit(unsigned char* buffer, size_t ) const
	{
		return ((*reinterpret_cast<unsigned char*>(buffer+m_startByteOffset)) & m_readMask) != 0;
	}
	void WriteBit(bool value, unsigned char* buffer, size_t ) const
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

	virtual void WriteUI64(boost::uint64_t value, unsigned char* buffer, size_t bufferSize) const { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	virtual void WriteI64(boost::int64_t value, unsigned char* buffer, size_t bufferSize) const { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	virtual void WriteUI32(boost::uint32_t value, unsigned char* buffer, size_t bufferSize) const { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	virtual void WriteI32(boost::int32_t value, unsigned char* buffer, size_t bufferSize) const { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	virtual void WriteF(float value, unsigned char* buffer, size_t bufferSize) const { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	virtual void WriteD(double value, unsigned char* buffer, size_t bufferSize) const { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	virtual void WriteB(bool value, unsigned char* buffer, size_t bufferSize) const { WriteBit(static_cast<bool>(value), buffer, bufferSize); }
	
	virtual boost::uint64_t ReadUI64(unsigned char* buffer, size_t bufferSize) const { return static_cast<unsigned long long>(ReadBit(buffer, bufferSize)); }
	virtual boost::int64_t ReadI64(unsigned char* buffer, size_t bufferSize) const { return static_cast<long long>(ReadBit(buffer, bufferSize)); }
	virtual boost::uint32_t ReadUI32(unsigned char* buffer, size_t bufferSize) const { return static_cast<unsigned long>(ReadBit(buffer, bufferSize)); }
	virtual boost::int32_t ReadI32(unsigned char* buffer, size_t bufferSize) const { return static_cast<long>(ReadBit(buffer, bufferSize)); }
	virtual float ReadF(unsigned char* buffer, size_t bufferSize) const { return static_cast<float>(ReadBit(buffer, bufferSize)); }
	virtual double ReadD(unsigned char* buffer, size_t bufferSize) const { return static_cast<double>(ReadBit(buffer, bufferSize)); }
	virtual bool ReadB(unsigned char* buffer, size_t bufferSize) const { return static_cast<bool>(ReadBit(buffer, bufferSize)); }
};

template<typename T>
struct SignExtensionPolicyNone
{
	SignExtensionPolicyNone(unsigned int ) { }
	T Extend(T value) const { return value; }
};

template<typename T>
struct SignExtensionPolicyExtend
{
	T mask;
	SignExtensionPolicyExtend(unsigned int bitSize) : mask( (~0) << (bitSize-1) )
	{
		//create a mask that contains only 1 except for the lowest bitSize-1 bits
	}
	T Extend(T value) const 
	{
		//check whether the sign bit is set using the mask. If it is set, set all bits except for the lowest bitSize-1
		//this is sufficient since we know that the bit at position bitSize (sign bit) is equal to 1
		return value & mask ? value | mask : value; 
	}
};

template<typename T>
struct EndianessPolicyNoSwap
{
	unsigned int shift;
	T mask;

	EndianessPolicyNoSwap(unsigned int startBit, unsigned int bitSize) 
		: shift(startBit % 8)
		, mask( 0 )
	{
		mask = ~static_cast<T>(0);
		mask <<= bitSize;
		mask = ~mask;
	}
	T Align(T value) const{ return value >> shift; }
	T InverseAlign(T value) const { return value << shift; }
	T ApplyMask(T value) const { return value & mask; }
	T Swap(T value) const { return value; }
};

template<typename T>
struct EndianessPolicySwap
{
	unsigned int shiftL;
	unsigned int shiftR;
	T mask;

	EndianessPolicySwap(unsigned int startBit, unsigned int bitSize) 
		: shiftR( startBit % 8 )
		, shiftL( (sizeof(T) - (bitSize + 7)/8)*8)
		, mask( 0 )
	{
		mask = ~static_cast<T>(0);
		mask <<= bitSize;
		mask = ~mask;
	}
	T Align(T value) const
	{ 
		T result = value >> shiftR;
		return result << shiftL; 
	} //the lowest value bit of the BE value must be a bit 0 of a byte
	T InverseAlign(T value) const { return value << shift; }
	T ApplyMask(T value) const { return value & mask; }
	T Swap(T value) const;
};

template<>
inline boost::uint8_t EndianessPolicySwap<boost::uint8_t>::Swap(boost::uint8_t value) const
{
	return value;
}

template<>
inline boost::uint16_t EndianessPolicySwap<boost::uint16_t>::Swap(boost::uint16_t value) const
{
	return Swap16(value);
}

template<>
inline boost::uint32_t EndianessPolicySwap<boost::uint32_t>::Swap(boost::uint32_t value) const
{
	return Swap32(value);
}

template<>
inline boost::uint64_t EndianessPolicySwap<boost::uint64_t>::Swap(boost::uint64_t value) const
{
	return Swap64(value);
}

template<typename T>
inline T EndianessPolicySwap<T>::Swap(T value) const
{
	throw std::logic_error("swaping not implemented");
}

/**
	This is the slowest possible implementation for reading. Reads using memcopy - this should always work. Should be taken as seldom as possible.
*/
template<typename internalBufferType, typename reinterpretType, typename endianessPolicy, typename signPolicy>
class GenericIntegerHandler : public DataHandler, private endianessPolicy, private signPolicy
{
private:
	unsigned int m_byteOffset;
	unsigned int m_bitOffset;
	unsigned int m_bytesToCopy;

	internalBufferType Read(unsigned char* buffer, size_t bufferSize) const;
	void Write(internalBufferType value, unsigned char* buffer, size_t bufferSize) const;

public:
	GenericIntegerHandler(unsigned int startBit, unsigned int bitSize);
	virtual ~GenericIntegerHandler() {}

	virtual void WriteUI64(boost::uint64_t value, unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual void WriteI64(boost::int64_t value, unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual void WriteUI32(boost::uint32_t value, unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual void WriteI32(boost::int32_t value, unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual void WriteF(float value, unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual void WriteD(double value, unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	virtual void WriteB(bool value, unsigned char* buffer, size_t bufferSize) const { throw std::logic_error("not implemented"); }
	
	virtual boost::uint64_t ReadUI64(unsigned char* buffer, size_t bufferSize) const 
	{ 
		internalBufferType result = Read(buffer,bufferSize);
		return static_cast<unsigned long long>(*reinterpret_cast<reinterpretType*>(&result)); 
	}
	virtual boost::int64_t ReadI64(unsigned char* buffer, size_t bufferSize) const 
	{ 
		internalBufferType result = Read(buffer,bufferSize);
		return static_cast<long long>(*reinterpret_cast<reinterpretType*>(&result)); 
	}
	virtual boost::uint32_t ReadUI32(unsigned char* buffer, size_t bufferSize) const 
	{ 
		internalBufferType result = Read(buffer,bufferSize);
		return static_cast<unsigned long>(*reinterpret_cast<reinterpretType*>(&result)); 
	}
	virtual boost::int32_t ReadI32(unsigned char* buffer, size_t bufferSize) const
	{ 
		internalBufferType result = Read(buffer,bufferSize);
		return static_cast<long>(*reinterpret_cast<reinterpretType*>(&result)); 
	}
	virtual float ReadF(unsigned char* buffer, size_t bufferSize) const
	{ 
		internalBufferType result = Read(buffer,bufferSize);
		return static_cast<float>(*reinterpret_cast<reinterpretType*>(&result)); 
	}
	virtual double ReadD(unsigned char* buffer, size_t bufferSize) const
	{ 
		internalBufferType result = Read(buffer,bufferSize);
		return static_cast<double>(*reinterpret_cast<reinterpretType*>(&result)); 
	}
	virtual bool ReadB(unsigned char* buffer, size_t bufferSize) const
	{ 
		internalBufferType result = Read(buffer,bufferSize);
		return static_cast<bool>(*reinterpret_cast<reinterpretType*>(&result)); 
	}
};

/**
Factory method to create the appropriate reader/writer class
*/
boost::shared_ptr<DataHandler> CreateBufferHandler(unsigned int startbit, unsigned int sizeInBits, DataType type);

//************************** Implementation Section ***********************************************************

template <typename T, typename intermediateType, typename swapPolicy>
void AlignedDataHandler<T,intermediateType,swapPolicy>::WriteData(T value, unsigned char* buffer, size_t bufferSize) const
{
	assert(m_startByteOffset + sizeof(T) - 1 < bufferSize);
	intermediateType tmp = *reinterpret_cast<intermediateType*>(&value);
	intermediateType swappedIfNeeded = swapPolicy::Swap(tmp);
	//this works as long as the intermediateType has the same width as T because we just want the pattern at that location
	*reinterpret_cast<intermediateType*>(buffer+m_startByteOffset)=swappedIfNeeded;
}

template <typename T, typename intermediateType, typename swapPolicy>
T AlignedDataHandler<T,intermediateType,swapPolicy>::ReadData(unsigned char* buffer, size_t bufferSize) const
{
	assert(m_startByteOffset + sizeof(T) - 1 < bufferSize);
	intermediateType tmp = *reinterpret_cast<intermediateType*>(buffer+m_startByteOffset);
	intermediateType result = swapPolicy::Swap(tmp);
	return *reinterpret_cast<T*>(&result);
}

template<typename internalBufferType, typename reinterpretType, typename endianessPolicy, typename signPolicy>
GenericIntegerHandler<internalBufferType,reinterpretType,endianessPolicy,signPolicy>::GenericIntegerHandler(unsigned int startBit, unsigned int bitSize)
	: signPolicy(bitSize), endianessPolicy(startBit, bitSize)
	, m_byteOffset(startBit / 8)
	, m_bitOffset(startBit % 8)
	, m_bytesToCopy( (bitSize+(startBit%8)+7)/8 )
{
	assert(m_bytesToCopy <= sizeof(internalBufferType));
}
template<typename internalBufferType, typename reinterpretType, typename endianessPolicy, typename signPolicy>
internalBufferType GenericIntegerHandler<internalBufferType,reinterpretType,endianessPolicy,signPolicy>::Read(unsigned char* buffer, size_t bufferSize) const
{
	//coyp into internal buffer
	internalBufferType result = 0;
	memcpy(&result,buffer+m_byteOffset,m_bytesToCopy);
	//align (right if LE, left if BE)
	result = Align(result);
	//swap if necessary
	result = Swap(result);
	//apply mask (depending on alignment)
	result = ApplyMask(result);
	//sign extension if necessary
	return Extend(result);
}

template<typename internalBufferType, typename reinterpretType, typename endianessPolicy, typename signPolicy>
void GenericIntegerHandler<internalBufferType,reinterpretType,endianessPolicy,signPolicy>::Write(internalBufferType value, unsigned char* buffer, size_t bufferSize) const
{
	//swap if necessary
	auto tmp = Swap(value);
	//mask
	tmp = ApplyMask(tmp);
	//align
	tmp = InverseAlign(tmp);
	//bytewise or into place
	for (unsigned int i=0; i<m_bytesToCopy; ++i)
	{
		buffer[m_byteOffset+i] |= (reinterpret_cast<unsigned char*>(&tmp))[i];
	}
}

#pragma warning( pop )

#endif