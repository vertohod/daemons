#include <condition_variable>
#include <unordered_map>
#include <exception>
#include <iostream>
#include <string>
#include <mutex>
#include "gtest/gtest.h"
#include <unistd.h>

#include "defines_for_filter.h"
#include "channel_user.h"
#include "singleton.h"
#include "swarm.h"
#include "message.h"
#include "user.h"
#include "util.h"
#include "log.h"

namespace test_synchronization
{
    bool flag;
    std::mutex mutex;
    std::condition_variable cv;
}

void unset()
{
    std::lock_guard<std::mutex> lock(test_synchronization::mutex);
    test_synchronization::flag = false;
}

void wait()
{
    std::unique_lock<std::mutex> lock(test_synchronization::mutex);
    test_synchronization::cv.wait(lock, []{ return test_synchronization::flag; });
}

void notify()
{
    {
        std::lock_guard<std::mutex> lock(test_synchronization::mutex);
        test_synchronization::flag = true;
    }
    test_synchronization::cv.notify_one();
}

namespace test_configuration
{
    std::string ip_port = "";
    std::string secret = "secret";
    std::unordered_map<std::string, std::string> upstreams;
}
/*
TEST (BlizzardTest, CheckUserTable)
{
    swarm::swarm bl(0, true, test_configuration::ip_port, test_configuration::secret, test_configuration::upstreams);

    bl.add_table<chatd::user>();

    auto ptr = chatd::user::create();

    ASSERT_EQ(bl.size<chatd::user>(), 0);

    unset();
    bl.insert(ptr, [](OID oid){ notify(); });
    wait();

    ASSERT_EQ(bl.size<chatd::user>(), 1);
}

TEST (BlizzardTest, CheckUserKeyCompare)
{
    const std::string nick("asdf");

    auto key1 = chatd::key_user_nickname::create();
    key1->set_nickname(nick);

    auto key2 = chatd::key_user_nickname::create();
    key2->set_nickname(nick);

    ASSERT_EQ(*key1, *key2);
}

TEST (BlizzardTest, CheckUserKeyTable)
{
    const std::string nick("asdf");

    swarm::swarm bl(0, true, test_configuration::ip_port, test_configuration::secret, test_configuration::upstreams);
    bl.add_table<chatd::user>();
		
    auto ptr = chatd::user::create();
    ptr->set_nickname(nick);

    unset();
    bl.insert(ptr, [](OID oid){ notify(); });
    wait();

    auto key_ptr = chatd::key_user_nickname::create();
	key_ptr->set_nickname(nick);

    auto answer_ptr = bl.find(key_ptr);

    ASSERT_EQ(answer_ptr->get_result()->size(), 1);
}

TEST (BlizzardTest, CheckChannelUserKeyChannel)
{
    swarm::swarm bl(0, true, test_configuration::ip_port, test_configuration::secret, test_configuration::upstreams);
    bl.add_table<chatd::channel_user>();

    for (size_t count = 1; count <= 20; ++count) {
        auto ptr = chatd::channel_user::create();
        ptr->set_channel_oid(count <= 10 ? 1 : 2);
        ptr->set_user_oid(count);

        unset();
        bl.insert(ptr, [](OID oid){ notify(); });
        wait();
    }

    auto key_ptr = chatd::key_channel_user_channel::create();
    key_ptr->set_channel_oid(2);

    auto answer_ptr = bl.find(key_ptr);

    ASSERT_EQ(answer_ptr->get_result()->size(), 10);
}

TEST (BlizzardTest, CheckChannelUserKeyChannelUser)
{
    swarm::swarm bl(0, true, test_configuration::ip_port, test_configuration::secret, test_configuration::upstreams);
    bl.add_table<chatd::channel_user>();

    for (size_t count = 1; count <= 20; ++count) {
        auto ptr = chatd::channel_user::create();
        ptr->set_channel_oid(count);
        ptr->set_user_oid(count);

        unset();
        bl.insert(ptr, [](OID oid){ notify(); });
        wait();
    }

    auto key_ptr = chatd::key_channel_user_channel_user::create();
    key_ptr->set_channel_oid(2);
    key_ptr->set_user_oid(2);

    auto answer_ptr = bl.find(key_ptr);

    ASSERT_EQ(answer_ptr->get_result()->size(), 1);
}

TEST (BlizzardTest, CheckFilterForGetWithLimit)
{
    swarm::swarm bl(0, true, test_configuration::ip_port, test_configuration::secret, test_configuration::upstreams);
    bl.add_table<chatd::user>();

    for (size_t count = 0; count < 20; ++count) {
        auto ptr = chatd::user::create();
        ptr->set_nickname(format(count));

        unset();
        bl.insert(ptr, [](OID oid){ notify(); });
        wait();
    }

    ASSERT_EQ(bl.size<chatd::user>(), 20);

    auto answer_ptr = bl.get_with_limit<chatd::user>(0, 0, [](const swarm::object& obj){FROM(chatd::user) WHERE(INDEX(oid) % 2 == 1)});

    ASSERT_EQ(answer_ptr->get_result()->size(), 9);
}

TEST (BlizzardTest, CheckFindRange)
{
    swarm::swarm bl(0, true, test_configuration::ip_port, test_configuration::secret, test_configuration::upstreams);
    bl.add_table<chatd::message>();

    for (size_t count = 0; count < 20; ++count) {
        auto ptr = chatd::message::create();
        ptr->set_channel_oid(count / 10 + 1);
        ptr->set_time(count);

        unset();
        bl.insert(ptr, [](OID oid){ notify(); });
        wait();
    }

    ASSERT_EQ(bl.size<chatd::message>(), 20);

    auto key_ptr = chatd::key_message_descend::create();
    key_ptr->set_channel_oid(2);
    key_ptr->set_time(20);
    auto answer_ptr = bl.find_range(key_ptr, nullptr, 0, 0, [](const swarm::object& obj){FROM(chatd::message) WHERE(FL(channel_oid) == 2)});

    ASSERT_EQ(answer_ptr->get_result()->size(), 10);
}

TEST (BlizzardTest, CheckMaximumMessageElements)
{
    swarm::swarm bl(0, true, test_configuration::ip_port, test_configuration::secret, test_configuration::upstreams);
    bl.add_table<chatd::message>();

    std::string str("Здесь какой-нибудь типичный текст, примерной средней длины. ");

    //size_t maximum = -1;
    size_t maximum = 1000;
    size_t count = 0;
    try {
        for (count = 0; count < maximum; ++count) {
            auto ptr = chatd::message::create();
            ptr->set_channel_oid(count);
            ptr->set_time(count);
            ptr->set_payload(str + format(count));

            unset();
            bl.insert(ptr, [](OID oid){ notify(); });
            wait();
        }
    } catch (const std::exception& e) {
        std::cout << "Was inserted record: " << count << std::endl;
        std::cout << e.what();
    }

    ASSERT_GT(bl.size<chatd::message>(), 0);
}
*/
TEST (BlizzardTest, CheckSynchronizationBetweenTwoServers)
{
    lo::set_log_level(lo::TRASH);

    std::string ip_port_server1 = "127.0.0.1:4444";
    auto bl1 = std::make_shared<swarm::swarm>(1, true, ip_port_server1, test_configuration::secret, test_configuration::upstreams);
    bl1->add_table<chatd::message>();

    std::string ip_port_server2 = "127.0.0.1:4455";
    std::unordered_map<std::string, std::string> upstreams2 = {{"master","127.0.0.1:4444"}};
    auto bl2 = std::make_shared<swarm::swarm>(2, false, ip_port_server2, test_configuration::secret, upstreams2);
    bl2->add_table<chatd::message>();

    sleep(10);

    auto ptr = chatd::message::create();

    ASSERT_EQ(bl1->size<chatd::message>(), 0);
    ASSERT_EQ(bl2->size<chatd::message>(), 0);

    unset();
    bl1->insert(ptr, [](OID oid){ notify(); });
    wait();

    ASSERT_EQ(bl1->size<chatd::message>(), 1);
    ASSERT_EQ(bl2->size<chatd::message>(), 1);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
