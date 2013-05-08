#include <cstdlib>
#include "cluster.h"

namespace Hadoop {

using namespace std;

Cluster* Cluster::instance = NULL;

void Cluster::init_cluster(size_t node_id, size_t total, size_t unique_id,
    std::string master_location) {
    if (instance != NULL) {
        cerr << "Cluster should be initialized only once." << endl;
        exit(1);
    }

    instance = new Cluster();
    instance->node = node_id;
    instance->total = total;
    instance->unique_id = unique_id;
    instance->master_location = master_location;
}

Cluster& Cluster::getInstance()
{
    if (instance == NULL) {
        cerr << "Should set Cluster first." << endl;
        exit(1);
    }
    return *instance;
}

} // namespace