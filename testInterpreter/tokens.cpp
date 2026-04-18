#include "tokens.h"

Token::Token(string val, TokenType t) {
    value = val;
    type = t;
}
Token::Token() {
    value = "";
    type = ERROR;
}

Expression::Expression(Token left, Token right, Token op) {
    if (op.value == "+") {
        if (left.type == NUMBER and right.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string(std::stoi(left.value) + std::stoi(right.value)), NUMBER);
                return;
            }
            else {
                result = Token(std::to_string(std::stof(left.value) + std::stof(right.value)), NUMBER);
            }

        }
        else if (left.type == STRING or right.type == STRING) {
            std::stringstream ss;
            ss << left.value << right.value;
            result = Token(ss.str(), STRING);
        }

        return;
    }

    if (op.value == "-") {
        if (left.type == NUMBER and right.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string(std::stoi(left.value) - std::stoi(right.value)), NUMBER);
                return;
            }
            else {
                result = Token(std::to_string(std::stof(left.value) - std::stof(right.value)), NUMBER);
            }
        }
        return;
    }

    if (op.value == "*") {
        if (left.type == NUMBER and right.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string(std::stoi(left.value) * std::stoi(right.value)), NUMBER);
                return;
            }
            else {
                result = Token(std::to_string(std::stof(left.value) * std::stof(right.value)), NUMBER);
            }
        }
        return;
    }

    if (op.value == "/") {
        if (left.type == NUMBER and right.type == NUMBER) {
            result = Token(std::to_string(stof(left.value) / stof(right.value)), NUMBER);
            return;
        }
        return;
    }

    if (op.value == "//") {
        if (left.type == NUMBER and right.type == NUMBER) {
            int rf = static_cast<int>(std::floor(static_cast<float>(stof(left.value) / stof(right.value))));

            result = Token(std::to_string(rf), NUMBER);
            return;
        }
        return;
    }

    if (op.value == "**") {
        if (left.type == NUMBER and right.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string(std::pow(std::stoi(left.value), std::stoi(right.value))), NUMBER);
                return;
            }
            else {
                result = Token(std::to_string(std::pow(std::stof(left.value), std::stof(right.value))), NUMBER);
            }
        }
        return;
    }
    stringstream s;
    s << "Error in arithmatic operation: " << left.type << " " << op.value << " " << right.type;
    throw std::runtime_error(s.str());
    return;
}

LogicExpression::LogicExpression(Token left, Token right, Token op) {
    if (op.value == "==") {
        if (left.type == right.type) {
            result = Token(std::to_string((left.value == right.value)), BOOL);
        }
        else {
            result = Token("0", BOOL);

        }
        return;
    }
    if (op.value == "<=") {
        if (left.type == right.type and left.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string((stoi(left.value) <= stoi(right.value))), BOOL);
            }
            else {
                result = Token(std::to_string((stof(left.value) <= stof(right.value))), BOOL);
            }

        }
        else {
            result = Token("0", BOOL);
        }
        return;
    }
    if (op.value == ">=") {
        if (left.type == right.type and left.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string((stoi(left.value) >= stoi(right.value))), BOOL);
            }
            else {
                result = Token(std::to_string((stof(left.value) >= stof(right.value))), BOOL);
            }
        }
        else {
            result = Token("0", BOOL);
        }
        return;
    }
    if (op.value == ">") {
        if (left.type == right.type and left.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string((stoi(left.value) > stoi(right.value))), BOOL);
            }
            else {
                result = Token(std::to_string((stof(left.value) > stof(right.value))), BOOL);
            }
        }
        else {
            result = Token("0", BOOL);
        }
        return;
    }
    if (op.value == "<") {
        if (left.type == right.type and left.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string((stoi(left.value) < stoi(right.value))), BOOL);
            }
            else {
                result = Token(std::to_string((stof(left.value) < stof(right.value))), BOOL);
            }
        }
        else {
            result = Token("0", BOOL);
        }
        return;
    }
    if (op.value == "!=") {
        if (left.type == right.type) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string((stoi(left.value) != stoi(right.value))), BOOL);
            }
            else {
                result = Token(std::to_string((stof(left.value) != stof(right.value))), BOOL);
            }
        }
        else {
            result = Token("1", BOOL);
        }
        return;
    }
    if (op.value == "and") {
        if (left.type == right.type and left.type == BOOL) {
            result = Token(std::to_string(((left.value == right.value) and (right.value == "1" and left.value == "1"))), BOOL);
            return;
        }
    }
    if (op.value == "or") {
        if (left.type == right.type and left.type == BOOL) {
            result = Token(std::to_string((left.value == "1" or right.value == "1")), BOOL);
            return;
        }
    }
    stringstream s;
    s << "Error in logic operation: " << left.type << " " << op.value << " " << right.type;
    throw std::runtime_error(s.str());
    return;
}
