﻿#ifndef FRAMED_PACKET_READER_HPP
#define FRAMED_PACKET_READER_HPP

#if defined(_MSC_VER)
    #define PACKETIO_API __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
    #define PACKETIO_API __attribute__((visibility("default")))
#else
    #define PACKETIO_API
#endif

#include <cstddef>
#include <functional>
#include <memory>
#include <span>
#include <vector>

#include "packet_buffers.hpp"

namespace net
{
    enum class packet_parse_status
    {
        complete,
        partial,
        error,
    };

    template <typename T, typename E>
    struct PACKETIO_API deserialization_result
    {
        packet_parse_status status;
        std::optional<T> data;
        std::optional<E> error;
    };

    template <typename PacketType, typename Error>
    class PACKETIO_API framed_packet_reader
    {
    public:
        using parse_result_t = deserialization_result<PacketType, Error>;
        using parser_fn_t = std::function<parse_result_t(packet_reader&)>;

        explicit framed_packet_reader(parser_fn_t parser,
                                      const size_t initial_capacity = 1024,
                                      const size_t shrink_threshold = 16384U) :
            shrink_threshold_(shrink_threshold), parser_(std::move(parser))
        {
            buffer_.reserve(initial_capacity);
        }

        void append(const std::byte* data, const size_t length)
        {
            buffer_.insert(std::end(buffer_), data, data + length);
        }

        [[nodiscard]] size_t available_bytes() const noexcept
        {
            return buffer_.size();
        }

        [[nodiscard]] std::span<const std::byte> peek() const noexcept
        {
            return {buffer_};
        }

        void reserve(const size_t capacity) { buffer_.reserve(capacity); }

        void clear() noexcept { buffer_.clear(); }

        parse_result_t try_read_packet()
        {
            if (buffer_.empty())
                return {.status = packet_parse_status::partial};

            packet_reader view(buffer_);

            auto result = parser_(view);

            if (result.status == packet_parse_status::complete)
            {
                if (const size_t consumed = view.bytes_read(); consumed > 0)
                {
                    if (consumed == buffer_.size())
                        buffer_.clear();
                    else
                        buffer_.erase(buffer_.begin(),
                                      buffer_.begin() +
                                      static_cast<ptrdiff_t>(consumed));

                    if (buffer_.capacity() > shrink_threshold_ &&
                        buffer_.size() < (buffer_.capacity() >> 2))
                        buffer_.shrink_to_fit();
                }
            }

            return result;
        }

        void set_shrink_threshold(const size_t threshold)
        {
            shrink_threshold_ = threshold;
        }

    private:
        std::vector<std::byte> buffer_;
        size_t shrink_threshold_ = 16384U;
        parser_fn_t parser_;
    };
} // namespace net

#endif // FRAMED_PACKET_READER_HPP
