/* Copyright 2013 Google Inc. All Rights Reserved.

   Distributed under MIT license.
   See file LICENSE for detail or copy at https://opensource.org/licenses/MIT
*/

// Heuristics for deciding about the UTF8-ness of strings.

#include <impl/enc/utf8_util.h>

#include <impl/types.h>

namespace brotli {

namespace {

size_t ParseAsUTF8(int* symbol, const uint8_t* input, size_t size) {
  // ASCII
  if ((input[0] & 0x80) == 0) {
    *symbol = input[0];
    if (*symbol > 0) {
      return 1;
    }
  }
  // 2-byte UTF8
  if (size > 1u &&
      (input[0] & 0xe0) == 0xc0 &&
      (input[1] & 0xc0) == 0x80) {
    *symbol = (((input[0] & 0x1f) << 6) |
               (input[1] & 0x3f));
    if (*symbol > 0x7f) {
      return 2;
    }
  }
  // 3-byte UFT8
  if (size > 2u &&
      (input[0] & 0xf0) == 0xe0 &&
      (input[1] & 0xc0) == 0x80 &&
      (input[2] & 0xc0) == 0x80) {
    *symbol = (((input[0] & 0x0f) << 12) |
               ((input[1] & 0x3f) << 6) |
               (input[2] & 0x3f));
    if (*symbol > 0x7ff) {
      return 3;
    }
  }
  // 4-byte UFT8
  if (size > 3u &&
      (input[0] & 0xf8) == 0xf0 &&
      (input[1] & 0xc0) == 0x80 &&
      (input[2] & 0xc0) == 0x80 &&
      (input[3] & 0xc0) == 0x80) {
    *symbol = (((input[0] & 0x07) << 18) |
               ((input[1] & 0x3f) << 12) |
               ((input[2] & 0x3f) << 6) |
               (input[3] & 0x3f));
    if (*symbol > 0xffff && *symbol <= 0x10ffff) {
      return 4;
    }
  }
  // Not UTF8, emit a special symbol above the UTF8-code space
  *symbol = 0x110000 | input[0];
  return 1;
}

}  // namespace

// Returns true if at least min_fraction of the data is UTF8-encoded.
bool IsMostlyUTF8(const uint8_t* data, const size_t pos, const size_t mask,
                  const size_t length, const double min_fraction) {
  size_t size_utf8 = 0;
  size_t i = 0;
  while (i < length) {
    int symbol;
    size_t bytes_read = ParseAsUTF8(
        &symbol, &data[(pos + i) & mask], length - i);
    i += bytes_read;
    if (symbol < 0x110000) size_utf8 += bytes_read;
  }
  return size_utf8 > min_fraction * static_cast<double>(length);
}

}  // namespace brotli
