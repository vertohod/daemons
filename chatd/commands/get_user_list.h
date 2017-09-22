#ifndef GET_USER_LIST_H
#define GET_USER_LIST_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(get_user_list,
    DECLARATION_FIELD(OID, channel_oid)
)

} // end of namespace

#endif
