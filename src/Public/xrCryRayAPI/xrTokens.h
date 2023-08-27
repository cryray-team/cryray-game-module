#ifndef XRTOKEN
#define XRTOKEN

#ifdef XRCRYRAY_EXPORTS
#define XRCRYRAY_API __declspec(dllexport)
#else
#define XRCRYRAY_API __declspec(dllimport)
#endif

// token type definition
struct XRCRYRAY_API xr_token
{
    const char* name;
    int id;
};

struct XRCRYRAY_API xr_token2
{
    const char* name;
    const char* info;
    int id;
};

inline int get_token_id(xr_token* tokens, const char* key)
{
    for (int k = 0; tokens[k].name; k++)
        if (stricmp(tokens[k].name, key) == 0)
            return tokens[k].id;

    return -1;
}

inline const char* get_token_name(xr_token* tokens, int key)
{
    for (int k = 0; tokens[k].name; k++)
        if (key == tokens[k].id)
            return tokens[k].name;

    return "";
}
#endif