#!/usr/bin/env python

import websocket
import thread
import time

request_ping = "{\"tp\":\"ping\",\"vr\":1,\"id\":44,\"time\":12345678}"

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
        send(ws, request_ping)
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
