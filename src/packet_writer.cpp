//
// Created by blomq on 2025-07-10.
//

#include "packet_writer.hpp"

#include <iterator>

namespace packet_io
{
	void packet_writer::write_raw(const void* data, const std::size_t size)
	{
		std::copy_n(static_cast<const uint8_t*>(data), size, std::back_inserter(_buffer));
	}

	void packet_writer::write(const uint8_t value)
	{
		_buffer.push_back(value);
	}

	packet_writer::packet_writer(const std::size_t reserved)
	{
		if (reserved > 0)
		{
			_buffer.reserve(reserved);
		}
	}

	void packet_writer::write_cstring(const std::string_view value)
	{
		const auto data = value.data();
		const auto size = value.size();
		write_raw(data, size);
		write(0);
	}

	void packet_writer::write(const std::string_view value)
	{
		write_raw(value.data(), value.size());
	}

	void packet_writer::write(const char* data, const std::size_t size)
	{
		write_raw(data, size);
	}
} // packet_io
