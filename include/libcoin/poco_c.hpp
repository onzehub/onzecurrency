#pragma once

#include <stdio.h>
#include <string.h>

#include "poco.hpp"

#include "transactions.hpp"

using namespace Crowd;

namespace Coin
{
    class PocoC: Poco
    {
    public:
        void create_and_send_block_c();
    private:
        void inform_chosen_ones_c(std::string my_next_block_nr, nlohmann::json block_j, std::string full_hash_req);
    private:
        std::shared_ptr<std::stack<std::string>> s_shptr_ = std::make_shared<std::stack<std::string>>();
        Transactions tx_;
    };
}