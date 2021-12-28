#pragma once

#include "AliAnalysisMuMuFnorm.h"
#include <memory>

std::unique_ptr<AliAnalysisMuMuFnorm> basicScalers(
    const char *runlist = "run.list", const char *configFile = "mumu.pp.config",
    const char *ocdbPath = "local:///Users/laurent/cernbox/ocdbs/2018/OCDB");

std::unique_ptr<AliAnalysisMuMuFnorm> mumuScalers(
    const char *ccFileName = "mumu.minv.root",
    const char *configFile = "mumu.pp.config",
    const char *ocdbPath = "local:///Users/laurent/cernbox/ocdbs/2018/OCDB");

void toto(){};
