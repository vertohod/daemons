#ifndef RENAME_USER_H
#define RENAME_USER_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(rename_user,
    DECLARATION_FIELD(sptr_cstr, nickname)
)

} // end of namespace

#endif
