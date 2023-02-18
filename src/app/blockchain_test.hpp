// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.
// SPDX-License-Identifier: MIT

#ifndef _KTHTEST__BLOCKCHAINTEST_H_
#define _KTHTEST__BLOCKCHAINTEST_H_
//====
#include "global_defines.h"
#include "app/block_test.h"
#include "hash_combine.h"

#include <deque>
#include <iostream>
#include <cstring>
#include <thread>

///////////////////////
//https://www.hamrodev.com/en/tutorials/javascript-blockchain
///////////////////////

void start_mining_blockchain();//Predefinition

void blockchain_test()
{
    printf("Starting the blockchain test:\n");
    start_mining_blockchain();
}

class CBlockchain
{
public:
    std::deque<CBlock> chain;
    std::deque<CAddress> accounts;
protected:
    std::vector<uint64_t> shorttxids;//ESP:actuales optimizaciones de tx 
    std::vector<CPrefilledTx> prefilledtxn;//ESP:actuales optimizaciones de tx
private:
    uint64_t _shorttxidk0;//ESP:actuales optimizaciones de tx
    uint64_t _shorttxidk1;//ESP:actuales optimizaciones de tx
public:
    //constructor
    CBlockchain() {}

    bool thread_stop=false;
    void ExecuteBlockchainThreading()
    {
        //Do other things in parallel meanwhile is executing this...
        std::thread t1(&CBlockchain::ExecuteBlockchain, this);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        WriteCommands();
        //...Until the function ends
        thread_stop=true;
        t1.join();
    }

    void ExecuteBlockchain()
    {
        while(!thread_stop/*chain.size()!=5*/)
        {
            //Add genesis block or a normal block
            AddNewBlock("1");
            if(chain.size()==1) printf(">>>>Genesis block added!: %s", chain.at(0).ToString().c_str());
            else printf(">>>>Block #%zu added!: %s", chain.size(), chain.front().ToString().c_str());
            //printf(">>>>Block # added!\n");
            std::this_thread::sleep_for(std::chrono::seconds(10));//ESP:10 segundos de espera
        }

        printf("\nNo more blocks");
    }

    void CommandsInstructions()
    {
        printf("Comands list to execute:\n");
        printf("------------------------\n");
        printf("exit=Stop executing and exit.\n");
        printf("validate=Validate the chain.\n");
        printf("register(N)=Register a new account number.\n");
        printf("addtransaction(N1,N2,Ncoins)=[TODO]Add new transaction from account, to account, amount.\n");
        printf("------------------------\n");
    }

    void WriteCommands()
    {
        std::string line;
        std::string delimiter = "(";
        do
        {
            printf("----------------------------------\n");
            printf("Please write a command to execute:\n");
            std::cin >> line;
            //std::cin.ignore();
            //char text[81]; int nc = scanf("%80[^\n]%*[^\n]", text); getchar();
            //char text[81]; fgets(text, sizeof(text), stdin);
            std::cout << line << ' ';
            std::string command = line.substr(0, line.find(delimiter));
            if(command=="exit")
            {
                printf("command executed.\n");
                break;
            }
            else if(command=="validate")
            {
                Valid_chain(chain);
                printf(" command executed.\n");
            }
            else if(command=="register")
            {
                size_t pos = 0;
                if((pos = line.find(delimiter)) != std::string::npos)
                {
                    line.erase(0, pos + delimiter.length());
                    if(line.size() >=1 && line.at(line.size()-1) == ')')
                    {
                        Register_account(line.substr(0, line.size()-1));
                        printf(" command executed.\n");
                    }
                    else
                        printf(" have a syntax error in command!\n");
                }
                else
                    printf(" have a syntax error in command!\n");
            }
            else if (command=="addtransaction")
            {
                //TODO:Add transaction to the current block
                printf("command NOT IMPLEMENTED YET.\n");
            }
            
            else
                printf("command not found!\n");
        }
        while(true);
    }

    bool CheckAccount(int pos)
    {
        printf("Account \"%s\" with %zu amount\n", accounts.at(pos).public_key, accounts.at(pos).amount);
        return true;
    }

    bool Register_account(std::string public_key)
    {
        if(public_key == "") return false;
        if(public_key == "0") return false;//burn address
        //ESP:TODO:comprobar que la dirección no está repetida
        CAddress a;
        strcpy(a.public_key, public_key.c_str());
        a.amount = 0;
        accounts.push_front(a);
        printf("Account registered \"%s\"", public_key.c_str());
        return true;
    }

    void FullfillBlockHeaderAndShortTxIDs(CBlock& block)
    {
        srand((unsigned) time(NULL)); uint32_t rand_int32 = (uint32_t) rand();
        block.nNonce = rand_int32; //GetRandInt(std::numeric_limits<uint32_t>::max());
        shorttxids.resize(block.vtx.size());
        prefilledtxn.resize(1);//ESP:Reseteamos las nuevas transacciones a sólo la primera
        FillShortTxID(block);
        // TODO: Use our mempool prior to block acceptance to predictively fill more
        // than just the coinbase.
        //ESP:Coinbase es la transacción obligatoria que tiene cada bloque y recompensa al validador
        prefilledtxn[0] = {0, block.vtx[0]};
        for (size_t i = 1; i < block.vtx.size(); i++)
        {
            const CTransaction &tx = block.vtx[i];
            shorttxids[i - 1] = GetShortID(tx.GetHash());
        }
    }

    void FillShortTxID(const CBlock &block)
    {
        uint256_t shorttxidhash;
        hash_combine64(shorttxidhash, block.GetHash());
        _shorttxidk0 = shorttxidhash & 0x00000000ffffffffL;
        _shorttxidk1 = shorttxidhash & 0xffffffff00000000L;
    }

    uint64_t GetShortID(const uint256_t &txhash) const
    {
        //shorttxids calculation assumes 6-byte shorttxids (but are stored in a 8-byte int type)
        uint256_t result = 0;
        hash_combine64(result, txhash);
        hash_combine64(result, _shorttxidk0);
        hash_combine64(result, _shorttxidk1);
        return result & 0xffffffffffffL;//6-byte truncate
    }

    uint256_t GetMerkleHash(CBlock& block)
    {
        //ESP:Simplificamos asumiendo que hay solo 1 transacción
        const CTransaction &tx = block.vtx[0];
        return tx.GetHash();
    }

    bool Hash_proof_of_work(CBlock& block)
    {
        printf(">>>>Creating PoW Hash: ");
        while(!Valid_hash_proof(block))
        {
            block.nNonce+=1;
        }
        std::cout << "PoW Hash number found: " << block.GetHash() << '\n';
        return true;
    }

    bool Valid_hash_proof(CBlock& block)
    {
        //hash starts with 7 zeroes '0000000'
        if((block.GetHash() & 0xFFFFFFF000000000UL) != 0) return false;
        return true;
    }

    bool Valid_chain(std::deque<CBlock> chain_to_check)
    {
        uint index = 1;
        if(chain_to_check.size()==0) return true;
        CBlock& last_block = chain_to_check[0];
        while(index < chain_to_check.size())
        {
            CBlock& block = chain_to_check[index];
            //ESP:Comprobamos el hash del bloque previo
            if(block.hashPrevBlock == last_block.GetHash()) {printf("chain not valid"); return false;}
            //ESP:Comprobamos que el hash cumple unas reglas para simular un pequeño trabajo (PoW)
            if(!Valid_hash_proof(block)) {printf("chain not valid"); return false;}
            //
            last_block = block;
            index += 1;
        }
        printf("chain valid");
        return true;
    }

    void AddNewBlock(std::string miner_address)
    {
        //Important: When block creation do not chage the data toa void creating inconsistent hashes 
        uint256_t hashPrevBlock = 0;
        if (chain.size() == 0)
        {
            chain.push_front(CreateBlock(1597811185, 114152193, 0x1d00ffff, 1, 50 /* * COIN*/, hashPrevBlock, miner_address, "Block genesis"));
        }
        else
        {
            time_t now = time(0);// current date/time based on current system
            uint32_t nNonce = 0;
            uint previous_index = chain.size() - 1;
            hashPrevBlock = chain.front().GetHash();
            chain.push_front(CreateBlock(now, nNonce, 80, 1, 50 /* * COIN*/, hashPrevBlock, miner_address, ""));
        }
        //Erase the current list of transactions to start fullfill the next block transactions
        prefilledtxn.clear();// current Txs 
    }

    CBlock CreateBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion,
        /*const Amount*/ uint64_t blockReward, uint256_t& hashPrevBlock, std::string public_key, const char* psz_str)
    {
        //ESP: Una CTransaction está finalizada si es "Mutable" está pendiente de ser finalizada
        CMutableTransaction txNew;
        txNew.nVersion = 1;
        txNew.vinput.resize(1);
        txNew.voutput.resize(1);
        // txNew.vin[0].scriptSig = 0;
        //     CScript() << ScriptInt::fromIntUnchecked(486604799)
        //             << CScriptNum::fromIntUnchecked(4)
        //            << std::vector<uint8_t>((const uint8_t *)psz_str,
        //                                    (const uint8_t *)psz_str + strlen(psz_str));
        txNew.voutput[0].nValue = blockReward;
        // txNew.vout[0].scriptPubKey = CScript();//ESP:en vez de un script lo dejamos vacio

        CBlock blockNew;
        blockNew.nVersion = nVersion;
        blockNew.nTime = nTime;
        blockNew.nBits = nBits;
        blockNew.nNonce = nNonce;
        // blockNew.vtx.push_back(MakeTransactionRef(std::move(txNew)));
        blockNew.hashPrevBlock = hashPrevBlock;
        blockNew.hashMerkleRoot = GetMerkleHash(blockNew);
        Hash_proof_of_work(blockNew);

        return blockNew;
    }
};

//Simplified test blockchain
void start_mining_blockchain()
{
    std::string init_banner = "";
    init_banner += "========================================\n";
    init_banner += "====Launching test-simply-blockchain====\n";
    init_banner += "========================================\n";
    std::cout << init_banner;

    CBlockchain b;
    b.Register_account("1");//"0" is burn address
    printf("\n");
    b.CommandsInstructions();
    b.ExecuteBlockchainThreading();
    //b.ExecuteBlockchain();
    //b.WriteCommands();
}

//====
#endif /*_KTHTEST__BLOCKCHAINTEST_H_*/