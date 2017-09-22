#ifndef CREATE_USER_H
#define CREATE_USER_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(create_user,
    DECLARATION_FIELD(sptr_cstr, nickname)
    DECLARATION_FIELD(sptr_cstr, password)
)

} // end of namespace

#endif
