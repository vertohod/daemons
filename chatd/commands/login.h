#ifndef LOGIN_H
#define LOGIN_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(login,
    DECLARATION_FIELD(sptr_cstr, nickname)
    DECLARATION_FIELD(sptr_cstr, password)
    DECLARATION_FIELD(sptr_cstr, hash)
)

} // end of namespace

#endif
