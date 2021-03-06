#include"fjorge.h"

void auth_basic(const char *b64str) {
  assert(b64str);

  const char *const auth_head = "Authorization: Basic ";
  char *restrict aret = malloc(1 + strlen(auth_head) + strlen(b64str));

  if(!aret)
    exit_verbose("malloc", __FILE__, __LINE__);

  sprintf(aret, "%s%s", auth_head, b64str);
 
  add_header(aret);

  return;
}

#if 0
int main(void) {
  const unsigned char *userpass="admin:admin";
  const size_t alen = strlen((const char*)userpass);
  const char *b64_userpass = encode_base64(userpass, alen);

  puts(b64_userpass);

  return 0;
}
#endif
