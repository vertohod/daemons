#ifndef FOUND_USER_H
#define FOUND_USER_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(found_user,
    DECLARATION_FIELD(OID, user_oid)
    DECLARATION_FIELD(sptr_cstr, nickname)
)

} // end of namespace

#endif
