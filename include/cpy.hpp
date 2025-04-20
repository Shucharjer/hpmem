#pragma once
#include <cstddef>
#include <cstdint>
#include <immintrin.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-pro-type-cstyle-cast)
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)

#define PREV_ALIGNED(addr, align) addr & ~(align - 1)
#define NEXT_ALIGNED(addr, align) (addr + align - 1) & ~(align - 1)

namespace hpmem {

/**
 * @brief Copy in 128 bits.
 */
static inline void copy_in_128b(uint8_t* dst, const uint8_t* src, uint8_t length) noexcept {
    if (length <= 0x4) {
        switch (length) {
        case 4:
            *(dst + 3) = *(src + 3);
        case 0x3:
            *(dst + 2) = *(src + 2);
        case 0x2:
            *(dst + 1) = *(src + 1);
        case 0x1:
            *dst = *src;
        default:
            break;
        }
        return;
    }
    else if (length & 0x8) {
        *(uint64_t*)dst                  = *(uint64_t*)src;
        *(uint64_t*)(dst + length - 0x8) = *(uint64_t*)(src + length - 0x8);
        return;
    }
    else {
        *(uint32_t*)dst                  = *(uint32_t*)src;
        *(uint32_t*)(dst + length - 0x4) = *(uint32_t*)(src + length - 0x4);
        return;
    }
}

/**
 * @brief Copy
 * Extremely fast when coping small memory blocks.
 *
 */
inline void hpcpy(void* dst, const void* src, ptrdiff_t length) noexcept {
    auto* dst_8_1 = (uint8_t*)dst;
    auto* src_8_1 = (const uint8_t*)src;
    if (length < 0x10) { // 128 bits, 16 bytes
        copy_in_128b(dst_8_1, src_8_1, length);
        return;
    }
    else if (length <= 0x20) { // 256 bits, 32 bytes
        _mm_storeu_si128((__m128i*)dst, _mm_loadu_si128((const __m128i*)src));
        _mm_storeu_si128(
            (__m128i*)(dst_8_1 + length - 0x10),
            _mm_loadu_si128((const __m128i*)(src_8_1 + length - 0x10)));
        return;
    }
    else {
// NOTE: avx512 would casue low frequency in some platforms.
#if !defined(__SCE__) || defined(__AVX__)
        __m256i* dst256 = (__m256i*)dst;
        __m256i* src256 = (__m256i*)src;
        // remove the head
        _mm256_storeu_si256(dst256, _mm256_loadu_si256(src256));
        // remove the tail
        _mm256_storeu_si256(
            (__m256i*)(dst_8_1 + length - 0x20),
            _mm256_loadu_si256((const __m256i*)(src_8_1 + length - 0x20)));
        if (length > 0x40) {      // 64 bytes
            if (length <= 0x80) { // 128 bytes
                switch (length >> 5) {
                case 4:
                    _mm256_storeu_si256(dst256 + 3, _mm256_loadu_si256(src256 + 3));
                case 3:
                    _mm256_storeu_si256(dst256 + 2, _mm256_loadu_si256(src256 + 2));
                default:
                    _mm256_storeu_si256(dst256 + 1, _mm256_loadu_si256(src256 + 1));
                    break;
                }
                return;
            }
            else {
                // copy aligned

                constexpr uintptr_t mask = 0xffffffffffffffe0ull;

                auto* aligned_dst_beg       = (__m256i*)(((uintptr_t)dst + 0x1f) & mask);
                auto* aligned_src_beg       = (const __m256i*)(((uintptr_t)src + 0x1f) & mask);
                auto* const aligned_dst_end = (__m256i*)((((uintptr_t)dst) + length - 0x1f) & mask);
                auto* const aligned_src_end = (__m256i*)((((uintptr_t)src) + length - 0x1f) & mask);

                const ptrdiff_t n = aligned_dst_end - aligned_dst_beg;

                ptrdiff_t v = n / 3;
                v           = (v << 1) + v;
                for (ptrdiff_t i = 0; i < v; i += 3) {
                    __m256i vec0 = _mm256_load_si256(aligned_src_beg + i);
                    __m256i vec1 = _mm256_load_si256(aligned_src_beg + i + 1);
                    __m256i vec2 = _mm256_load_si256(aligned_src_beg + i + 2);
                    _mm256_store_si256(aligned_dst_beg + i, vec0);
                    _mm256_store_si256(aligned_dst_beg + i + 1, vec1);
                    _mm256_store_si256(aligned_dst_beg + i + 2, vec2);
                }

                _mm256_store_si256(aligned_dst_end - 2, _mm256_load_si256(aligned_src_end - 2));
                _mm256_store_si256(aligned_dst_end - 1, _mm256_load_si256(aligned_src_end - 1));
                _mm256_store_si256(aligned_dst_end, _mm256_load_si256(aligned_src_end));

                return;
            }
        }
#elif !defined(__SCE__) || defined(__SSE__)
// TODO: SSE
#else

#endif
    }
}

} // namespace hpmem

// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
// NOLINTEND(cppcoreguidelines-pro-type-cstyle-cast)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
