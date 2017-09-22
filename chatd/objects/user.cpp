#include "user.h"
#include "defines_key_store.h"
#include "util.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(user)

DEFINITION_CONSTS(user, "user", BIT_SYNCHRONIZE, 1)

DEFINITION_FIELD(user, swarm::index, oid)
DEFINITION_FIELD(user, sptr_cstr, nickname)
DEFINITION_FIELD(user, size_tt, hash)
DEFINITION_FIELD(user, sptr_cstr, salt)
DEFINITION_FIELD(user, time_tt, created)
DEFINITION_FIELD(user, time_tt, block_time)
DEFINITION_FIELD(user, size_tt, violations_amount)
DEFINITION_FIELD(user, bool, block)

user::user() : 
    FIELD(oid)(0),
    FIELD(hash)(0),
    FIELD(created)(time(NULL)),
    FIELD(block_time)(0),
    FIELD(violations_amount)(0),
    FIELD(block)(false)
{}

// обязательные методы ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

DEFINITION_INIT_KEYS(user,
    DEFINITION_KEY_STORE(UNIQUE_MODEL, UNIQUE_SORTED_STORE, key_user_nickname)
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_SORTED_STORE, key_user_nickname3)
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_SORTED_STORE, key_user_nickname4)
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_SORTED_STORE, key_user_nickname5)
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_SORTED_STORE, key_user_nickname6)
)

DEFINITION_UNIQUE_KYES(user)
DEFINITION_SERIALIZATION(user)
DEFINITION_DESERIALIZATION_1(user)
DEFINITION_DESERIALIZATION_2(user)
DEFINITION_MAKE_COPY(user)
DEFINITION_SET_INDEX_FIELDS(user)

// ключи ++++++++++++++++++++++++++++++++++++++++++++++++++++++

DEFINITION_KEY(key_user_nickname, INIT(nickname), EQUAL(nickname), LESS(nickname))
DEFINITION_FIELD(key_user_nickname, sptr_cstr, nickname)
size_t key_user_nickname::hash() const
{
	return std::hash<std::string>()(*FIELD(nickname));
}

// 3 символа
DEFINITION_KEY(key_user_nickname3, auto string_temp = *user::convert(record_ptr->get_payload())->get_nickname(); auto offset = util::offset_utf8(string_temp, 3); FIELD(nickname) = string_temp.substr(0, offset);, EQUAL(nickname), LESS(nickname))
DEFINITION_FIELD(key_user_nickname3, sptr_cstr, nickname)
size_t key_user_nickname3::hash() const
{
	return std::hash<std::string>()(*FIELD(nickname));
}

// 4 символа
DEFINITION_KEY(key_user_nickname4, auto string_temp = *user::convert(record_ptr->get_payload())->get_nickname(); auto offset = util::offset_utf8(string_temp, 4); FIELD(nickname) = string_temp.substr(0, offset);, EQUAL(nickname), LESS(nickname))
DEFINITION_FIELD(key_user_nickname4, sptr_cstr, nickname)
size_t key_user_nickname4::hash() const
{
	return std::hash<std::string>()(*FIELD(nickname));
}

// 5 символа
DEFINITION_KEY(key_user_nickname5, auto string_temp = *user::convert(record_ptr->get_payload())->get_nickname(); auto offset = util::offset_utf8(string_temp, 5); FIELD(nickname) = string_temp.substr(0, offset);, EQUAL(nickname), LESS(nickname))
DEFINITION_FIELD(key_user_nickname5, sptr_cstr, nickname)
size_t key_user_nickname5::hash() const
{
	return std::hash<std::string>()(*FIELD(nickname));
}

// 6 символа
DEFINITION_KEY(key_user_nickname6, auto string_temp = *user::convert(record_ptr->get_payload())->get_nickname(); auto offset = util::offset_utf8(string_temp, 6); FIELD(nickname) = string_temp.substr(0, offset);, EQUAL(nickname), LESS(nickname))
DEFINITION_FIELD(key_user_nickname6, sptr_cstr, nickname)
size_t key_user_nickname6::hash() const
{
	return std::hash<std::string>()(*FIELD(nickname));
}

} // end of namespace
