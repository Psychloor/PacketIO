//
// Created by blomq on 2025-07-10.
//

#include "packet_reader.hpp"

namespace packet_io {
	packet_reader::packet_reader(const std::span<uint8_t> buffer)
	{
		_buffer = buffer;
		_length = buffer.size();
		_offset = 0;
	}

	std::string packet_reader::read_cstring()
	{
		std::string value;
		while (true) {
			const auto c = read_raw<uint8_t>();
			if (c == 0) {
				break;
			}
			value.push_back(c);
		}

		return value;
	}

	std::vector<uint8_t> packet_reader::read(const std::size_t size)
	{
		if (_offset + size > _length) {
			throw std::out_of_range("Buffer overflow in read");
		}

		std::vector<uint8_t> data(size);
		std::memcpy(data.data(), _buffer.data() + _offset, size);
		_offset += size;
		return data;
	}

	void packet_reader::read_raw(void* data, const std::size_t size)
	{
		if (_offset + size > _length) {
			throw std::out_of_range("Buffer overflow in read_raw");
		}

		std::memcpy(data, _buffer.data() + _offset, size);
		_offset += size;
	}
} // packet_io