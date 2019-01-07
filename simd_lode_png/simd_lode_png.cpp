// simd_lode_png.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "lodepng.h"
#include "libdeflate.h"



struct libdeflate_decompressor *decompressor = nullptr;

unsigned lodepng_inflate(unsigned char** out, size_t* outsize,
	const unsigned char* in, size_t insize,
	const LodePNGDecompressSettings* settings);

unsigned custom_inflate(unsigned char** out, size_t* outsize,
	const unsigned char* in, size_t insize,
	const LodePNGDecompressSettings* settings, size_t outbuffsize)
{
	/*bit pointer in the "in" data, current byte is bp >> 3, current bit is bp & 0x7 (from lsb to msb of the byte)*/
	size_t insize_available = insize;
	size_t outsize_available = outbuffsize;// 1 * 1024 * 1024;/*byte position in the out buffer*/
	auto result = libdeflate_deflate_decompress_ex(decompressor, in, insize,
		*out, outsize_available, &insize_available, outsize);
	switch (result) {
	case libdeflate_result::LIBDEFLATE_BAD_DATA: return 20;
	case libdeflate_result::LIBDEFLATE_SHORT_OUTPUT: return 52;
	case libdeflate_result::LIBDEFLATE_INSUFFICIENT_SPACE: return 52;
	default:
	case libdeflate_result::LIBDEFLATE_SUCCESS: return 0;
	}
}

unsigned custom_zlib(unsigned char** out, size_t* outsize,
	const unsigned char* in, size_t insize,
	const LodePNGDecompressSettings* settings, size_t outbuffsize)
{
	/*bit pointer in the "in" data, current byte is bp >> 3, current bit is bp & 0x7 (from lsb to msb of the byte)*/
	size_t insize_available = insize;
	size_t outsize_available = outbuffsize;// 1 * 1024 * 1024;/*byte position in the out buffer*/
	auto result = libdeflate_zlib_decompress(decompressor, in, insize,
		*out, outsize_available, &insize_available);
	switch (result) {
	case libdeflate_result::LIBDEFLATE_BAD_DATA: return 20;
	case libdeflate_result::LIBDEFLATE_SHORT_OUTPUT: return 52;
	case libdeflate_result::LIBDEFLATE_INSUFFICIENT_SPACE: return 52;
	default:
	case libdeflate_result::LIBDEFLATE_SUCCESS:
		*outsize = outbuffsize;
		return 0;
	}
}

unsigned custom_unfilterScanLine(unsigned char* recon, const unsigned char* scanline, const unsigned char* precon,
	size_t bytewidth, unsigned char filterType, size_t length)
{
	return 0;
}

unsigned custom_adler32(unsigned adler, const unsigned char* data, unsigned len)
{
	return libdeflate_adler32(adler, data, len);
}



int main(int argc, char** argv)
{
	unsigned error;
	unsigned char* image = nullptr;
	unsigned width, height;
	char* filename = "techno_x10_pngout.png";
	bool customed = true;
	if (argc > 1)
	{
		int ac = 1;
		if (std::string(argv[1]) == "-c") {
			customed = true;
			ac++;
			puts("customed");
		}
		filename = argv[ac++];
	}

	decompressor = libdeflate_alloc_decompressor();

	LodePNGDecoderSettings decoderSettings = { 0 };
	lodepng_decoder_settings_init(&decoderSettings);
	//decoderSettings.zlibsettings.custom_inflate = lodepng_inflate;
	decoderSettings.zlibsettings.custom_zlib = custom_zlib;
	//decoderSettings.zlibsettings.custom_inflate = custom_inflate;
	//decoderSettings.zlibsettings.custom_adler32 = custom_adler32;
	//	decoderSettings.zlibsettings.custom_unfilterScanLine = custom_unfilterScanLine;
	//if(customed)
		lodepng_decoder_settings_user_regist(&decoderSettings);

	//FILE *fp;

	///* ファイルオープン */
	//if ((fp = fopen(filename, "rb")) == NULL) {
	//	fprintf(stderr, "%s%s\n", "error::", "can not open image.");
	//	exit(EXIT_FAILURE);
	//}

	///* ファイルサイズの取得 */
	//fseek(fp, 0L, SEEK_END);
	//fpos_t filelen;
	////	fgetpos(fp, &(png_buf.data_len));
	//fgetpos(fp, &filelen);
	//fseek(fp, 0L, SEEK_SET);
	////printf("filesize = %ld\n", png_buf.data_len);

	///* PNGデータ用のメモリ確保 */
	//unsigned char * filedata = (unsigned char*)calloc(filelen, 1);
	///* ファイル読み込み */
	//fread(filedata, 1, filelen, fp);
	///* ファイルを閉じます */
	//fclose(fp);
	for (int i = 0; i < 50; i++) {
		error = lodepng_decode24_file(&image, &width, &height, filename);
		//error = lodepng_decode24(&image, &width, &height, filedata, filelen);
		if (error) printf("error %u: %s\n", error, lodepng_error_text(error));
		free(image);
	}

	/*use image here*/
	printf("width:%d, height:%d", width, height);

	return 0;
}

