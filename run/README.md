# Example of running the scalers2csv executable

## "Online" mode

This mode relies only on OCDB information.

```shell
$ scalers2csv --ocdb-path raw:// --run-list run.list --output-file scalers.online.csv
...snip...
$ head -3 scalers.online.csv
run,cint7l0b,cmul7l0b,duration(s),interacting_bunches
290222,2821944,1135,4613,1
290223,1215525,507,2126,1
```

Even better, if available, is to use an OCDB copied locally. The above command will work, but won't be terribly efficient as the OCDB access through AliEn is quite slow.

## "Offline" mode

For this mode, you'll need the result of an [AliAnalysisMuMu...](https://github.com/nantes-m2-rps-exp/aliphysics-lite/blob/master/muon/runMuMu.C) analysis. The output of such an analysis can be found at [mumu.minv.root](https://cernbox.cern.ch/index.php/s/WYY0FXaY6SGzJYt/download).

The runlist is taken from the analysis result directly and thus does not need to be given.

```shell
$ curl -L https://cernbox.cern.ch/index.php/s/WYY0FXaY6SGzJYt/download -o mumu.minv.root
$ scalers2csv --ocdb-path raw:// --input-file ./mumu.minv.root --output-file scalers.offline.csv
$ head -3 scalers.offline.csv
run,cint7all,cint7ps,cmul7all,cmul7ps,cmsl7all,cmsl7ps,cint7all&0msl,cmsl7all&0mul
290222,138603,138486,1119,1118,50296,50278,2518,1119
290223,57552,57491,484,484,21027,21014,1038,484
```
