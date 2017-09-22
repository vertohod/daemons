#ifndef CREATE_CHANNEL_H
#define CREATE_CHANNEL_H

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(create_channel,
    DECLARATION_FIELD(sptr_cstr, name)
    DECLARATION_FIELD(sptr_cstr, description)
)

} // end of namespace

#endif
