#pragma once

#include "Arduino.h"

enum LogLevel { LOG_INFO, LOG_SUCCESS, LOG_WARNING, LOG_ERROR };

void log(LogLevel level, const char *message);