#include "messageHandler.h"

MessageHandler::MessageHandler() {}

/**
 * Interprets received message and updates accompanying device
 */
void MessageHandler::handleMessage(Device* d, string message) {
    string key;
    double val;
	std::sscanf(message, "%s : %d", key, val);
	d->update(val);
}
