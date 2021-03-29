#include "create_block.hpp"
#include "merkle_tree.hpp"
#include "poco.hpp"
#include "p2p.hpp"

using namespace Crowd;

CreateBlock::CreateBlock(std::vector<nlohmann::json> &message_j_vec)
{
    message_j_vec_ = message_j_vec;

    merkle_tree mt;

    nlohmann::json m_j, entry_tx_j, entry_transactions_j, exit_tx_j, exit_transactions_j;
    nlohmann::json to_block_j;
    std::string fh_s;

    for (int i = 0; i < message_j_vec_.size(); i++)
    {
        m_j = message_j_vec_[i];

        std::string full_hash_req = m_j["full_hash_req"];

        to_block_j["full_hash"] = full_hash_req;
        to_block_j["ecdsa_pub_key"] = m_j["ecdsa_pub_key"];
        to_block_j["rsa_pub_key"] = m_j["rsa_pub_key"];
        s_shptr_->push(to_block_j.dump());

        entry_tx_j["full_hash"] = to_block_j["full_hash"];
        entry_tx_j["ecdsa_pub_key"] = to_block_j["ecdsa_pub_key"];
        entry_tx_j["rsa_pub_key"] = to_block_j["rsa_pub_key"];
        entry_transactions_j.push_back(entry_tx_j);
        exit_tx_j["full_hash"] = "";
        exit_transactions_j.push_back(exit_tx_j);
    }

    s_shptr_ = mt.calculate_root_hash(s_shptr_);
    std::string datetime = mt.time_now();
    std::string root_hash_data = s_shptr_->top();
    block_j_ = mt.create_block(datetime, root_hash_data, entry_transactions_j, exit_transactions_j);
    Protocol proto;
    std::string my_latest_block_nr = proto.get_last_block_nr();

    // send hash of this block with the block contents to the co's, forget save_block_to_file
    // is the merkle tree sorted, then find the last blocks that are gathered for all the co's

    // send intro_block to co's
    Poco poco;
    poco.inform_chosen_ones(my_latest_block_nr, block_j_);

    //std::string block_s = mt.save_block_to_file(block_j, my_latest_block_nr);
std::cout << "--------5: " << std::endl;
    std::string block_s = block_j_.dump();
    set_hash_of_new_block(block_s);
}

std::vector<std::string> CreateBlock::update_new_peers_with_full_hash()
{
    // TODO intro_peer is doesn't know its full_hash until new_block is send
    // but the other users don't know the whereabouts of intro_peer
    // tcp.client(full_hash) --> client should save full_hash

    nlohmann::json m_j;
    std::vector<std::string> your_full_hash;

    Tcp* tcp = new Tcp;
    for (int i = 0; i < message_j_vec_.size(); i++)
    {
        m_j = message_j_vec_[i];
        nlohmann::json msg_j;
        msg_j["req"] = "your_full_hash";
        msg_j["full_hash"] = m_j["full_hash_req"];
        msg_j["block"] = block_j_;
        msg_j["hash_of_block"] = get_hash_of_new_block();

        std::string msg_s = msg_j.dump();
        your_full_hash.push_back(msg_s);
    }
    delete tcp;

    return your_full_hash;
}

std::string CreateBlock::get_hash_of_new_block()
{
    return hash_of_block_;
}

void CreateBlock::set_hash_of_new_block(std::string block)
{
    Crypto crypto;
    hash_of_block_ = crypto.bech32_encode(block);
}