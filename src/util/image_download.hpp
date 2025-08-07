#ifndef UTIL_IMAGE_DOWNLOAD_HPP
#define UTIL_IMAGE_DOWNLOAD_HPP

#include <curl/curl.h>
#include <raylib.h>

#include <string>

// libcurl wrappers to download images
void init_downloader();
void download_image(const std::string &source,
                    const std::string &save_location);
void quit_downloader();

const std::string SERVER_LOCATION = "./res/server/";

Texture load_texture(const std::string &url);

#endif // UTIL_IMAGE_DOWNLOAD_HPP
