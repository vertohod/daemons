#ifndef STATUS_H
#define STATUS_H

#include <list>

#include "object_includes.h"
#include "defines_object.h"
#include "channel.h"
#include "client.h"

namespace chatd
{

DECLARATION_OBJECT(status,
    DECLARATION_FIELD(OID, user_oid)
    DECLARATION_FIELD(sptr_cstr, nickname)
    DECLARATION_FIELD(time_tt, created)
    DECLARATION_FIELD(size_tt, connections_amount)
    DECLARATION_FIELD(std::shared_ptr<std::list<std::shared_ptr<const chatd::channel>>>, channels)
)

} // end of namespace

#endif
