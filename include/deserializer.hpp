//
// Created by blomq on 2025-07-10.
//

#ifndef DESERIALIZER_HPP
#define DESERIALIZER_HPP

#include <optional>

namespace packet_io
{
	class packet_reader;
}

namespace packet_io {
	enum class deserialize_result {
		success,            // Deserialization succeeded
		invalid_data,       // Data is invalid (wrong format, validation failed, etc.)
		insufficient_data   // Need more bytes in buffer
	};

	template<typename T>
	struct deserialize_state {
		deserialize_result result;
		std::optional<T> value;        // Only valid if result == success
	};

	template<typename T>
	class deserializer {
	public:
		// Main deserialization function
		// ReSharper disable CppFunctionIsNotImplemented once
		static deserialize_state<T> deserialize(packet_reader& buffer);
	};

}

#endif //DESERIALIZER_HPP
