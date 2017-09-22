#ifndef REMOVE_LOGIN_H
#define REMOVE_LOGIN_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(remove_login,
    DECLARATION_FIELD(sptr_cstr, password)
)

} // end of namespace

#endif
