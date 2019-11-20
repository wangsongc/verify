#include <stdio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>

#define EVENT__HAVE_NANOSLEEP 1

void
evutil_usleep_(const struct timeval *tv)
{
	if (!tv)
		return;
#if defined(_WIN32)
	{
		long msec = evutil_tv_to_msec_(tv);
		Sleep((DWORD)msec);
	}
#elif defined(EVENT__HAVE_NANOSLEEP)
	{
		struct timespec ts;
		ts.tv_sec = tv->tv_sec;
		ts.tv_nsec = tv->tv_usec*1000;
		clock_nanosleep(CLOCK_REALTIME, 0, &ts, NULL);
	}
#elif defined(EVENT__HAVE_USLEEP)
	/* Some systems don't like to usleep more than 999999 usec */
	sleep(tv->tv_sec);
	usleep(tv->tv_usec);
#else
	{
		struct timeval tv2 = *tv;
		select(0, NULL, NULL, NULL, &tv2);
	}
#endif
}


int main()
{
	struct timeval tv1, tv2, tv3, diff1, diff2;
	const struct timeval quarter_sec = {0, 250*1000};
	const struct timeval tenth_sec = {0, 100*1000};
	long usec1, usec2;

	gettimeofday(&tv1, NULL);
	evutil_usleep_(&quarter_sec);
	gettimeofday(&tv2, NULL);
	evutil_usleep_(&tenth_sec);
	gettimeofday(&tv3, NULL);

	timersub(&tv2, &tv1, &diff1);
	timersub(&tv3, &tv2, &diff2);

	usec1 = diff1.tv_sec * 1000000 + diff1.tv_usec;
	usec2 = diff2.tv_sec * 1000000 + diff2.tv_usec;
	printf("tv1.tv_sec=:%ld\n",tv1.tv_sec);
	printf("tv1.tv_usec=:%ld\n",tv1.tv_usec);
	printf("tv2.tv_sec=:%ld\n",tv2.tv_sec);
	printf("tv2.tv_usec=:%ld\n",tv2.tv_usec);
	printf("tv3.tv_sec=:%ld\n",tv3.tv_sec);
	printf("tv3.tv_usec=:%ld\n",tv3.tv_usec);

	assert(usec1 > 200000 && usec1 < 300000);
	assert(usec2 > 80000 && usec2 < 120000);
}

// int main()
// {
// 	unsigned long a;
// 	int r;

// 	SSL_library_init();
// 	ERR_load_crypto_strings();
// 	SSL_load_error_strings();
// 	OpenSSL_add_all_algorithms();

// 	a = SSLeay();
// 	r = (a == OPENSSL_VERSION_NUMBER);
// 	printf("SSLeay = %1x\n", a);
// 	printf("OPENSSL_VERSION_NUMBER = %1x\n", OPENSSL_VERSION_NUMBER);

// 	printf("result--->[%s]\n", r ? "SUCCESS" : "FAIL");
// 	return !r; 
// }
