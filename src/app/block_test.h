// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2022 The Bitcoin developers
// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.
// SPDX-License-Identifier: MIT

#pragma once
#include "global_defines.h"

#include "utils/prevector.h"

#include <cstdint>
#include <string>
#include <vector>

/**
 * Nodes collect new transactions into a block, hash them into a hash tree, and
 * scan through nonce values to make the block's hash satisfy proof-of-work
 * requirements. When they solve the proof-of-work, they broadcast the block to
 * everyone and the block is added to the block chain. The first transaction in
 * the block is a special one that creates a new coin owned by the creator of
 * the block.
 */

///////////////////////
//https://www.novatica.es/criptografia-aplicada-calculo-del-hash-sha-256-de-un-bloque-bitcoin/
///////////////////////

enum class EMagicID : uint32_t
{
    None        = 0x0,
    Mainnet     = 0xf9beb4d9,
    Testnet     = 0xfabfb5da,
    Testnet3	= 0x0b110907,
    Namecoin	= 0xf9beb4fe,
    Regtest     = 0xfabfb5da,
};

///////////////////////
//https://stackoverflow.com/questions/14883896/why-is-stduint32-t-different-from-uint32-t
///////////////////////

// struct uint256_test {uint8_t integer[32];}; //ESP:Simulamos un uint256
typedef std::size_t uint256_t;

// Max size of an address
inline constexpr size_t ADDRESS_BUFFER_SIZE = 20u;

//Public address
class CAddress
{
public:
    char public_key[ADDRESS_BUFFER_SIZE + 1];
    uint64_t amount;//Calculated
};

class CTxInput
{
public:
    uint256_t prev_output_txid;//We save here the previous output of the transaction as input
    prevector<28, uint8_t> scriptSig;
    uint32_t nSequence = 0xffffffff;
};

//Maximum data stored in a transaction output
class COutputData
{
public:
    uint256_t tokenid;
    uint8_t bitfield = 0;
    uint64_t tokenamount;
    prevector<40, uint8_t> data_commitment;
};

//Check spent transaction output (STXO) and unspent transaction output (UTXO)
class CTxOutput
{
public:
    uint64_t nValue;//Amount
    prevector<28, uint8_t> scriptPubKey;
    COutputData tokenData; //Fungible token/NFT or tag NFTs with data
};

//Past transactions on blocks. A transaction can contain multiple inputs and outputs
class CTransaction
{
    static constexpr int32_t CURRENT_VERSION = 2;// Default transaction version
public:
    uint256_t txid;//The Tx id is currently identical to the transaction hash
    uint256_t txhash;
    uint64_t txshortid;
    std::vector<CTxInput> vinput;
    std::vector<CTxOutput> voutput;
    int32_t nVersion = CURRENT_VERSION;
    uint32_t nLockTime;

    //constructor
    CTransaction() noexcept = default;
    bool IsCoinBase() const { return (vinput.size() == 1 && vinput[0].prev_output_txid == 0); }
    uint256_t GetHash() const; //get and calculate the transaction hash
};

//A mutable transaction is a transaction on memory (of the last block or not broadcasted yet)
class CMutableTransaction : public CTransaction
{
};

// Dumb serialization/storage-helper for CBlockHeaderAndShortTxIDs and PartiallyDownloadedBlock
class CPrefilledTx
{
public:
    // Used as an offset since last prefilled tx in CBlockHeaderAndShortTxIDs,
    // as a proper transaction-in-block-index in PartiallyDownloadedBlock
    uint32_t index;
    CTransaction tx;
};

class CBlockHeader
{
public:
    //header
    //------
    //EMagicID magicID;//(obsolete)
    //uint32_t size;//(obsolete)
    int32_t nVersion{};//versionNumber//(4bytes)
    uint256_t hashPrevBlock{};//(32bytes)
    uint256_t hashMerkleRoot{};//(32bytes)//The hash of all the hashes of all the transactions
    uint32_t nTime{};//timestamp//(4bytes)
    uint32_t nBits{};//targetDifficulty//(4bytes)
    uint32_t nNonce{};//(4bytes)
    //uint32_t transactionCount//(obsolete)

    //constructor
    CBlockHeader() noexcept = default;

    void SetNull() { *this = CBlockHeader{}; } //inline
    bool IsNull() const { return (nBits == 0u); } //inline
    uint256_t GetHash() const; //get and calculate the block hash
};

/// Block headers are 80 bytes. Some code depends on this constant (see: validation.cpp).
inline constexpr size_t BLOCK_HEADER_SIZE = 80u;

class CBlock : public CBlockHeader
{
public:
    std::vector<CTransaction> vtx;//vector of tx for transactions on network and disk
    bool fChecked = false;//flag for memory only
    uint32_t extraNonce = 0;//number to add to nTime if you use all nNonce without reach difficulty
    //constructor
    CBlock() {}
    CBlock(const CBlockHeader &header) : CBlockHeader(header) {}

    //basic class functions
    void SetNull() //inline
    {
        CBlockHeader::SetNull();
        fChecked = false;
    }
    CBlockHeader GetBlockHeader() const { return *this; } //inline
    std::string ToString() ; //{ return "CBlock()\n"; }

    //other class functions
};