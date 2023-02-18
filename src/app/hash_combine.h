// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.
// SPDX-License-Identifier: MIT

#include <functional>

template <class T>
inline void hash_combine32(std::uint32_t& seed, const T& p_value)
{
    size_t h = std::hash<T>()(p_value);
    const uint32_t irrational_number = 0x727C9716;//(2^32)/sqrt(5)
    seed = seed ^ (h + irrational_number + (seed << 6) + (seed >> 2));
}

template <class T>
inline void hash_combine64(std::uint64_t& seed, const T& p_value)
{
    size_t h = std::hash<T>()(p_value);
    const uint64_t irrational_number = 0x727C9716FFB764D5;//(2^64)/sqrt(5)
    seed = seed ^ (h + irrational_number + (seed << 12) + (seed >> 4));
}