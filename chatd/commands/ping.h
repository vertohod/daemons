#ifndef PING_H
#define PING_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(ping,
    DECLARATION_FIELD(OID, id)
    DECLARATION_FIELD(time_tt, time)
)

} // end of namespace

#endif
