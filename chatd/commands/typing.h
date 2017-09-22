#ifndef TYPING_H
#define TYPING_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(typing,
    DECLARATION_FIELD(OID, user_oid)
    DECLARATION_FIELD(sptr_cstr, nickname)
    DECLARATION_FIELD(bool, flag)
)

} // end of namespace

#endif
