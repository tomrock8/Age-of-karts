#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "NetworkIDManager.h"
#include "NetworkIDObject.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "PacketLogger.h"

#if LIBCAT_SECURITY==1
#include "SecureHandshake"
#endif

#if defined(_CONSOLE_2)
#include "Console2SampleIncludes.h"
#endif
#ifdef _CONSOLE_2
_CONSOL_2_SetSystemProcessParams
#endif

#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include "Kbhit.h"
#include <stdio.h>
#include <string.h>