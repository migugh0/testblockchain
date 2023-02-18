// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.
// SPDX-License-Identifier: MIT

#include "wrappers/ucrttypes.h"

#include "app/run_app.hpp"

KTH_USE_MAIN

static auto const application_name = "kth test";

int main(int argc, char* argv[])
{
    const char** args = const_cast<const char**>(argv);

    kth::node::Class_parser metadata;
    metadata.configured.argc=argc;
    metadata.configured.args=args;
    /*node::parser metadata(kth::domain::config::network::testnet);
    if ( ! metadata.parse(argc, args, cerr)) {
        return EXIT_FAILURE;
    }*/

    // executor host(metadata.configured, cout, cerr);
    kth::node::Class_executor host(metadata.configured, true);
    return menu(application_name, metadata, host) ? EXIT_SUCCESS : EXIT_FAILURE ;
}
