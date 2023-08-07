#include "log.h"

void log(LogLevel level, const char *message) {
  if (Serial) {
    String prefix = "";
    switch (level) {
    case LogLevel::INFO:
      prefix = "[INFO]";
      break;
    case LogLevel::SUCCESS:
      prefix = "[SUCCESS]";
      break;
    case LogLevel::WARNING:
      prefix = "[WARNING]";
      break;
    case LogLevel::ERROR:
      prefix = "[ERROR]";
      break;
    }
    Serial.println(prefix + " : [" + millis() + "] : " + message);
  }
}