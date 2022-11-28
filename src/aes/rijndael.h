/// RJINDAEL.H
///
/// @brief Computes the Rjindael AES S-Boxes at compile-time.
/// @author Mae B. Morella <https://github.com/morellam-dev>

#ifndef AES_RJINDAEL_H_
#define AES_RJINDAEL_H_

#include <array>
#include <cstdint>
#include <bit> // for std::rotl

#include "galois.h"
#include "aes.h" // for bytes_t


namespace rjindael {

using aes::byte;

/// @brief Generates the rjindael S-box.
/// @return {sbox, sbox_inverse}
consteval std::pair<std::array<byte, 256>, std::array<byte, 256>> generate_sbox() {
  std::array<byte, 256> sbox, sbox_inverse;
  uint8_t p = 1, q = 1;
  // loop invariant: p * q == 1 in the Galois field
  do {
    p = galois::x3[p];             // multiply p by 3
    q = galois::gn_mult(q, 0xf6);  // divide q by 3
    // Affine transformation
    uint8_t affine =
        q ^ std::rotl(q, 1) ^ std::rotl(q, 2) ^ std::rotl(q, 3) ^ std::rotl(q, 4) ^ 0x63;
    sbox[p] = affine;
    sbox_inverse[affine] = p;
  } while (p != 1);

  /* 0 is a special case since it has no inverse */
  sbox[0] = 0x63;
  sbox_inverse[0x63] = 0;
  return {sbox, sbox_inverse};
}

constexpr auto SBOX = generate_sbox().first;
constexpr auto SBOX_INVERSE = generate_sbox().second;

}  // namespace rjindael

#endif