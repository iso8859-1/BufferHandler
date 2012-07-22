#include "BufferHandler.h"
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