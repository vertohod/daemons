#include "create_channel.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(create_channel)

DEFINITION_CONSTS(create_channel, "create_channel", BIT_NULL, 1)

DEFINITION_FIELD(create_channel, sptr_cstr, name)
DEFINITION_FIELD(create_channel, sptr_cstr, description)

create_channel::create_channel() {}

DEFINITION_SPECIAL_MEMBERS(create_channel)

} // end of namespace
