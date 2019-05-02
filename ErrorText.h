#pragma once

#define ERROR_NO_ERROR ("Everything is all right.")
#define ERROR_TOKEN_INVALID(token) ("The token \"" + string(token) + "\" is invalid.")
#define ERROR_TOKEN_REDEFINE(token) ("The token \"" + string(token) + "\" is redefined.")
#define ERROR_TOKEN_UNDEFINE(token) ("The token \"" + string(token) + "\" is undefined.")
#define ERROR_COMMAND_NOT_FOUND(name) ("Command not found: " + string(name) + ".")
#define ERROR_PARA_COUNT_MISMATCH(name, count) (string(name) + ": This command requires "#count" parameter(s).")
#define ERROR_SUB_PROCESS_INVALID ("Invalid sub process.")
#define ERROR_MISSING_LEFT_CURLY_BRACKET ("Missing \"{\".")
#define ERROR_MISSING_RIGHT_CURLY_BRACKET ("Missing \"}\".")
