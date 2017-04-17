//
// Created by 宋辰伟 on 2017/4/17.
//

#ifndef TEST_LOCK_I_EVENT_H
#define TEST_LOCK_I_EVENT_H

#include "I_Action.h"
#include "I_EThread.h"

#define MAX_EVENTS_PER_THREAD 100000

// Events

#define EVENT_NONE CONTINUATION_EVENT_NONE // 0
#define EVENT_IMMEDIATE 1
#define EVENT_INTERVAL 2
#define EVENT_ERROR 3
#define EVENT_CALL 4 // used internally in state machines
#define EVENT_POLL 5 // negative event; activated on poll or epoll

// Event callback return functions

#define EVENT_DONE CONTINUATION_DONE // 0
#define EVENT_CONT CONTINUATION_CONT // 1
#define EVENT_RETURN 5
#define EVENT_RESTART 6
#define EVENT_RESTART_DELAYED 7

// Event numbers block allocation
// ** ALL NEW EVENT TYPES SHOULD BE ALLOCATED FROM BLOCKS LISTED HERE! **

#define VC_EVENT_EVENTS_START 100
#define NET_EVENT_EVENTS_START 200
#define DISK_EVENT_EVENTS_START 300
#define CLUSTER_EVENT_EVENTS_START 400
#define HOSTDB_EVENT_EVENTS_START 500
#define DNS_EVENT_EVENTS_START 600
#define CONFIG_EVENT_EVENTS_START 800
#define LOG_EVENT_EVENTS_START 900
#define REFCOUNT_CACHE_EVENT_EVENTS_START 1000
#define CACHE_EVENT_EVENTS_START 1100
#define CACHE_DIRECTORY_EVENT_EVENTS_START 1200
#define CACHE_DB_EVENT_EVENTS_START 1300
#define HTTP_NET_CONNECTION_EVENT_EVENTS_START 1400
#define HTTP_NET_VCONNECTION_EVENT_EVENTS_START 1500
#define GC_EVENT_EVENTS_START 1600
#define ICP_EVENT_EVENTS_START 1800
#define TRANSFORM_EVENTS_START 2000
#define STAT_PAGES_EVENTS_START 2100
#define HTTP_SESSION_EVENTS_START 2200
#define HTTP2_SESSION_EVENTS_START 2250
#define HTTP_TUNNEL_EVENTS_START 2300
#define HTTP_SCH_UPDATE_EVENTS_START 2400
#define NT_ASYNC_CONNECT_EVENT_EVENTS_START 3000
#define NT_ASYNC_IO_EVENT_EVENTS_START 3100
#define RAFT_EVENT_EVENTS_START 3200
#define SIMPLE_EVENT_EVENTS_START 3300
#define UPDATE_EVENT_EVENTS_START 3500
#define LOG_COLLATION_EVENT_EVENTS_START 3800
#define AIO_EVENT_EVENTS_START 3900
#define BLOCK_CACHE_EVENT_EVENTS_START 4000
#define UTILS_EVENT_EVENTS_START 5000
#define CONGESTION_EVENT_EVENTS_START 5100
#define INK_API_EVENT_EVENTS_START 60000
#define SRV_EVENT_EVENTS_START 62000
#define REMAP_EVENT_EVENTS_START 63000

// define misc events here
#define ONE_WAY_TUNNEL_EVENT_PEER_CLOSE (SIMPLE_EVENT_EVENTS_START + 1)
#define PREFETCH_EVENT_SEND_URL (SIMPLE_EVENT_EVENTS_START + 2)

typedef int EventType;
const int ET_CALL         = 0;
const int MAX_EVENT_TYPES = 8; // conservative, these are dynamically allocated

class EThread;

class Event : public Action
{
public:
    void schedule_imm(int callback_event = EVENT_IMMEDIATE);
    void schedule_at(ink_hrtime atimeout_at, int callback_event = EVENT_INTERVAL);
    void schedule_in(ink_hrtime atimeout_in, int callback_event = EVENT_INTERVAL);
    void schedule_every(ink_hrtime aperiod, int callback_event = EVENT_INTERVAL);
    void free();
    EThread *ethread;

    unsigned int in_the_prot_queue : 1;
    unsigned int in_the_priority_queue : 1;
    unsigned int immediate : 1;
    unsigned int globally_allocated : 1;
    unsigned int in_heap : 4;
    int callback_event;

    ktime timeout_at;
    ktime period;

    // Bound event specific data.
    void *cookie;

    Event();

    Event *
    init(Continuation *c, ink_hrtime atimeout_at = 0, ink_hrtime aperiod = 0)
    {
        continuation = c;
        timeout_at   = atimeout_at;
        period       = aperiod;
        immediate    = !period && !atimeout_at;
        cancelled    = false;
        return this;
    }

private:
//    void *operator new(size_t size); // use the fast allocators
private:
    // prevent unauthorized copies (Not implemented)
    Event(const Event &);
    Event &operator=(const Event &);

public:
    // LINK(Event, link);
    ~Event() {
        mutex = nullptr;
    }
};

inline
Event::Event()
        : ethread(0),
          in_the_prot_queue(false),
          in_the_priority_queue(false),
          immediate(false),
          globally_allocated(true),
          in_heap(false),
          timeout_at(0),
          period(0),
          cookie(nullptr)
{
}

#endif //TEST_LOCK_I_EVENT_H
