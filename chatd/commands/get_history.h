#ifndef GET_HISTORY_H
#define GET_HISTORY_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(get_history,
    DECLARATION_FIELD(OID, channel_oid)
    DECLARATION_FIELD(time_tt, time)
    DECLARATION_FIELD(size_tt, limit)
)

} // end of namespace

#endif
