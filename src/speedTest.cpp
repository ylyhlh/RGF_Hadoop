#include "timer.h"
#include "accumulate.h"
#include <map>
#include "utils.h"

using std::map;
using std::string;

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

int main(int argc, const char *argv[])
{
  Timer allreduce_wait_timer;
  if (argc < 4) 
  {
  	std::cerr<<"NO enough parameters!"<<std::endl;
  	abort();
  }
  //int length = atoi(argv[1]);
  //int num_of_allreduce = atoi(argv[2]);
  int length = 1000;
  int times = 100000;
  

  // Register to cluster if cluster parameters are given.
  map<string, string> arg_map;
  
  parse_args(argc, argv, arg_map);
  if (arg_map.size() > 0) {
    // cluster parameters are given.
  	verify(arg_map.count("master") > 0);
    verify(arg_map.count("node_id") > 0);
    verify(arg_map.count("total") > 0);
    verify(arg_map.count("unique_id") > 0);
    if (arg_map.count("length") > 0)
    	length = atoi(arg_map["length"].c_str());
    if (arg_map.count("times") > 0)
    	times = atoi(arg_map["times"].c_str());
    int node = atoi(arg_map["node_id"].c_str());
    int unique_id = atoi(arg_map["unique_id"].c_str());
    int total = atoi(arg_map["total"].c_str());
    Hadoop::Cluster::init_cluster(node, total, unique_id, arg_map["master"]);
  }
  printf("length=%d\n", length);
  printf("num_of_allreduce=%d\n",times);

	allreduce_wait_timer.start();
	float *data = new float[length];
	for (int i=0; i < times; ++i)
	{
		Hadoop::accumulate_avg(data,length);
		//std::cerr << "time: " << Hadoop::all_reduce_watch.get_total() << "data(bytes): " << Hadoop::all_reduce_data_amount << "count: " << Hadoop::all_reduce_counter << std::endl;
	}
	//
	std::cerr << "speed(Gips)" << double(Hadoop::all_reduce_data_amount)/Hadoop::all_reduce_watch.get_total()/1E9<<std::endl;
}