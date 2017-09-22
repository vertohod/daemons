#!/usr/bin/env python

import websocket
import thread
import time

request_create_user = "{\"id\":1,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"create_user\",\"vr\":1,\"nickname\":\"test\",\"password\":\"asdf\"}}"
request_login = "{\"id\":2,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"login\",\"vr\":1,\"nickname\":\"test\",\"password\":\"asdf\"}}"
request_create_channel3 = "{\"id\":3,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"create_channel\",\"vr\":1,\"name\":\"abcdefghajk\",\"description\":\"\"}}"
request_create_channel4 = "{\"id\":4,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"create_channel\",\"vr\":1,\"name\":\"abcdefhajk\",\"description\":\"\"}}"
request_create_channel5 = "{\"id\":5,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"create_channel\",\"vr\":1,\"name\":\"abcdeghajk\",\"description\":\"\"}}"
request_create_channel6 = "{\"id\":6,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"create_channel\",\"vr\":1,\"name\":\"abcdfghajk\",\"description\":\"\"}}"
request_create_channel7 = "{\"id\":7,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"create_channel\",\"vr\":1,\"name\":\"abcefghajk\",\"description\":\"\"}}"
request_look_for_channel1 = "{\"id\":1,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"look_for_channel\",\"vr\":1,\"text\":\"abc\"}}"
request_look_for_channel2 = "{\"id\":2,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"look_for_channel\",\"vr\":1,\"text\":\"abcd\"}}"
request_look_for_channel3 = "{\"id\":3,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"look_for_channel\",\"vr\":1,\"text\":\"abcde\"}}"
request_look_for_channel4 = "{\"id\":4,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"look_for_channel\",\"vr\":1,\"text\":\"abcdef\"}}"
request_look_for_channel5 = "{\"id\":5,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"look_for_channel\",\"vr\":1,\"text\":\"abcdefg\"}}"
request_look_for_channel6 = "{\"id\":6,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"look_for_channel\",\"vr\":1,\"text\":\"abcdefgh\"}}"
request_look_for_channel7 = "{\"id\":6,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"look_for_channel\",\"vr\":1,\"text\":\"abcdefghajk\"}}"
request_logout = "{\"id\":8,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"logout\",\"vr\":1}}"

def send(ws, message):
    print "Request:"
    print message
    ws.send(message)
    time.sleep(1)

def on_message(ws, message):
    print "Response:"
    print message

def on_error(ws, error):
    print error

def on_close(ws):
    print "### closed ###"

def on_open(ws):
    def run(*args):
        send(ws, request_create_user)
        send(ws, request_login)
        send(ws, request_create_channel3)
        send(ws, request_create_channel4)
        send(ws, request_create_channel5)
        send(ws, request_create_channel6)
        send(ws, request_create_channel7)
        send(ws, request_look_for_channel1)
        send(ws, request_look_for_channel2)
        send(ws, request_look_for_channel3)
        send(ws, request_look_for_channel4)
        send(ws, request_look_for_channel5)
        send(ws, request_look_for_channel6)
        send(ws, request_look_for_channel7)
        send(ws, request_logout)
        ws.close()
        print "thread terminating..."
    thread.start_new_thread(run, ())


if __name__ == "__main__":
    websocket.enableTrace(False)
    ws = websocket.WebSocketApp("wss://friendsoft.ru/ws/",
        on_message = on_message,
        on_error = on_error,
        on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()
