//
// Created by blomq on 2025-07-10.
//

#ifndef PACKET_WRITER_HPP
#define PACKET_WRITER_HPP

#include <cstdint>
#include <memory>
#include <string_view>
#include <boost/endian/conversion.hpp>
#include <type_traits>
#include <vector>

namespace packet_io
{
	class packet_writer : std::enable_shared_from_this<packet_writer>
	{
	public:
		explicit packet_writer(std::size_t reserved = 0);

		template <typename T>
		void write_le(const T& value);

		template <typename T>
		void write_be(const T& value);

		template <typename LengthType = uint16_t>
		void write_string_le(std::string_view value);

		template <typename LengthType = uint16_t>
		void write_string_be(std::string_view value);

		void write_cstring(std::string_view value);
		void write(std::string_view value);

		void write(const char* data, std::size_t size);
		void write(std::byte value);

		size_t size() const { return _buffer.size(); }
		const std::vector<std::byte>& buffer() const { return _buffer; }

	private:
		void write_raw(const void* data, std::size_t size);

		std::vector<std::byte> _buffer;
	};

	template <typename T>
	void packet_writer::write_le(const T& value)
	{
		static_assert(std::is_integral_v<T>, "Type must be integral");
		T le_value = boost::endian::native_to_little(value);
		write_raw(&le_value, sizeof(T));
	}

	template <typename T>
	void packet_writer::write_be(const T& value)
	{
		static_assert(std::is_integral_v<T>, "Type must be integral");
		T be_value = boost::endian::native_to_big(value);
		write_raw(&be_value, sizeof(T));
	}

	template <typename LengthType>
	void packet_writer::write_string_le(const std::string_view value)
	{
		static_assert(std::is_integral_v<LengthType>, "Length type must be integral");
		write_le<LengthType>(static_cast<LengthType>(value.size()));
		write_raw(value.data(), value.size());
	}

	template <typename LengthType>
	void packet_writer::write_string_be(const std::string_view value)
	{
		static_assert(std::is_integral_v<LengthType>, "Length type must be integral");
		write_be<LengthType>(static_cast<LengthType>(value.size()));
		write_raw(value.data(), value.size());
	}
} // packet_io

#endif //PACKET_WRITER_HPP
