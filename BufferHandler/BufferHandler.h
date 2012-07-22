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

	
	virtual unsigned long long ReadULL(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual long long ReadLL(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual unsigned long ReadUL(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual long ReadL(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual float ReadF(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
	virtual double ReadD(unsigned char* buffer, size_t bufferSize) { throw std::logic_error("not implemented"); }
};

template <typename T>
class AlignedDataHandler : public DataHandler
{
	unsigned int startByteOffset;

	T ReadData(char* buffer, size_t bufferSize);
	void WriteData(T value, unsigned char* buffer, size_t bufferSize);
public:
	AlignedDataHandler(unsigned int startBit, unsigned int bitSize);

	virtual void WriteULL(unsigned long long value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteLL(long long value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteUL(unsigned long value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteL(long value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteF(float value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }
	virtual void WriteD(double value, unsigned char* buffer, size_t bufferSize) { WriteData(static_cast<T>(value), buffer, bufferSize); }

	
	virtual unsigned long long ReadULL(unsigned char* buffer, size_t bufferSize) { return static_cast<unsigned long long>(Read(buffer, bufferSize)); }
	virtual long long ReadLL(unsigned char* buffer, size_t bufferSize) { return static_cast<long long>(Read(buffer, bufferSize)); }
	virtual unsigned long ReadUL(unsigned char* buffer, size_t bufferSize){ return static_cast<unsigned long>(Read(buffer, bufferSize)); }
	virtual long ReadL(unsigned char* buffer, size_t bufferSize) { return static_cast<long>(Read(buffer, bufferSize)); }
	virtual float ReadF(unsigned char* buffer, size_t bufferSize) { return static_cast<float>(Read(buffer, bufferSize)); }
	virtual double ReadD(unsigned char* buffer, size_t bufferSize) { return static_cast<double>(Read(buffer, bufferSize)); }
};

/**
Factory method to create the appropriate reader/writer class
*/
DataHandler CreateBufferHandler(unsigned int startbit, unsigned int sizeInBits, DataType type);

//************************** Implementation Section ***********************************************************


#endif