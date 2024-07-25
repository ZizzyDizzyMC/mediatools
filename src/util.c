#include <libavcodec/avcodec.h>
#include "util.h"

static int valid_demuxer(const AVInputFormat *fmt)
{
    // apng:      animated PNG
    // png_pipe:  static PNG
    // image2:    JPEG
    // jpeg_pipe: JPEG
    // gif:       GIF
    // svg_pipe:  SVG (recommended not to use this currently)
    // matroska:  MKV/WebM

    return
      fmt == av_find_input_format("apng")      ||
      fmt == av_find_input_format("png_pipe")  ||
      fmt == av_find_input_format("image2")    ||
      fmt == av_find_input_format("jpeg_pipe") ||
      fmt == av_find_input_format("gif")       ||
      fmt == av_find_input_format("svg_pipe")  ||
      fmt == av_find_input_format("matroska");
}

static const AVInputFormat *image2_demuxer()
{
    return av_find_input_format("image2");
}

int open_input_correct_demuxer(AVFormatContext **ctx, const char *filename)
{
    if (avformat_open_input(ctx, filename, NULL, NULL) < 0) {
        return avformat_open_input(ctx, filename, image2_demuxer(), NULL);
    }

    // Should usually happen
    if (valid_demuxer((*ctx)->iformat)) {
        return 0;
    }

    // Wrong demuxer, force to image2 so we don't error out here
    avformat_close_input(ctx);
    return avformat_open_input(ctx, filename, image2_demuxer(), NULL);
}
