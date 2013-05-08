#ifndef CLUSTER_H
#define CLUSTER_H

#include <string>
#include <iostream>
#include "allreduce.h"

// Singleton, not thread-safe.
namespace Hadoop {

class Cluster
{
public:
    size_t node;
    size_t total;
    size_t unique_id;
    std::string master_location;
    node_socks socks;

    static void init_cluster(size_t node_id, size_t total, size_t unique_id,
        std::string master_location);
    static Cluster& getInstance();

private:
    static Cluster* instance;

    Cluster() {};

    // Non-copyable.
    Cluster(Cluster const&);
    void operator=(Cluster const&);
};
}

#endif
