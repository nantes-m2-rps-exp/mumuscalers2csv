#include "AliAnalysisMuMuConfig.h"
#include "AliAnalysisMuMuFnorm.h"
#include "AliAnalysisRunList.h"
#include "AliCounterCollection.h"
#include <TFile.h>
#include <TString.h>
#include <array>

std::unique_ptr<AliAnalysisMuMuFnorm> basicScalers(
    const char *runlist = "run.list", const char *configFile = "mumu.pp.config",
    const char *ocdbPath = "local:///Users/laurent/cernbox/ocdbs/2018/OCDB") {
  AliAnalysisRunList rl(runlist);

  AliCounterCollection cc;
  cc.AddRubric("run", 1000);
  cc.Init();
  for (auto run : rl.AsVector()) {
    cc.Count(TString::Format("run:%d", run));
  }
  cc.Print();
  AliAnalysisMuMuConfig config;
  config.ReadFromFile(configFile);
  auto triggerType = AliAnalysisMuMuFnorm::kMUL;
  std::unique_ptr<AliAnalysisMuMuFnorm> fnorm(
      new AliAnalysisMuMuFnorm(cc, config, triggerType, ocdbPath));
  fnorm->RunNumbers();
  auto pileUpCorrected = false;
  auto eventSelectionCorrected = 0;
  fnorm->ComputeFnormScalers(pileUpCorrected, eventSelectionCorrected);
  return fnorm;
}

std::unique_ptr<AliAnalysisMuMuFnorm> mumuScalers(
    const char *ccFileName = "mumu.minv.root",
    const char *configFile = "mumu.pp.config",
    const char *ocdbPath = "local:///Users/laurent/cernbox/ocdbs/2018/OCDB") {

  std::unique_ptr<TFile> ccFile(TFile::Open(ccFileName));
  AliCounterCollection *cc =
      static_cast<AliCounterCollection *>(ccFile->Get("MuMuMinv/CC"));
  cc->Print();
  AliAnalysisMuMuConfig config;
  config.ReadFromFile(configFile);
  auto triggerType = AliAnalysisMuMuFnorm::kMUL;
  std::unique_ptr<AliAnalysisMuMuFnorm> fnorm(
      new AliAnalysisMuMuFnorm(*cc, config, triggerType, ocdbPath));

  auto eventSelectionCorrected = 0;
  for (auto pileUpCorrected : std::array<bool, 2>{true, false}) {
    fnorm->ComputeFnormScalers(pileUpCorrected, eventSelectionCorrected);
  }
  return fnorm;
}
