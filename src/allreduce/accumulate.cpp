/*
Copyright (c) by respective owners including Yahoo!, Microsoft, and
individual contributors. All rights reserved.  Released under a BSD (revised)
license as described in the file LICENSE.
 */
/*
This implements the allreduce function of MPI.  Code primarily by
Alekh Agarwal and John Langford, with help Olivier Chapelle.
*/

#include <iostream>
#include <sys/timeb.h>
#include <cmath>
#include <stdint.h>
#include "accumulate.h"

namespace Hadoop {

using namespace std;

double accumulate_scalar(double local_sum) {
  Cluster& cluster = Cluster::getInstance();
  double temp = local_sum;
  all_reduce(&temp, 1, cluster.master_location, cluster.unique_id, cluster.total, cluster.node, cluster.socks);
  return temp;
}

void accumulate_sum(double* array, size_t length) {
  Cluster& cluster = Cluster::getInstance();
  all_reduce(array, length, cluster.master_location, cluster.unique_id, cluster.total, cluster.node, cluster.socks);
}

void accumulate_avg(double* array, size_t length) {
  Cluster& cluster = Cluster::getInstance();
  accumulate_sum(array, length);
  double total = cluster.total;
  for (int i = 0; i < length; ++i)
  {
    if(!isfinite(array[i]))
        std::cerr<<"bug"<<std::endl;
    array[i] /= total;
  }
}

} // namespace
