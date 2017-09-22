#ifndef MESSAGE_H
#define MESSAGE_H

#include "object_includes.h"
#include "defines_object.h"
#include "defines_key.h"

namespace chatd
{

DECLARATION_OBJECT(message,
    DECLARATION_FIELD(OID, id)
    DECLARATION_FIELD(OID, channel_oid)
    DECLARATION_FIELD(OID, sender_oid)
    DECLARATION_FIELD(OID, recipient_oid)
    DECLARATION_FIELD(time_tt, time)
    DECLARATION_FIELD(bool, encrypted)
    DECLARATION_FIELD(sptr_cstr, payload)
)

DECLARATION_KEY(message, key_message_recipient,
    DECLARATION_FIELD(OID, recipient_oid)
    DECLARATION_FIELD(time_tt, time)
)
DECLARATION_KEY(message, key_message_ascend,
    DECLARATION_FIELD(OID, channel_oid)
    DECLARATION_FIELD(time_tt, time)
)
DECLARATION_KEY(message, key_message_descend,
    DECLARATION_FIELD(OID, channel_oid)
    DECLARATION_FIELD(time_tt, time)
)

} // end of namespace

#endif
