#ifndef USER_H
#define USER_H

#include "object_includes.h"
#include "defines_object.h"
#include "defines_key.h"

namespace chatd
{

DECLARATION_OBJECT(user,
    DECLARATION_FIELD(swarm::index, oid)
    DECLARATION_FIELD(sptr_cstr, nickname)
    DECLARATION_FIELD(size_tt, hash)
    DECLARATION_FIELD(sptr_cstr, salt)
    DECLARATION_FIELD(time_tt, created)
    DECLARATION_FIELD(time_tt, block_time)
    DECLARATION_FIELD(size_tt, violations_amount) // при N нарушениях блокировать по IP
    DECLARATION_FIELD(bool, block)
)

DECLARATION_KEY(user, key_user_nickname, DECLARATION_FIELD(sptr_cstr, nickname))
DECLARATION_KEY(user, key_user_nickname3, DECLARATION_FIELD(sptr_cstr, nickname))
DECLARATION_KEY(user, key_user_nickname4, DECLARATION_FIELD(sptr_cstr, nickname))
DECLARATION_KEY(user, key_user_nickname5, DECLARATION_FIELD(sptr_cstr, nickname))
DECLARATION_KEY(user, key_user_nickname6, DECLARATION_FIELD(sptr_cstr, nickname))

} // end of namespace

#endif
