//
// Created by Bruno Liberini on 15/10/18.
//

#include "TmpCleaner.h"
#include <cstdlib>
#include "Logger.h"

TmpCleaner::TmpCleaner() {}

void TmpCleaner::clean_tmp() {
  std::system("rm -r /tmp/lakeconcu.bsq.* /tmp/lakeconcu.dock.* /tmp/lakeconcu.log.lock");
  LOG(LOG_INFO, "Carpeta /tmp/ limpia");
}
