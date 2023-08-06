#include "log.h"

void log(LogLevel level, const char *message) {
  if (Serial) {
    String prefix = "";
    switch (level) {
    case LOG_INFO:
      prefix = "[INFO]";
      break;
    case LOG_SUCCESS:
      prefix = "[SUCCESS]";
      break;
    case LOG_WARNING:
      prefix = "[WARNING]";
      break;
    case LOG_ERROR:
      prefix = "[ERROR]";
      break;
    }
    Serial.println(prefix + " : [" + millis() + "] : " + message);
  }
}