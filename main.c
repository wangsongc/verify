#include <stdio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int main()
{
	unsigned long a;
	int r;

	SSL_library_init();
	ERR_load_crypto_strings();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();

	a = SSLeay();
	r = (a == OPENSSL_VERSION_NUMBER);
	printf("SSLeay = %1x\n", a);
	printf("OPENSSL_VERSION_NUMBER = %1x\n", OPENSSL_VERSION_NUMBER);

	printf("result--->[%s]\n", r ? "SUCCESS" : "FAIL");
	return !r; 
}
