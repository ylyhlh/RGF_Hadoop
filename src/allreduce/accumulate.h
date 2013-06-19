/*
Copyright (c) by respective owners including Yahoo!, Microsoft, and
individual contributors. All rights reserved.  Released under a BSD
license as described in the file LICENSE.
 */
//This implements various accumulate functions building on top of allreduce.
#ifndef ACCUMULATE_H
#define ACCUMULATE_H

#include "allreduce.h"
#include "cluster.h"

// Usage:
//   Before using accumulate, we need to initial the environment of cluster by
//   specifying
//
//   Hadoop::Cluster::init_cluster(
//     size_t node_id, // node id, from 0
//     size_t total, // number of all machines involved in the cluster.
//     size_t unique_id, // the unique id of the job.
//     std::string master_location); // where is the spanning tree master.
//
//   This step is done only once, see cluster.h(.cpp)
//   After that, we can use accumulate anywhere in the program.
//
//   double* array; // Pointer to an array.
//   int length;
//   Hadoop::accumulate_sum(array, length);
//
//   Now array is the sum over all machines and all machines view the same result.
//
namespace Hadoop {

float accumulate_scalar(float local_sum);
void accumulate_sum(float* array, size_t length);
void accumulate_avg(float* array, size_t length);
}

#endif
