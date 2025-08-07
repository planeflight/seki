#include "image_download.hpp"

#include <curl/curl.h>
#include <raylib.h>

#include <iostream>
#include <string>
#include <unordered_map>

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
    static int count = 0;
    // track where they are on disk and in RAM
    static std::unordered_map<std::string, std::pair<int, Texture>> url_map;

    // if already loaded for whatever reason, return the texture
    if (url_map.contains(url)) return url_map[url].second;

    // otherwise, download image onto local server
    const std::string local = SERVER_LOCATION + std::to_string(count) + ".jpg";
    download_image(url, local);

    // create Raylib texture for rendering, and save it
    Texture texture = LoadTexture(local.c_str());
    url_map[url] = {count, texture};

    count++;
    return texture;
}
