// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is used as a precompiled header for both C and C++ files. So
// any C++ headers must go in the __cplusplus block below.

#if defined(BUILD_PRECOMPILE_H_)
#error You shouldn't include the precompiled header file more than once.
#endif

#define BUILD_PRECOMPILE_H_

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <memory.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(__cplusplus)

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iosfwd>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#endif  // __cplusplus



#ifdef _USE_UI_LIBRAY

#include "souistd.h"
#include "SApp.h"
#include "core/SHostDialog.h"
#include "core/SMsgLoop.h"
#include "control/SMessageBox.h"
#include "control/souictrls.h" 

#endif


#define GG_UNUSED(x) ((void)x) 

#define ASAFE_DELETE(x) { if (x) delete x; x = NULL; }
#define ASAFE_DELETES(x) { if (x) delete [] x; x = NULL; }
#define ASAFE_CLOSE(x) { if (x) CloseHandle(x); x = NULL; }