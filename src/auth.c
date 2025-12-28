#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <auth.h>

static bool IsComplexityValid(const char *input)
{
    if (input == NULL)
        return false;
    bool foundChar = false;
    bool foundDigit = false;

    for (int i = 0; input[i] != '\0'; i++)
    {
        if (isalpha(input[i]))
            foundChar = true;
        if (isdigit(input[i]))
            foundDigit = true;

        if (foundChar && foundDigit)
            return true;
    }
    return (foundChar && foundDigit);
}

static AUTH_STATUS RegisterNameRules(char *name)
{
    if (name == NULL || strlen(name) == 0)
        return NAME_EMPTY;
    if (strlen(name) < 4)
        return NAME_TOO_SHORT;
    if (strlen(name) > 10)
        return NAME_TOO_LONG;

    if (!IsComplexityValid(name))
        return NAME_INVALID_COMPLEXITY;

    return AUTH_SUCCESS;
}

static AUTH_STATUS CheckIfPasswordValid(char *pass, char *passCheck)
{
    if (pass == NULL || passCheck == NULL)
        return PASS_EMPTY;

    size_t len = strlen(pass);
    if (len < 4)
        return PASS_TOO_SHORT;
    if (len > 10)
        return PASS_TOO_LONG;

    if (strcmp(pass, passCheck) != 0)
        return PASS_MISMATCH;

    if (!IsComplexityValid(pass))
        return PASS_INVALID_COMPLEXITY;

    return AUTH_SUCCESS;
}

AUTH_STATUS HandleRegisterRules(char *name, char *pass, char *passCheck)
{
    AUTH_STATUS nameStatus = RegisterNameRules(name);
    if (nameStatus != AUTH_SUCCESS)
    {
        return nameStatus;
    }

    AUTH_STATUS passStatus = CheckIfPasswordValid(pass, passCheck);
    if (passStatus != AUTH_SUCCESS)
    {
        return passStatus;
    }

    return AUTH_SUCCESS;
}