#ifndef PASSWORD_HASH_H
#define PASSWORD_HASH_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(password_hash,
    DECLARATION_FIELD(sptr_cstr, value)
)

} // end of namespace

#endif
