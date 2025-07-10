//
// Created by blomq on 2025-07-10.
//

#ifndef DESERIALIZER_HPP
#define DESERIALIZER_HPP

#include <optional>
#include <type_traits>

namespace packet_io
{
	class packet_reader;
}

namespace packet_io
{
	enum class deserialize_result
	{
		success,
		invalid_data,
		insufficient_data
	};

	template <typename T>
	struct deserialize_state
	{
		deserialize_result result;
		std::optional<T> value; // Only valid if result == success
	};

	template <typename T>
	inline deserialize_state<T> deserialize_success(T&& value)
	{
		return {deserialize_result::success, std::forward<T>(value)};
	}

	template <typename T>
	concept deserializable = requires(packet_reader& r)
	{
		{ deserialize(r, std::type_identity<T>{}) } -> std::same_as<deserialize_state<T>>;
	};

	template <deserializable T>
	deserialize_state<T> read(packet_reader& r)
	{
		return deserialize(r, std::type_identity<T>{});
	}
}

#endif //DESERIALIZER_HPP
