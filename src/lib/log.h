#pragma once

#include "Arduino.h"

enum class LogLevel { INFO, SUCCESS, WARNING, ERROR };

void log(LogLevel level, const char *message);