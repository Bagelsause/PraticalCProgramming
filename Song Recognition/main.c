//Start by asking the user for a song that they would like to look up
//Should grab a sample of the song (maybe 30 seconds or the entire song)
//Fingerprint that current song
//Check the database to see if song is there
//If song exists, then speed up by 5% (may change that later) until fingerprinting does not work
//If song doesn't exist, create a new entry into the database with track name (0 clue about how to do) AUBIO
//Print out the current percentage of speed it is on, and the last one where it worked
//Output the audio to a file so the user can listen to it

#include <stdio.h>
#include <chromaprint.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#define TIMETYPE curl_off_t
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
 
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
 
//Below audio-file-decoder code from https://rodic.fr/blog/libavcodec-tutorial-decode-audio-file/
int decode_audio_file(const char* path, const int sample_rate, int16_t** data, int* size) {
 
    // initialize all muxers, demuxers and protocols for libavformat
    // (does nothing if called twice during the course of one program execution)
    av_register_all();
 
    // get format from audio file
    AVFormatContext* format = avformat_alloc_context();
    if (avformat_open_input(&format, path, NULL, NULL) != 0) {
        fprintf(stderr, "Could not open file '%s'\n", path);
        return -1;
    }
    if (avformat_find_stream_info(format, NULL) < 0) {
        fprintf(stderr, "Could not retrieve stream info from file '%s'\n", path);
        return -1;
    }
 
    // Find the index of the first audio stream
    int stream_index =- 1;
    for (int i=0; i<format->nb_streams; i++) {
        if (format->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            stream_index = i;
            break;
        }
    }
    if (stream_index == -1) {
        fprintf(stderr, "Could not retrieve audio stream from file '%s'\n", path);
        return -1;
    }
    AVStream* stream = format->streams[stream_index];
 
    // find & open codec
    AVCodecContext* codec = stream->codec;
    if (avcodec_open2(codec, avcodec_find_decoder(codec->codec_id), NULL) < 0) {
        fprintf(stderr, "Failed to open decoder for stream #%u in file '%s'\n", stream_index, path);
        return -1;
    }
 
    // prepare resampler
    struct SwrContext* swr = swr_alloc();
    av_opt_set_int(swr, "in_channel_count",  codec->channels, 0);
    av_opt_set_int(swr, "out_channel_count", 1, 0);
    av_opt_set_int(swr, "in_channel_layout",  codec->channel_layout, 0);
    av_opt_set_int(swr, "out_channel_layout", AV_CH_LAYOUT_MONO, 0);
    av_opt_set_int(swr, "in_sample_rate", codec->sample_rate, 0);
    av_opt_set_int(swr, "out_sample_rate", sample_rate, 0);
    av_opt_set_sample_fmt(swr, "in_sample_fmt",  codec->sample_fmt, 0);
    av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_DBL,  0);
    swr_init(swr);
    if (!swr_is_initialized(swr)) {
        fprintf(stderr, "Resampler has not been properly initialized\n");
        return -1;
    }
 
    // prepare to read data
    AVPacket packet;
    av_init_packet(&packet);
    AVFrame* frame = av_frame_alloc();
	if (!frame) {
		fprintf(stderr, "Error allocating the frame\n");
		return -1;
	}
 
    // iterate through frames
    *data = NULL;
    *size = 0;
    while (av_read_frame(format, &packet) >= 0) {
        // decode one frame
        int gotFrame;
        if (avcodec_decode_audio4(codec, frame, &gotFrame, &packet) < 0) {
            break;
        }
        if (!gotFrame) {
            continue;
        }
        // resample frames
        double* buffer;
        av_samples_alloc((uint8_t**) &buffer, NULL, 1, frame->nb_samples, AV_SAMPLE_FMT_DBL, 0);
        int frame_count = swr_convert(swr, (uint8_t**) &buffer, frame->nb_samples, (const uint8_t**) frame->data, frame->nb_samples);
        // append resampled frames to data
        *data = (int16_t*) realloc(*data, (*size + frame->nb_samples) * 16);
        memcpy(*data + *size, buffer, frame_count * sizeof(double));
        *size += frame_count;
    }
 
    // clean up
    av_frame_free(&frame);
    swr_free(&swr);
    avcodec_close(codec);
    avformat_free_context(format);
 
    // success
    return 0;
 
}


int main(int argc, char *argv[]){
	//Song querying/desampling here
	char songTitle[100];
	char songLocation[100];
	
	printf("\nEnter the song's location: ");
    scanf("%s", &songLocation);
	
	int sampleRate = 44100;
    int16_t* data;
    int size;
    if (decode_audio_file(songLocation, sampleRate, &data, &size) != 0) {
        return -1;
    }
	
	//Fingerprinting starts here
	
	ChromaprintContext *ctx;
	char *fp;
	
	const int sample_rate = 44100;
	const int num_channels = 2;
	
	ctx = chromaprint_new(CHROMAPRINT_ALGORITHM_DEFAULT);
	chromaprint_start(ctx, sample_rate, num_channels);
	chromaprint_feed(ctx, data, size);
		
	chromaprint_finish(ctx);
	
	chromaprint_get_fingerprint(ctx, &fp);
	printf("%s\n", fp);
	
	size_t len = 0;
	char* URL;
	
	URL = (char *) malloc(79);
	strcpy(URL, "https://api.acoustid.org/v2/lookup?client=JEwBQvSQyAI&duration=641&fingerprint=");
	
	URL = realloc(URL, 79+ sizeof(fp));
	strcat(URL, fp);
	printf("%s", download_url(URL, &len));
	free(URL);
	chromaprint_free(ctx);
}