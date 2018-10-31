#ifndef _WEBSOCKET_H
#define _WEBSOCKET_H

#include <Arduino.h>
#include <Stream.h>
#include <WiFiMulti.h>
#include "sha1.h"
#include "Base64.h"

// CRLF characters to terminate lines/handshakes in headers.
#define CRLF "\r\n"

// Amount of time (in ms) a user may be connected before getting disconnected
// for timing out (i.e. not sending any data to the server).
#define TIMEOUT_IN_MS 10000

// ACTION_SPACE is how many actions are allowed in a program. Defaults to
// 5 unless overwritten by user.
#ifndef CALLBACK_FUNCTIONS
#define CALLBACK_FUNCTIONS 1
#endif

// Don't allow the client to send big frames of data. This will flood the Arduinos
// memory and might even crash it.
#ifndef MAX_FRAME_LENGTH
#define MAX_FRAME_LENGTH 256
#endif

#define SIZE(array) (sizeof(array) / sizeof(*array))

// WebSocket protocol constants
// First byte
#define WS_FIN            0x80
#define WS_OPCODE_TEXT    0x01
#define WS_OPCODE_BINARY  0x02
#define WS_OPCODE_CLOSE   0x08
#define WS_OPCODE_PING    0x09
#define WS_OPCODE_PONG    0x0a
// Second byte
#define WS_MASK           0x80
//#define WS_MASK           0x00
#define WS_SIZE16         126
#define WS_SIZE64         127


class WebSocketClient {
public:

    // Handle connection requests to validate and process/refuse
    // connections.
    bool handshake(WiFiClient &client);

    // Get data off of the stream
    bool getData(String& data, uint8_t *opcode = NULL);
    bool getData(char *data, uint8_t *opcode = NULL);

    // Write data to the stream
    void sendData(const char *str, uint8_t opcode = WS_OPCODE_TEXT, bool fast = true);
    void sendData(String str, uint8_t opcode = WS_OPCODE_TEXT, bool fast = true);

    char *path;
    char *host;
    char *protocol;

private:
    Client *socket_client;
    unsigned long _startMillis;

    const char *socket_urlPrefix;

    // Discovers if the client's header is requesting an upgrade to a
    // websocket connection.
    bool analyzeRequest();

    bool handleStream(String& data, uint8_t *opcode);
    bool handleStream(char *data, uint8_t *opcode);

    // Disconnect user gracefully.
    void disconnectStream();

    int timedRead();

    void sendEncodedData(char *str, uint8_t opcode);
    void sendEncodedData(String str, uint8_t opcode);

    void sendEncodedDataFast(char *str, uint8_t opcode);
    void sendEncodedDataFast(String str, uint8_t opcode);
};

#endif
