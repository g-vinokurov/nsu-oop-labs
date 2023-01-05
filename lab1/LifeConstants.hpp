#pragma once

#include <string>

namespace life {
    const std::string LIFE_FILE_FORMAT = "#Life 1.06";
    const std::string UNIVERSE_NAME_PREFIX = "#N ";
    const std::string UNIVERSE_RULE_PREFIX = "#R ";
    const std::string BIRTH_RULE_PREFIX = "B";
    const std::string SURVIVAL_RULE_PREFIX = "S";
    const char FORBIDDEN_NEIGHBOURS_NUMBER = '9';
}
