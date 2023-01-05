#include "LifeFileParser.hpp"

life::LifeConfig life::LifeFileParser::parse(std::string const & filename, std::ostream & err) {
    std::ifstream file(filename);
    if (not file.is_open())
        throw life::LifeParserError("Couldn't open the file \"" + filename + "\"");
    std::string line;

    std::getline(file, line);
    if (utils::shrink(line) != life::LIFE_FILE_FORMAT)
        throw life::LifeParserError("Unsupported life-file format: " + line);

    life::LifeConfig config;

    std::getline(file, line);
    bool file_has_universe_name = utils::starts_with(line, life::UNIVERSE_NAME_PREFIX);
    if (file_has_universe_name)
        config.set_universe_name(line.erase(0, life::UNIVERSE_NAME_PREFIX.length()));
    else
        err << "There is no universe name. The default universe name was set" << std::endl;
    if (file_has_universe_name)
        std::getline(file, line);

    bool file_has_transition_rule = utils::starts_with(line, life::UNIVERSE_RULE_PREFIX);
    if (file_has_transition_rule)
        parse_rule(line.erase(0, life::UNIVERSE_RULE_PREFIX.length()), config, err);
    else
        err << "There is no transition rule. The default rule was set" << std::endl;
    if (file_has_transition_rule)
        std::getline(file, line);

    parse_field_size(line, config);
    parse_cells(file, config, err);
    return config;
}

void life::LifeFileParser::parse_rule(std::string const & rule, life::LifeConfig & config, std::ostream & err) {
    std::vector<std::string> bs = utils::split(utils::shrink(rule), '/');
    if (bs.size() != 2) {
        err << "Couldn't define transition rule from string \"" << rule << "\"" << std::endl;
        return;
    }
    if (not utils::starts_with(bs[0], life::BIRTH_RULE_PREFIX)) {
        err << "Couldn't define birth rule from string \"" << bs[0] << "\"" << std::endl;
        return;
    }
    if (not utils::starts_with(bs[1], life::SURVIVAL_RULE_PREFIX)) {
        err << "Couldn't define survival rule from string \"" << bs[1] << "\"" << std::endl;
        return;
    }
    size_t b_rule_len = bs[0].length();
    size_t s_rule_len = bs[1].length();
    life::LifeConfig::TrRule b_rule, s_rule;
    for (size_t i = life::BIRTH_RULE_PREFIX.length(); i != b_rule_len; ++i) {
        if (not std::isdigit(bs[0][i]) or bs[0][i] == life::FORBIDDEN_NEIGHBOURS_NUMBER) {
            err << "Couldn't define birth rule from string \"" << bs[0] << "\"" << std::endl;
            return;
        } else
            b_rule[utils::digit_to_int(bs[0][i])] = true;
    }
    for (size_t i = life::SURVIVAL_RULE_PREFIX.length(); i != s_rule_len; ++i) {
        if (not std::isdigit(bs[1][i]) or bs[1][i] == life::FORBIDDEN_NEIGHBOURS_NUMBER) {
            err << "Couldn't define survival rule from string \"" << bs[1] << "\"" << std::endl;
            return;
        } else
            s_rule[utils::digit_to_int(bs[1][i])] = true;
    }
    config.set_birth_rule(b_rule);
    config.set_survival_rule(s_rule);
}

void life::LifeFileParser::parse_field_size(std::string const & line, life::LifeConfig & config) {
    std::pair<long long, long long> field_size;
    try {
        field_size = life::LifeFileParser::parse_xy_value(utils::shrink(line));
    } catch (life::LifeParserError & e) {
        throw life::LifeParserError("Couldn't define field size from string \"" + line + "\"");
    }
    if (field_size.first <= 0 or field_size.second <= 0)
        throw life::LifeParserError("Field sizes must be positive");
    config.set_field_width(static_cast<unsigned long long>(field_size.first));
    config.set_field_height(static_cast<unsigned long long>(field_size.second));
}

void life::LifeFileParser::parse_cells(std::ifstream & file, life::LifeConfig & config, std::ostream & err) {
    std::string line;
    while (not file.eof() && not file.fail()) {
        std::getline(file, line);
        std::pair<long long, long long> xy;
        try {
            xy = life::LifeFileParser::parse_xy_value(utils::shrink(line));
        } catch (life::LifeParserError & e) {
            throw life::LifeParserError("Couldn't define coordinates from string \"" + line + "\"");
        }
        size_t old_set_size = config.get_live_cells_count();
        config.insert_live_cell(xy.first, xy.second);
        size_t new_set_size = config.get_live_cells_count();
        if (old_set_size == new_set_size)
            err << "The cell (" << xy.first << ", " << xy.second << ") has already live" << std::endl;
    }
}

std::pair<long long, long long> life::LifeFileParser::parse_xy_value(std::string const & line) {
    std::vector<std::string> split_line = utils::split(utils::shrink(line), ' ');
    if (split_line.size() != 2)
        throw life::LifeParserError("Couldn't split (x, y)-value");

    size_t x_char_processed = 0, y_char_processed = 0;
    long long x = 0, y = 0;
    try {
        x = std::stoll(split_line[0], &x_char_processed);
        y = std::stoll(split_line[1], &y_char_processed);
    } catch (...) {
        throw life::LifeParserError("Couldn't make integer values for (x, y)-value");
    }
    if (x_char_processed != split_line[0].length() or y_char_processed != split_line[1].length())
        throw life::LifeParserError("Couldn't make integer values for (x, y)-value");
    return std::make_pair(x, y);
}
