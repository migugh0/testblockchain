// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.
// SPDX-License-Identifier: MIT

#ifndef _KTHTEST__RUN_APP_HPP_
#define _KTHTEST__RUN_APP_HPP_
//====

#include "global_defines.h"

#include "app/blockchain_test.hpp"

namespace kth::node
{
    struct Database
    {
        std::string host_name="test_node1";
    };

    struct Metadata
    {
        std::string info="";
        bool help=false;
        bool version=false;
        int argc;
        const char** args;
    };

    class Class_executor
    {
    public:
        //constructor
        //ESP:Asegúrese de que los miembros aparezcan en la lista de inicializadores en el mismo orden en que aparecen en la clase, eso le da más velocidad
        Class_executor(Metadata _metadata, bool _a) : metadata(_metadata), a(_a) 
        {}
        //variables
        Metadata metadata;
        bool a;
        Database db;
    };

    class Class_parser
    {
    public:
        //variables
        Metadata configured;
    };

    //Run tests
    const std::string version()
    {
        return "testblockchain version=" KTH_NODE_EXE_VERSION;
    }

    void do_help()
    {
        printf("Show help\n");
    }

    bool run(kth::node::Class_executor& host)
    {
        printf("%s\n", version().c_str());
        printf("%s\n", host.db.host_name.c_str());
        printf("__cplusplus=%liL\n",__cplusplus);
        printf("\n");
        blockchain_test();
        printf("\n");
        return true;
    }

    bool menu(std::string application_name, kth::node::Class_parser& metadata, kth::node::Class_executor& host)
    {
        const kth::node::Metadata& config = metadata.configured;

        if (config.help) {
            do_help();
            return true;
        }

        if (config.version) {
            printf(version().c_str());
            return true;
        }

        //If there are no command line arguments, just run the node
        return run(host);
    }
};

#ifndef KTH_USE_MAIN
#define KTH_USE_MAIN
#endif /*KTH_USE_MAIN*/

//====
#endif /*_KTHTEST__RUN_APP_HPP_*/
