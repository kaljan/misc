#include "buffer.h"
#include "str.h"

extern void sha256(const uint8_t *data, size_t size, uint8_t *md);

#define BUFFER_SIZE	2048

buffer_t * buffer = NULL;

uint8_t temp_data[129];
uint8_t out_data[65];
uint8_t in_data_sha[32];
char in_sha_str[65];
uint8_t out_data_sha[32];
char out_sha_str[65];
uint8_t buf_data_sha[32];
char buf_sha_str[65];

int create_data_buffer(void) {
	ssize_t ret = -1;
	size_t size;
	size = randint(1024, 4096);
	printf("[%s:%d] Creating data buffer \n", __func__, __LINE__);

	if (!buffer && !(buffer = create_buffer(size))) {
		printf(D_FAILED "\n");
		return -1;
	}

	printf("[%s:%d] expected size: %lu \n", __func__, __LINE__, size);
	printf("[%s:%d] actual size  : %lu \n", __func__, __LINE__, buffer->size);

	printf("[%s:%d] Fill data buffer \n", __func__, __LINE__);
	while (1) {

		printf("[%s:%d] bytes: %lu\n", __func__, __LINE__, buffer->bytes);

		size = (size_t)randint(32, 128);
		printf("[%s:%d] size : %lu\n", __func__, __LINE__, size);

		frandstr((char *)temp_data, size);
		printf("[%s:%d] data : %s\n", __func__, __LINE__, (char *)temp_data);

		ret = write_buffer(buffer, temp_data, size);
		printf("[%s:%d] ret  : %li\n\n", __func__, __LINE__, ret);

		if (ret > 0) {
			//			printf("[%s:%d] Add to SHA-256\n", __func__, __LINE__);
			sha256(temp_data, ret, NULL);
			//			printf("[%s:%d] Added  SHA-256\n\n", __func__, __LINE__);
			continue;
		}
		if (ret < 0) {
			break;
		}
		printf("[%s:%d] Calculate SHA-256\n", __func__, __LINE__);
		sha256(temp_data, ret, in_data_sha);
		fstrdata(in_sha_str, in_data_sha, 32);

		sha256(buffer->data, buffer->size, buf_data_sha);
		fstrdata(buf_sha_str, buf_data_sha, 32);

		printf("[%s:%d] in  SHA256 sum: %s \n", __func__, __LINE__, in_sha_str);
		printf("[%s:%d] buf SHA256 sum: %s \n", __func__, __LINE__, buf_sha_str);
		break;
	}

	return (int)ret;
}

int read_data_buffer(void) {
	if (buffer) {
		ssize_t ret = -1;

		printf("[%s:%d] Reading data from buffer\n", __func__, __LINE__);

		while (1) {
			printf("[%s:%d] bytes: %lu\n", __func__, __LINE__, buffer->bytes);
			ret = read_buffer(buffer, out_data, 64);

			if (ret > 0) {
				out_data[ret] = 0;
				printf("[%s:%d] read : %li\n", __func__, __LINE__, ret);
				printf("[%s:%d] data : %s\n\n", __func__, __LINE__, out_data);
				sha256(out_data, ret, NULL);
				continue;
			}
			if (ret < 0) {
				return (int)ret;
			}


			printf("[%s:%d] Calculate SHA-256\n", __func__, __LINE__);
			sha256(out_data, ret, out_data_sha);
			fstrdata(out_sha_str, out_data_sha, 32);
			if (memcmp(in_sha_str, out_sha_str, 32) != 0) {
//				printf(FG_BOLD_RED "[%s:%d] out SHA256 sum: %s " C_ENDLINE, __func__, __LINE__, in_sha_str);
//				printf(FG_BOLD_RED "[%s:%d] out SHA256 sum: %s " C_ENDLINE, __func__, __LINE__, out_sha_str);
			}
//			printf(FG_CYAN "[%s:%d] out SHA256 sum: %s " C_ENDLINE, __func__, __LINE__, in_sha_str);
//			printf(FG_CYAN "[%s:%d] out SHA256 sum: %s " C_ENDLINE, __func__, __LINE__, out_sha_str);

			break;
		}
	}
	return -1;
}

int test_buffer(void) {
	initrand();
	create_data_buffer();
	read_data_buffer();
	return 0;
}
