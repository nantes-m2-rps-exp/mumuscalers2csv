#include "AliAnalysisRunList.h"
#include "AliAnalysisTriggerScalers.h"
#include "AliCounterCollection.h"
#include "AliLHCData.h"
#include <TFile.h>
#include <boost/program_options.hpp>
#include <fmt/core.h>
#include <iostream>
#include <string>

namespace po = boost::program_options;

void scalerOnline2csv(AliAnalysisTriggerScalers &ts, std::ostream &out) {

  out << "run,cint7l0b,cmul7l0b,duration(s),interacting_bunches\n";
  for (auto run : ts.GetRunList()) {
    AliLHCData *lhc =
        static_cast<AliLHCData *>(ts.GetOCDBObject("GRP/GRP/LHCData", run));
    auto cint = ts.GetTriggerScaler(run, "L0B", "CINT7-B-NOPF-MUFAST");
    auto cint7l0b = cint->Value();
    auto cmul = ts.GetTriggerScaler(run, "L0B", "CMUL7-B-NOPF-MUFAST");
    auto cmul7l0b = cmul->Value();
    int runDuration =
        cint->Duration(); // should be the same as cmul->Duration()
    int interactingBunches =
        ts.NumberOfInteractingBunches(*lhc, run, false, true);
    out << fmt::format("{:d},{},{},{:d},{:d}\n", run, cint7l0b, cmul7l0b,
                       runDuration, interactingBunches);
  }
}
void scalerOffline2csv(AliCounterCollection &cc, std::ostream &out) {

  out << "run,cint7all,cint7ps,cmul7all,cmul7ps,cmsl7all,cmsl7ps,cint7all&0msl,"
         "cmsl7all&0mul\n";
  AliAnalysisRunList runList(cc.GetKeyWords("run"));
  for (auto run : runList.AsVector()) {
    int cint7ps = static_cast<int>(cc.GetSum(
        fmt::format(
            "run:{}/centrality:PP/event:PSANY/trigger:CINT7-B-NOPF-MUFAST", run)
            .c_str()));
    int cint7all = static_cast<int>(cc.GetSum(
        fmt::format(
            "run:{}/centrality:PP/event:ALL/trigger:CINT7-B-NOPF-MUFAST", run)
            .c_str()));
    int cmul7ps = static_cast<int>(cc.GetSum(
        fmt::format(
            "run:{}/centrality:PP/event:PSANY/trigger:CMUL7-B-NOPF-MUFAST", run)
            .c_str()));
    int cmul7all = static_cast<int>(cc.GetSum(
        fmt::format(
            "run:{}/centrality:PP/event:ALL/trigger:CMUL7-B-NOPF-MUFAST", run)
            .c_str()));
    int cmsl7ps = static_cast<int>(cc.GetSum(
        fmt::format(
            "run:{}/centrality:PP/event:PSANY/trigger:CMSL7-B-NOPF-MUFAST", run)
            .c_str()));
    int cmsl7all = static_cast<int>(cc.GetSum(
        fmt::format(
            "run:{}/centrality:PP/event:ALL/trigger:CMSL7-B-NOPF-MUFAST", run)
            .c_str()));
    int cint_and_0msl = static_cast<int>(cc.GetSum(
        fmt::format(
            "run:{}/centrality:PP/event:ALL/trigger:CINT7-B-NOPF-MUFAST&0MSL",
            run)
            .c_str()));
    int cmsl_and_0mul = static_cast<int>(cc.GetSum(
        fmt::format(
            "run:{}/centrality:PP/event:ALL/trigger:CMSL7-B-NOPF-MUFAST&0MUL",
            run)
            .c_str()));
    out << fmt::format("{:d},{:d},{:d},{:d},{:d},{:d},{:d},{:d},{:d}\n", run,
                       cint7all, cint7ps, cmul7all, cmul7ps, cmsl7all, cmsl7ps,
                       cint_and_0msl, cmsl_and_0mul);
  }
}

int main(int argc, char **argv) {
  po::variables_map vm;
  po::options_description options("options");

  // clang-format off
  options.add_options()
    ("help,h","produce help message")
    ("input-file,i",po::value<std::string>()->default_value(""),"input root file (result from MuMu analysis)")
    ("output-file,o",po::value<std::string>()->default_value("scalers.csv"),"output CSV file")
    ("ocdb-path",po::value<std::string>()->default_value("raw://"),"ocdb path")
    ("run-list",po::value<std::string>()->default_value(""),"run list to be used (for online scalers")
    ;
  // clang-format on

  po::options_description cmdline;
  cmdline.add(options);

  po::store(po::command_line_parser(argc, argv).options(cmdline).run(), vm);

  if (vm.count("help")) {
    std::cout << "Convert counter collection from mumu analysis to CSV file\n";
    std::cout << R"(
    two modes of operation:
    - using --ocdb-path and --run-list to get only information from OCDB
    - using (instead of or in addition to the previous) --input-file to get information from a prior analysis with counter collection
    )";
    return 2;
  }

  try {
    po::notify(vm);

  } catch (boost::program_options::error &e) {
    std::cout << "Error: " << e.what() << "\n";
    exit(1);
  }

  auto inFile = vm["input-file"].as<std::string>();
  auto outFile = vm["output-file"].as<std::string>();
  std::ofstream fout(outFile);

  std::ostream &out = outFile.empty() ? std::cout : fout;

  if (!inFile.empty()) {
    TFile *f = TFile::Open(inFile.c_str());
    if (!f || !f->IsOpen()) {
      std::cout << "could not open root file " << inFile << "\n";
      return 3;
    }
    AliCounterCollection *cc =
        static_cast<AliCounterCollection *>(f->Get("MuMuMinv/CC"));
    if (!cc) {
      std::cout << "could not get counter collection from input file (folder: "
                   "MuMuMinv/CC)\n";
      return 3;
    }
    scalerOffline2csv(*cc, out);
  }

  auto ocdbPath = vm["ocdb-path"].as<std::string>();
  auto runList = vm["run-list"].as<std::string>();

  if (!ocdbPath.empty() && !runList.empty()) {
    AliAnalysisTriggerScalers ts(runList.c_str(), ocdbPath.c_str());
    scalerOnline2csv(ts, out);
  }
}
