#include "look_for_channel.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(look_for_channel)

DEFINITION_CONSTS(look_for_channel, "look_for_channel", BIT_NULL, 1)

DEFINITION_FIELD(look_for_channel, sptr_cstr, text)

look_for_channel::look_for_channel() {}

DEFINITION_SPECIAL_MEMBERS(look_for_channel)

} // end of namespace
