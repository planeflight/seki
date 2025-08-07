#include "image_download.hpp"

#include <curl/curl.h>
#include <raylib.h>

#include <iostream>

CURL *image = nullptr;

void init_downloader() {
    image = curl_easy_init();
    if (!image) std::cerr << "Error initializing libcurl.\n";
}

// https://stackoverflow.com/questions/6645550/how-to-save-image-using-libcurl
void download_image(const std::string &source,
                    const std::string &save_location) {
    CURLcode error_code;
    FILE *fp;

    image = curl_easy_init();
    if (image) {
        // Open file
        fp = fopen(save_location.c_str(), "wb");
        if (!fp) std::cerr << "File cannot be opened.\n";

        curl_easy_setopt(image, CURLOPT_URL, source.c_str());
        curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(image, CURLOPT_WRITEDATA, fp);
        // Grab image
        error_code = curl_easy_perform(image);
        if (error_code) {
            std::cerr << "Cannot grab the image!\n";
        }
    }

    // Close the file
    fclose(fp);
}

void quit_downloader() {
    if (image) curl_easy_cleanup(image);
}

Texture load_texture(const std::string &url) {
    const std::string local = SERVER_LOCATION + url;
    download_image(url, local);
    Texture texture = LoadTexture(local.c_str());
    return texture;
}
