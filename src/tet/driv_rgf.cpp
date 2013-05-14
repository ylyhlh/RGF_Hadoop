/* * * * *
 *  driv_rgf.cpp
 *  Copyright (C) 2011, 2012 Rie Johnson
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * * * * */

#define _AZ_MAIN_

#include <map>
#include "AzUtil.hpp"
#include "AzTETmain.hpp"
#include "AzRgfTrainerSel.hpp"
#include "AzTET_Eval_Dflt.hpp"
#include "AzHelp.hpp"
#include "accumulate.h"
#include "utils.h"
#include "timer.h"

extern Timer vis_timer;

using std::map;
using std::string;

/*-----------------------------------------------------------------*/
void help(int argc, const char *argv[])
{
  cout << "Arguments: action  parameters" <<endl;
  cout << "   action: "<<kw_train<<"|"<<kw_predict<<"|"<<kw_train_test<<"|"<<kw_train_predict<<"|"<<kw_features<<endl;
  AzHelp h(log_out);
  h.set_indent(11);
  h.set_kw_width(17);
  AzBytArr s_kw, s_desc;
  s_kw.reset(kw_train); s_kw.c("      ..."); s_desc.reset(help_train);
  h.item_noquotes(s_kw.c_str(), s_desc.c_str());
  s_kw.reset(kw_predict); s_kw.c("    ..."); s_desc.reset(help_predict);
  h.item_noquotes(s_kw.c_str(), s_desc.c_str());
  s_kw.reset(kw_train_test); s_kw.c(" ..."); s_desc.reset(help_train_test);
  h.item_noquotes(s_kw.c_str(), s_desc.c_str());
  s_kw.reset(kw_train_predict); s_kw.c(" ..."); s_desc.reset(help_train_predict);
  h.item_noquotes(s_kw.c_str(), s_desc.c_str());
  s_kw.reset(kw_features); s_kw.c(" ..."); s_desc.reset(help_features);
  h.item_noquotes(s_kw.c_str(), s_desc.c_str());
  cout << endl;
  cout << "To get help on parameters, enter "<<argv[0]<<" action."<<endl;
  cout << "For example:  "<<argv[0]<<" "<<kw_train_test<<endl;
  cout << "              "<<argv[0]<<" "<<kw_train     <<endl;
}

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

/*******************************************************************/
/*     main                                                        */
/*******************************************************************/
int main(int argc, const char *argv[])
{
  AzException *stat = NULL;

  if (argc < 2) {
    help(argc, argv);
    return -1;
  }
  vis_timer.start();
  // Register to cluster if cluster parameters are given.
  map<string, string> arg_map;
  parse_args(argc, argv, arg_map);
  if (arg_map.size() > 0) {
    // cluster parameters are given.
    verify(arg_map.count("master") > 0);
    verify(arg_map.count("node_id") > 0);
    verify(arg_map.count("total") > 0);
    verify(arg_map.count("unique_id") > 0);
    int node = atoi(arg_map["node_id"].c_str());
    int unique_id = atoi(arg_map["unique_id"].c_str());
    int total = atoi(arg_map["total"].c_str());
    Hadoop::Cluster::init_cluster(node, total, unique_id, arg_map["master"]);
  }

  const char *action = argv[1];

  AzRgfTrainerSel alg_sel;
  AzTET_Eval_Dflt eval;
  AzTETmain driver(&alg_sel, &eval);
  try {
    Az_check_system_();

    if (strcmp(action, kw_train) == 0) {
      driver.train(argv, argc);
    }
    else if (strcmp(action, kw_predict) == 0) {
      driver.predict_single(argv, argc);
    }
    else if (strcmp(action, kw_train_test) == 0) {
      driver.train_test(argv, argc);
    }
    else if (strcmp(action, kw_train_predict) == 0) {
      driver.train_predict(argv, argc);
    }
    else if (strcmp(action, kw_batch_predict) == 0) {
      driver.batch_predict(argv, argc);
    }
    else if (strcmp(action, kw_features) == 0) {
      driver.features(argv, argc);
    }
    else {
      help(argc, argv);
      return -1;
    }
  }
  catch (AzException *e) {
    stat = e;
  }

  if (stat != NULL) {
    cout << stat->getMessage() << endl;
    return -1;
  }
  cerr << "Allreduce time: " << Hadoop::all_reduce_watch.get_total() << endl;
  cerr << "Allreduce data(bytes): " << Hadoop::all_reduce_data_amount << endl;
  cerr << "Allreduce count: " << Hadoop::all_reduce_counter << endl;

  return 0;
}

