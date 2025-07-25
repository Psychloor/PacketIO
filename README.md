# PacketIO

A modern C++23 header-only library for efficient packet handling and binary serialization. PacketIO provides robust utilities for reading and writing binary data with built-in endianness support.

## Features

- Header-only library — include and use
- Modern C++23 implementation
- Endian-aware reading and writing (LE/BE)
- Zero-copy optimizations where possible
- Strong type safety with compile-time checks
- Efficient binary buffer management
- Support for framed packet processing
- String handling utilities (fixed, variable, null-terminated)

## Requirements

- C++23 compatible compiler
- CMake 3.31 or higher

## Installation

### Using CMake FetchContent
```cmake
include(FetchContent)

FetchContent_Declare(
        PacketIO
        GIT_REPOSITORY https://github.com/Psychloor/PacketIO.git
        GIT_TAG main
)
FetchContent_MakeAvailable(PacketIO)

target_link_libraries(your_target PRIVATE PacketIO::PacketIO)
```
### Manual Installation

1. Clone the repository:
```shell
git clone https://github.com/Psychloor/PacketIO.git
```
2. Add to your CMake project:
```cmake
add_subdirectory(PacketIO)
target_link_libraries(your_target PRIVATE PacketIO::PacketIO)
```
## Quick Start
```cpp
#include <packet_buffers.hpp>
#include <framed_packet_reader.hpp>

// Writing packets
net::packet_writer writer;
writer.write_le<uint32_t>(42);           // Write little-endian integer
writer.write_be<uint16_t>(1234);         // Write big-endian integer

// Reading packets
net::packet_reader reader(writer.span());
auto value1 = reader.read_le<uint32_t>(); // Read little-endian integer
auto value2 = reader.read_be<uint16_t>(); // Read big-endian integer

// Framed packet handling
net::framed_packet_reader<MyPacket, MyError> packet_reader(
    [](net::packet_reader& reader) {
        // Your packet parsing logic here
        return parse_result;
    }
);
```
## Core Components

### PacketWriter

- Append-only binary buffer for outgoing packets
- Endian-aware writing methods
- Automatic buffer management
- Support for shared buffer ownership

### PacketReader

- Non-owning view over incoming data
- Tracking read position
- Error handling using `std::expected`
- Support for various data types and string formats

### FramedPacketReader

- Support for handling framed protocol messages
- Automatic buffer management
- Partial packet handling
- Custom error handling

## License

MIT License

Copyright (c) 2025 Psychloor (Kevin Blomqvist)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.