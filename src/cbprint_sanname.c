#include"fjorge.h"

void cbprint_sanname(const char *label, const X509 *const cert) {
  int success = 0;
  unsigned char *utf8 = NULL;
    
  do {
    if(!cert) 
      break; /* failed */
        
    const GENERAL_NAMES *names = X509_get_ext_d2i(cert, NID_subject_alt_name, NULL, NULL);

    if(!names)
      break;
        
    int i = 0, count = sk_GENERAL_NAME_num(names);

    if(!count) 
      break; 

    const GENERAL_NAME *entry = NULL;
        
    for(i = 0;i < count;++i) {
      entry = sk_GENERAL_NAME_value(names, i);

      if(!entry)
        continue;
            
      if(GEN_DNS == entry->type) {
        int len1 = 0, len2 = -1;
                
        len1 = ASN1_STRING_to_UTF8(&utf8, entry->d.dNSName);

        if(utf8)
          len2 = (int)strlen((const char *)utf8);
                
        if(len1 != len2)
          fjprintf_error("  Strlen and ASN1_STRING size do not match (embedded null?): %d vs %d", len2, len1);
          
                
        /* If there's a problem with string lengths, then     */
        /* we skip the candidate and move on to the next.     */
        /* Another policy would be to fails since it probably */
        /* indicates the client is under attack.              */
        if(utf8 && len1 && len2 && (len1 == len2)) {
          fjprintf_callback("  %s: %s", label, utf8);
        
          success = 1;
        }
                
        if(utf8) {
          OPENSSL_free(utf8), utf8 = NULL;
        }
      } else {
        fjprintf_error("  Unknown GENERAL_NAME type: %d", entry->type);
      }
    }

    if(names)
      GENERAL_NAMES_free((GENERAL_NAMES *)names);
  } while (0);
    
  if(utf8)
    OPENSSL_free(utf8);
    
  if(!success)
    fjprintf_callback("  %s: <not available>", label);
   
   return;
}