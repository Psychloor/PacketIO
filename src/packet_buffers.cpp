#include "packet_buffers.hpp"

namespace net {
    void packet_writer::reserve(const size_t bytes)
    { buffer_.reserve(bytes); }

    void packet_writer::write_bytes(std::span<const byte> bytes)
    {
        buffer_.insert(std::end(buffer_), bytes.begin(), bytes.end());
    }

    void packet_writer::emplace_bytes(std::vector<std::byte>&& data)
    {
        buffer_.insert(buffer_.end(),
                       std::make_move_iterator(data.begin()),
                       std::make_move_iterator(data.end()));
    }

    shared_bytes_ptr packet_writer::to_shared() &&
    {
        return make_shared_bytes(std::move(buffer_));
    }

    void packet_writer::append_bytes(const shared_bytes& bytes)
    {
        write_bytes(bytes.view());
    }

    packet_reader::packet_reader(const std::span<const std::byte> data) noexcept: data_(data)
    {
    }

    std::expected<std::string, std::error_code> packet_reader::read_c_string()
    {
        auto rem = remaining();
        const auto null_pos = std::ranges::find(rem, std::byte{0});

        if (null_pos == rem.end())
        {
            // No null terminator in the current buffer
            return std::unexpected(
                std::make_error_code(std::errc::invalid_argument));
        }

        const auto offset = static_cast<size_t>(null_pos - rem.begin());

        std::string str{reinterpret_cast<const char*>(rem.data()), offset};

        consumed_ += offset + 1; // +1 to skip the null terminator
        return std::move(str);
    }
}
