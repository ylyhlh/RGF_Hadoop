#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

#include "accumulate.h"
#include "utils.h"

using namespace std;
using namespace Hadoop;

void parse_args(int argc, const char* argv[], map<string, string>& arg_map) {
    string prefix("--");
    for (int i = 0; i < argc-1; ++i)
    {
        string arg(argv[i]);
        if (arg.compare(0, prefix.size(), prefix) == 0) {
            string arg_value(argv[i+1]);
            arg_map[arg.substr(2)] = arg_value;
            ++i;
        }
    }
}

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

void test_count_file(const string& file_name) {
  double count = 0;
  string line;
  ifstream fin(file_name.c_str());
  if (fin.is_open())
  {
    while ( fin.good() )
    {
      getline (fin,line);
      if (!line.empty()) count++;
      // cout << line << endl;
    }
    fin.close();
  }
  else cout << "Unable to open file";
  double total_count = accumulate_scalar(count);
  cerr << "total count: " << total_count << endl;
}

int main(int argc, const char* argv[]) {
    if (argc < 4) {
        cout << argv[0] << " <master> <total> <node_id>" << endl;
        return 1;
    }

    string master_location(argv[1]);
    int node = atoi(argv[3]);
    int unique_id = 1234;
    int total = atoi(argv[2]);

    string test_job;
    map<string, string> arg_map;
    parse_args(argc, argv, arg_map);
    if (arg_map.count("job") > 0) test_job = arg_map["job"];

    Cluster::init_cluster(node, total, unique_id, master_location);

    Cluster& cluster = Cluster::getInstance();
    cout << "total: " << cluster.total << " node_id: " << cluster.node << endl;

    if (test_job.empty()) {
        test_scale(total);
        cout << "test_scale passes." << endl;
        test_sum(total);
        cout << "test_sum passes." << endl;
        test_avg(total);
        cout << "test_avg passes." << endl;
    } else if (test_job == "count_file") {
        if (arg_map.count("file")) {
            test_count_file(arg_map["file"]);
        } else {
            cout << "--file is not given" << endl;
            return 1;
        }
    }
    return 0;
}
