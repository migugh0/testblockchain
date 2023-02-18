// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2019 The Bitcoin developers
// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.
// SPDX-License-Identifier: MIT

#include "block_test.h"

#include "hash_combine.h"

#include <string>
#include <sstream>
#include <functional>
#include <iomanip>


uint256_t CTransaction::GetHash() const
{
    // char byte[sizeof(CTx) + 1];
    // char* p_byte=byte;
    // p_byte=(char*)this;
    // p_byte[sizeof(CTx)] = 0;
    // std::string str = byte;
    // std::size_t hash = std::hash<std::string>{}(str);
    // return hash;//SerializeHash(*this));
    uint32_t result = 0;
    hash_combine32(result, this);//ESP:Creamos hashes más fáciles de 'calcular' los ceros inciales
    return (uint256_t) result;
}

uint256_t CBlockHeader::GetHash() const
{
    // char byte[BLOCK_HEADER_SIZE + 1];
    // char* p_byte=byte;
    // p_byte=(char*)this;
    // p_byte[BLOCK_HEADER_SIZE] = 0;
    // std::vector v = byte;
    // std::size_t hash = std::hash<std::vector>{}(v);
    // return hash;//SerializeHash(*this));
    uint32_t result = 0;
    hash_combine32(result, this);//ESP:Creamos hashes más fáciles de 'calcular' los ceros inciales
    return (uint256_t) result;
}

//print the block to string
std::string CBlock::ToString()
{
    std::stringstream s;
    s << "CBlock(hash=" << "0x" << std::setfill('0') << std::setw(16) << std::uppercase << std::hex << GetHash() << //%020d //18446744073709551615 //0xFFFFFFFFFFFFFFFF
                    ", ver=" << std::dec << nVersion <<
                    ", hashPrevBlock=" << "0x" << std::setfill('0') << std::setw(16) << std::uppercase << std::hex << hashPrevBlock <<
                    ", hashMerkleRoot=" << "0x" << std::setfill('0') << std::setw(16) << std::uppercase << std::hex << hashMerkleRoot <<
                    ", nTime=" << std::dec << nTime <<
                    ", nBits=" << std::dec << nBits <<
                    ", nNonce=" << std::dec << nNonce <<
                    ")\n";
    // s << strprintf("CBlock(hash=%i, ver=0x%08x, hashPrevBlock=%s, "
    //                "hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, "
    //                ")\n",
    //                GetHash(), nVersion, hashPrevBlock,
    //                hashMerkleRoot, nTime, nBits, nNonce);
    return s.str();
    //return "CBlock(test)\n";
}
