#ifndef CHANNEL_H
#define CHANNEL_H

#include "object_includes.h"
#include "defines_object.h"
#include "defines_key.h"

namespace chatd
{

DECLARATION_OBJECT(channel,
    DECLARATION_FIELD(swarm::index, oid)
    DECLARATION_FIELD(sptr_cstr, name)
    DECLARATION_FIELD(sptr_cstr, description)
)

DECLARATION_KEY(channel, key_channel_name, DECLARATION_FIELD(sptr_cstr, name))
DECLARATION_KEY(channel, key_channel_name3, DECLARATION_FIELD(sptr_cstr, name))
DECLARATION_KEY(channel, key_channel_name4, DECLARATION_FIELD(sptr_cstr, name))
DECLARATION_KEY(channel, key_channel_name5, DECLARATION_FIELD(sptr_cstr, name))
DECLARATION_KEY(channel, key_channel_name6, DECLARATION_FIELD(sptr_cstr, name))

} // end of namespace chatd

#endif
