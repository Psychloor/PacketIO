#ifndef PACKET_READER_HPP
#define PACKET_READER_HPP

#include <cstdint>
#include <memory>
#include <span>
#include <stdexcept>
#include <vector>
#include <boost/endian/conversion.hpp>

namespace packet_io
{
	class packet_reader : std::enable_shared_from_this<packet_reader>
	{
	public:
		explicit packet_reader(std::span<uint8_t> buffer);

		template<typename T>
		T read_le();

		template<typename T>
		T read_be();

		template<typename T>
		T read_raw();

		template <typename LengthType = uint16_t>
		std::string read_string_le();

		template <typename LengthType = uint16_t>
		std::string read_string_be();

		std::string read_cstring();
		std::vector<uint8_t> read(std::size_t size);

		size_t offset() const { return _offset; }
		bool can_read(const std::size_t size) const { return _offset + size <= _length; }

	private:
		void read_raw(void* data, std::size_t size);


		std::span<const uint8_t> _buffer;
		size_t _offset;
		size_t _length;
	};

	template <typename T>
	T packet_reader::read_le()
	{
		static_assert(std::is_integral_v<T>, "Type must be integral");

		if (_offset + sizeof(T) > _length) {
			throw std::out_of_range("Buffer overflow in read_le");
		}

		T value;
		std::memcpy(&value, _buffer.data() + _offset, sizeof(T));
		_offset += sizeof(T);

		return boost::endian::little_to_native(value);
	}

	template <typename T>
	T packet_reader::read_be()
	{
		static_assert(std::is_integral_v<T>, "Type must be integral");

		if (_offset + sizeof(T) > _length) {
			throw std::out_of_range("Buffer overflow in read_be");  // Changed from read_le
		}

		T value;
		std::memcpy(&value, _buffer.data() + _offset, sizeof(T));
		_offset += sizeof(T);

		return boost::endian::big_to_native(value);
	}

	template <typename T>
	T packet_reader::read_raw()
	{
		static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");

		if (_offset + sizeof(T) > _length) {
			throw std::out_of_range("Buffer overflow in read_le");
		}

		T value;
		std::memcpy(&value, _buffer.data() + _offset, sizeof(T));
		_offset += sizeof(T);

		return value;
	}

	template <typename LengthType>
	std::string packet_reader::read_string_le()
	{
		static_assert(std::is_integral_v<LengthType>, "Length type must be integral");

		const auto length = read_le<LengthType>();
		auto data = read(length);

		return std::string(data.begin(), data.end());
	}

	template <typename LengthType>
	std::string packet_reader::read_string_be()
	{
		static_assert(std::is_integral_v<LengthType>, "Length type must be integral");

		const auto length = read_be<LengthType>();
		auto data = read(length);

		return std::string(data.begin(), data.end());
	}

} // packet_io

#endif //PACKET_READER_HPP
