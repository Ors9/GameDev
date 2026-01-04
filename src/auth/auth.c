#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <auth/auth.h>
#include "sodium.h"

bool HashedPassword(const char *password, char hashedPassword[])
{

    int r = crypto_pwhash_str(hashedPassword, password, strlen(password), crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE);
    if (r == -1 || r != 0)
    {
        fprintf(stderr, "crypto_pwash_str failed at HashedPassword\n");
        return false;
    }

    return true;
}

static bool IsComplexityValid(const char *input)
{
    if (input == NULL || input[0] == '\0')
        return false;

    bool foundChar = false;
    bool foundDigit = false;

    for (int i = 0; input[i] != '\0'; i++)
    {

        if (!isalnum((unsigned char)input[i]))
        {
            return false;
        }

        if (isalpha((unsigned char)input[i]))
            foundChar = true;
        if (isdigit((unsigned char)input[i]))
            foundDigit = true;
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