#include "channel.h"
#include "defines_key_store.h"
#include "util.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(channel)

DEFINITION_CONSTS(channel, "channel", BIT_SYNCHRONIZE, 1)

DEFINITION_FIELD(channel, swarm::index, oid)
DEFINITION_FIELD(channel, sptr_cstr, name)
DEFINITION_FIELD(channel, sptr_cstr, description)

channel::channel() {}

// обязательные методы ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

DEFINITION_INIT_KEYS(channel,
    DEFINITION_KEY_STORE(UNIQUE_MODEL, UNIQUE_SORTED_STORE, key_channel_name)
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_SORTED_STORE, key_channel_name3)
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_SORTED_STORE, key_channel_name4)
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_SORTED_STORE, key_channel_name5)
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_SORTED_STORE, key_channel_name6)
)

DEFINITION_UNIQUE_KYES(channel)
DEFINITION_SERIALIZATION(channel)
DEFINITION_DESERIALIZATION_1(channel)
DEFINITION_DESERIALIZATION_2(channel)
DEFINITION_MAKE_COPY(channel)
DEFINITION_SET_INDEX_FIELDS(channel)

// ключи ++++++++++++++++++++++++++++++++++++++++++++++++++++++

DEFINITION_KEY(key_channel_name, INIT(name), EQUAL(name), LESS(name))
DEFINITION_FIELD(key_channel_name, sptr_cstr, name)
size_t key_channel_name::hash() const
{
	return std::hash<std::string>()(*FIELD(name));
}

// 3 символа
DEFINITION_KEY(key_channel_name3, auto string_temp = *channel::convert(record_ptr->get_payload())->get_name(); auto offset = util::offset_utf8(string_temp, 3); FIELD(name) = string_temp.substr(0, offset);, EQUAL(name), LESS(name))
DEFINITION_FIELD(key_channel_name3, sptr_cstr, name)
size_t key_channel_name3::hash() const
{
	return std::hash<std::string>()(*FIELD(name));
}

// 4 символа
DEFINITION_KEY(key_channel_name4, auto string_temp = *channel::convert(record_ptr->get_payload())->get_name(); auto offset = util::offset_utf8(string_temp, 4); FIELD(name) = string_temp.substr(0, offset);, EQUAL(name), LESS(name))
DEFINITION_FIELD(key_channel_name4, sptr_cstr, name)
size_t key_channel_name4::hash() const
{
	return std::hash<std::string>()(*FIELD(name));
}

// 5 символа
DEFINITION_KEY(key_channel_name5, auto string_temp = *channel::convert(record_ptr->get_payload())->get_name(); auto offset = util::offset_utf8(string_temp, 5); FIELD(name) = string_temp.substr(0, offset);, EQUAL(name), LESS(name))
DEFINITION_FIELD(key_channel_name5, sptr_cstr, name)
size_t key_channel_name5::hash() const
{
	return std::hash<std::string>()(*FIELD(name));
}

// 6 символа
DEFINITION_KEY(key_channel_name6, auto string_temp = *channel::convert(record_ptr->get_payload())->get_name(); auto offset = util::offset_utf8(string_temp, 6); FIELD(name) = string_temp.substr(0, offset);, EQUAL(name), LESS(name))
DEFINITION_FIELD(key_channel_name6, sptr_cstr, name)
size_t key_channel_name6::hash() const
{
	return std::hash<std::string>()(*FIELD(name));
}

} // end of namespace chatd
