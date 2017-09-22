#ifndef CHANNEL_USER_H
#define CHANNEL_USER_H

#include "object_includes.h"
#include "defines_object.h"
#include "defines_key.h"

namespace chatd
{

DECLARATION_OBJECT(channel_user,
    DECLARATION_FIELD(OID, channel_oid)
    DECLARATION_FIELD(OID, user_oid)
)

DECLARATION_KEY(channel_user, key_channel_user_user, DECLARATION_FIELD(OID, user_oid))
DECLARATION_KEY(channel_user, key_channel_user_channel, DECLARATION_FIELD(OID, channel_oid))
DECLARATION_KEY(channel_user, key_channel_user_channel_user,
    DECLARATION_FIELD(OID, channel_oid)
    DECLARATION_FIELD(OID, user_oid)
)

} // end of namespace chatd

#endif
