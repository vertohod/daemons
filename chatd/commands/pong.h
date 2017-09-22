#ifndef PONG_H
#define PONG_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(pong,
    DECLARATION_FIELD(OID, id)
    DECLARATION_FIELD(time_tt, time)
)

} // end of namespace

#endif
