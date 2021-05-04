#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#define TIMETYPE curl_off_t

typedef struct myprogress{
	TIMETYPE lastruntime;
	CURL *curl;
} myprogress;

typedef struct downloadbuffer{
	size_t len;
	char *buff;
} downloadbuffer;

int progress_callback(void *clientp,   curl_off_t dltotal,   curl_off_t dlnow,   curl_off_t ultotal,   curl_off_t ulnow){
	if (dltotal != 0) {
		fprintf(stderr, "\rReading in progress, %ld bytes out of %ld", dlnow, dltotal);
		if (dlnow != 0) {
			int percentage = (dlnow / dltotal) * 100;
			for (int i = 0; i < percentage; i = i + 5){
				fprintf(stderr, "|");
			}
		}
	}
	
	if (dltotal == dlnow && dltotal != 0) {
		fprintf(stderr, "\rDone reading data.                                              ");
	}
	
	return 0;
}

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	downloadbuffer *buffer = userdata;
	buffer->buff = realloc(buffer->buff, buffer->len + nmemb);
	memcpy(buffer->buff + buffer->len, ptr, nmemb);
	buffer->len += nmemb;
	fprintf(stderr, "Successfully done");
	return nmemb;
}

char *download_url(const char *url, size_t *len) {
	CURL *curl = curl_easy_init();
	CURLcode res = CURLE_OK;
	myprogress prog;
	downloadbuffer data = {0, NULL};
	if (curl) {
		prog.lastruntime = 0;
		prog.curl = curl;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, prog);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
		
		fprintf(stderr, "\n");
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "%s\n", curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);
	}
	fprintf(stderr, "\n");
	
	*len = data.len;
	data.buff = realloc(data.buff, data.len + 1);
	data.buff[data.len] = 0;
	
	return data.buff;
}

int main(int argc, char *argv[]) {
	size_t len = 0;
	printf("%s", download_url(argv[1], &len));
	return 0;
}
