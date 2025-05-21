#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <iomanip>
#include <algorithm>

#include <openssl/sha.h>
#include <openssl/hmac.h>

#define ENCRYPT_FILE "ft_otp.key"

std::vector<uint8_t> timestampToByte(uint64_t timestamp) {
    std::vector<uint8_t> counterBytes;
    for (int i = 7; i >= 0; --i) {
        counterBytes.push_back(timestamp & 0xFF);
        timestamp >>= 8;
    }
    std::reverse( counterBytes.begin(), counterBytes.end() );
    return counterBytes;
}

void encryptInFile(std::string hexKey) {
    if (access(hexKey.c_str(), F_OK) == 0)
        std::getline(std::ifstream(hexKey), hexKey);
    if (hexKey.size() != 64)
        throw std::runtime_error("hexKey contain more or lower of 64 char");

    std::vector<uint8_t> byteKey(hexKey.size() / 2);
    for (size_t i = 0; i < hexKey.size(); i += 2)
        byteKey[i / 2] = std::stoi(hexKey.substr(i, 2), nullptr, 16);

    if (access(ENCRYPT_FILE, F_OK))
        std::remove(ENCRYPT_FILE);
    std::ofstream outputFile(ENCRYPT_FILE, std::ios::binary);

    uint8_t xorKey = 0xAA;
    for (auto& byte : byteKey) {
        byte ^= xorKey; // XOR sur chaque octet
    }

    outputFile.write(reinterpret_cast<const char*>(byteKey.data()), byteKey.size());
    std::cout << "Key is encrypted in: " << ENCRYPT_FILE << std::endl;
}

void printHex(const uint8_t* digest, size_t size) {
    for (size_t i = 0; i < size; ++i)
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    std::cout << std::dec << std::endl;
}

std::vector<uint8_t> computeHmac(std::vector<uint8_t>& key, std::vector<uint8_t>& message) {
    const size_t blockSize = 64;
    std::vector<uint8_t> K = key;

    if (K.size() > blockSize) {
        uint8_t digest[SHA_DIGEST_LENGTH];
        SHA1(K.data(), K.size(), digest);
        K = std::vector<uint8_t>(digest, digest + SHA_DIGEST_LENGTH);
    }
    if (K.size() < blockSize)
        K.resize(blockSize, 0x00);

    std::vector<uint8_t> K_ipad(blockSize), K_opad(blockSize);
    for (size_t i = 0; i < blockSize; ++i) {
        K_ipad[i] = K[i] ^ 0x36;
        K_opad[i] = K[i] ^ 0x5C;
    }

    K_ipad.insert(K_ipad.end(), message.begin(), message.end());
    uint8_t inner[SHA_DIGEST_LENGTH];
    SHA1(K_ipad.data(), K_ipad.size(), inner);
    std::vector<uint8_t> innerHash(inner, inner + sizeof(inner));

    K_opad.insert(K_opad.end(), innerHash.begin(), innerHash.end());
    uint8_t shaKey[SHA_DIGEST_LENGTH];
    SHA1(K_opad.data(), K_opad.size(), shaKey);
    return (std::vector<uint8_t>(shaKey, shaKey + sizeof(shaKey)));
}

void generate_TOTP(std::string keyFile) {
    std::ifstream inputFile(keyFile, std::ios::binary);
    if (!inputFile)
    throw std::runtime_error("Failed to open " + keyFile);
    
    std::vector<uint8_t> key((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>()); // Lis byte par byte dans le fichier pour le mettre dans le vector

    uint8_t xorKey = 0xAA;
    for (auto& byte : key) {
        byte ^= xorKey;  // Appliquer le même XOR pour récupérer les données originales
    }

    // uint8_t digest[SHA_DIGEST_LENGTH];
    // uint8_t line[6] = "abcde";
    // SHA1(line, 5, digest);
    // std::cout << digest << std::endl;
    // for (uint8_t byte : digest)
    //     std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    // std::cout << std::dec << std::endl;

    if (key.size() != 32)
        throw std::runtime_error("Not valid key");

    // message = Time Based code / 8 octet
    std::vector<uint8_t> message = timestampToByte(std::time(0) / 30);
    std::vector<uint8_t> HmacBytes = computeHmac(key, message);
    
    int offset = HmacBytes[19] & 0x0F;
    uint32_t binCode = (HmacBytes[offset] & 0x7F) << 24 |
                        (HmacBytes[offset + 1] & 0xFF) << 16 |
                        (HmacBytes[offset + 2] & 0xFF) << 8 |
                        (HmacBytes[offset + 3] & 0xFF);
    uint32_t code = binCode % 1000000;
    std::cout << "RESULT : " << code << std::endl;
}

void parser(int ac, char** av, std::vector<std::string>* args) {
    if (ac != 3)
        throw std::runtime_error("Wrong number of arg");
    else if (std::string(av[1]) != "-g" && std::string(av[1]) != "-k")
        throw std::runtime_error("Wrong flag in arg (-g or -k)");
    else if (std::string(av[1]) == "-k" && access(av[2], F_OK) != 0)
        throw std::runtime_error("File not found");
    args->push_back(std::string(av[1]));
    args->push_back(std::string(av[2]));
}

int main(int argc, char** argv) {
    try {
        std::vector<std::string> args;
        parser(argc, argv, &args);
        if (args[0] == "-g")
            encryptInFile(args[1]);
        else
            generate_TOTP(args[1]);
    } catch (const std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}