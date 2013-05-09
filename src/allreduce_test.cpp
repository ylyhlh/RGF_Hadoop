#include <cstdlib>
#include <iostream>
#include <sstream>

#include "accumulate.h"
#include "utils.h"

using namespace std;
using namespace Hadoop;

void test_scale(int total) {
    double data;
    data = accumulate_scalar(1); // 1 * total
    data = accumulate_scalar(data); // 1 * total * total
    cout << "Sum of data: " << data << endl;
    verify((int)data == total * total);
}

void test_sum(int total) {
    const int length = 10;
    double data[length];
    for (int i = 0; i < length; ++i)
    {
        data[i] = i;
    }
    accumulate_sum(data, length);
    for (int i = 0; i < length; ++i)
    {
        verify(int(data[i]) == i * total);
    }
}

void test_avg(int total) {
    const int length = 10;
    double data[length];
    for (int i = 0; i < length; ++i)
    {
        data[i] = i;
    }
    accumulate_avg(data, length);
    for (int i = 0; i < length; ++i)
    {
        verify(int(data[i]) == i);
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        cout << argv[0] << " <master> <total> <node_id>" << endl;
        return 1;
    }
    string master_location;
    stringstream ss;
    ss << argv[1];
    ss >> master_location;

    int node = atoi(argv[3]);
    int unique_id = 1234;
    int total = atoi(argv[2]);

    Cluster::init_cluster(node, total, unique_id, master_location);

    Cluster& cluster = Cluster::getInstance();
    cout << "total: " << cluster.total << " node_id: " << cluster.node << endl;

    test_scale(total);
    cout << "test_scale passes." << endl;
    test_sum(total);
    cout << "test_sum passes." << endl;
    test_avg(total);
    cout << "test_avg passes." << endl;
    return 0;
}