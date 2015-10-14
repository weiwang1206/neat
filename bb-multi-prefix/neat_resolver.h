#ifndef NEAT_RESOLVE_H
#define NEAT_RESOLVE_H

#include <uv.h>
#include <ldns/ldns.h>

#include "include/queue.h"

#define DNS_TIMEOUT     60000
#define DNS_BUF_SIZE    1472

//We know these servers will not lie and will accept queries from an network
//address. Until we have defined a syntax for IP/interface information in
//resolv.conf (and the like), then this is as good as we can do
//TODO: Some firewalls like to block these, implement a platform-independent way of reading from resolv.conf etc.
static char* const INET_DNS_SERVERS [] = {"8.8.8.8", "8.8.4.4", "208.67.222.222", "208.67.220.220"};
static char* const INET6_DNS_SERVERS [] = {"2001:4860:4860::8888", "2001:4860:4860::8844", "2620:0:ccc::2", "2620:0:ccd::2"};

struct neat_addr;
struct neat_resolver;

//Represent one source/dst address used for DNS lookups. We could save space by
//recycling handle, but this structure will make it easier to support
//fragmentation of DNS requests (way down the line)
struct neat_resolver_src_dst_addr {
    struct neat_resolver *resolver;
    struct neat_addr *src_addr;
    //TODO: Dynamically allocate?
    struct neat_addr dst_addr;

    char dns_rcv_buf[DNS_BUF_SIZE];
    ldns_buffer *dns_snd_buf;
    uv_buf_t dns_uv_snd_buf;
    uv_udp_send_t dns_snd_handle;
    uv_udp_t resolve_handle;

    LIST_ENTRY(neat_resolver_src_dst_addr) next_pair;

    //Keep track of which pairs are closed
    uint8_t closed;
};

#endif
