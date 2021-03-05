#include "../include/Common.hpp"

/**
 * @brief Load file and get its bytes binary.
 * @param outBytes String to save bytes binary of the image file.
 * @param filePath Path of image file. Extension suffix should be included.
 */
void getFileBytes (std::string& outBytes, const std::string filePath) {
    std::ifstream bin(filePath, std::ios::binary);
    outBytes.clear();
    outBytes = std::string((std::istreambuf_iterator<char>(bin)), std::istreambuf_iterator<char>());
}