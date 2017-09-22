#ifndef INSIDE_H
#define INSIDE_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(inside,
    DECLARATION_FIELD(OID, user_oid)
    DECLARATION_FIELD(sptr_cstr, nickname)
    DECLARATION_FIELD(bool, flag)
)

} // end of namespace

#endif
