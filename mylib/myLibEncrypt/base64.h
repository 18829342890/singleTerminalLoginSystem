#ifndef __MY_ENCRYPT_H__
#define __MY_ENCRYPT_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include "md5.h"

#define MD5_SIZE        16
#define MD5_STR_LEN     (MD5_SIZE * 2)
 
static int base64_encode(const char *in_str, int in_len, char *out_str)
{
    BIO *b64, *bio;
    BUF_MEM *bptr = NULL;
    size_t size = 0;
 
    if (in_str == NULL || out_str == NULL)
        return -1;
 
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
 
    BIO_write(bio, in_str, in_len);
    BIO_flush(bio);
 
    BIO_get_mem_ptr(bio, &bptr);
    memcpy(out_str, bptr->data, bptr->length);
    out_str[bptr->length] = '\0';
    size = bptr->length;
 
    BIO_free_all(bio);
    return size;
}
 
static int base64_decode(const char *in_str, int in_len, char *out_str)
{
    BIO *b64, *bio;
    BUF_MEM *bptr = NULL;
    int counts;
    int size = 0;
 
    if (in_str == NULL || out_str == NULL)
        return -1;
 
    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
 
    bio = BIO_new_mem_buf(in_str, in_len);
    bio = BIO_push(b64, bio);
 
    size = BIO_read(bio, out_str, in_len);
    out_str[size] = '\0';
 
    BIO_free_all(bio);
    return size;
}

static void getmd5_string(const char *dest_str, unsigned int dest_len, char *md5_str)
{
    int i;
    unsigned char md5_value[MD5_SIZE];
    MY_MD5_CTX_t md5;

    // init md5
    MD5Init(&md5);

    MD5Update(&md5, (unsigned char *)dest_str, dest_len);

    MD5Final(&md5, md5_value);

    // convert md5 value to md5 string
    for(i = 0; i < MD5_SIZE; i++)
    {
        snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
    }

    return;
}

#endif
 
// int main()
// {
// 	char instr[] = "hello";
// 	char outstr1[1024] = {0};
// 	base64_encode(instr,5,outstr1);
// 	printf("base64:%s\n",outstr1);
 
// 	char outstr2[1024] = {0};
// 	base64_decode(outstr1,strlen(outstr1),outstr2);
// 	printf("str:%s\n",outstr2);

    // const char *test_str = "18829342890@163.com";
    // Compute_string_md5(test_str, strlen(test_str), md5_str);
    // printf("[string - %s] md5 value:%s\n", test_str, md5_str);
// 	return 0;
// }
