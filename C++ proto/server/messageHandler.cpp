#include "messageHandler.h"

MessageHandler::MessageHandler() {}

/**
 * Interprets received message and updates accompanying device
 */
void MessageHandler::handleMessage(Device* d, string message) {
	sscanf(token, "%s : %s", key, val);
	d->update(val);
}
