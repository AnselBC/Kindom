//
// Created by 宋辰伟 on 2017/4/12.
//

#ifndef PROJECT_RAFTSERVER_H
#define PROJECT_RAFTSERVER_H


using ::std::string
using ::std::vector
using ::std::map

enum {
    LEADER_STATE = 0,
    CANDIDAT_STATE,
    FOLLOWER_STATE,
};

class RaftServer : public Raft, public RaftServerInterface
{
public:
    virtual void SetElectionTimeout(ink_hrtime hrtime);
    virtual void Recover(const LogEntry &entry);
    virtual void Start(ink_hrtime now, int64_t random_seed);
    virtual void Tick(ink_hrtime now);
    virtual void Propose(const LogEntry &entry);
    virtual void Run(ink_hrtime now, const Message &message);
    virtual void Snapshot(bool uncommitted, vector<LogEntry> *entries);
    virtual void StartVote();
    virtual void Stop(); // Clean shutdown for faster failover.


    bool i_am_leader() {
        return my_id == leader_id;
    }

    // Only for leader
    uint64_t get_nodes_len() {
        return nodes_len;
    }


    // for leader only
    int get_node_next_index(string named, uint64_t *result)
    {
        int ret;
        MUTEX_TAKE_TRY_LOCK_FOR_SPIN(mutex, this_thread(), nullptr, READ_LOCK_SPIN_COUNT);
        ret = ink_hash_table_lookup(next_index, namd, result);
        MUTEX_UNTAKE_LOCK(mutex, this_thread());

        return ret;
    }

    int get_node_match_index(string named)
    {
        int ret;
        MUTEX_TAKE_TRY_LOCK_FOR_SPIN(mutex, this_thread(), nullptr, READ_LOCK_SPIN_COUNT);
        ret = ink_hash_table_lookup(next_index, namd);
        MUTEX_UNTAKE_LOCK(mutex, this_thread());

        return ret;
    }

    void set_node_next_index(unsigned log key, InkHashTableEntry * entry)
    {
        MUTEX_TAKE_TRY_LOCK_FOR_SPIN(mutex, this_thread(), nullptr, READ_LOCK_SPIN_COUNT);
        ink_hash_table_insert(next_index, entry);
        MUTEX_UNTAKE_LOCK(mutex, this_thread());
    }

    void get_node_match_index(unsigned log key, InkHashTableEntry * entry)
    {
        MUTEX_TAKE_TRY_LOCK_FOR_SPIN(mutex, this_thread(), nullptr, READ_LOCK_SPIN_COUNT);
        ink_hash_table_insert(next_index, entry);
        MUTEX_UNTAKE_LOCK(mutex, this_thread());
    }

private:
    volatile uint64_t commit_index;
    volatile uint64_t last_applied;

    uint64_t current_term;
    uint64_t vote_for_id;
    LogManager *logmg;
    int         current_stat;
    uint64_t    my_id;
    uint64_t    leader_id;
    uint64_t    nodes_len;
    vector<IpAddr> nodes;

    // for Leader only
    InkHashTable next_index;
    InkHashTable match_index;
};

TS_INLNE
RaftServer::RaftServer()
{
    ink_release_assert(next_index = ink_hash_table_create(InkHashTableKeyType_String));
    ink_release_assert(match_index = ink_hash_table_create(InkHashTableKeyType_String));
    current_stat = FOLLOWER_STATE;

}

#endif //PROJECT_RAFTSERVER_H
