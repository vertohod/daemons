#ifndef CHANGED_NICKNAME_H
#define CHANGED_NICKNAME_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(changed_nickname,
    DECLARATION_FIELD(OID, user_oid)
    DECLARATION_FIELD(sptr_cstr, nickname)
    DECLARATION_FIELD(sptr_cstr, nickname_past)
)

} // end of namespace

#endif
