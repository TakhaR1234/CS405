#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cassert>

class Cipher {
public:
    std::string encrypt(const std::string& input, const std::string& key) {
        return transform(input, key);
    }

    std::string decrypt(const std::string& input, const std::string& key) {
        return transform(input, key);  // XOR again for decryption
    }

private:
    std::string transform(const std::string& input, const std::string& key) {
        assert(!key.empty() && !input.empty());
        std::string output = input;
        size_t key_length = key.length();
        for (size_t i = 0; i < input.length(); ++i) {
            output[i] = input[i] ^ key[i % key_length];
        }
        return output;
    }
};

std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;

    if (file) {
        buffer << file.rdbuf();
        file.close();
    } else {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    return buffer.str();
}

std::string get_student_name(const std::string& text) {
    size_t pos = text.find('\n');
    return (pos != std::string::npos) ? text.substr(0, pos) : "Unknown";
}

void save_data_file(const std::string& filename, const std::string& student_name,
                    const std::string& key, const std::string& data) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Unable to write to file: " + filename);
    }

    time_t now = time(nullptr);
    tm* local_time = localtime(&now);
    char date_str[11];
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", local_time);

    file << student_name << '\n';
    file << date_str << '\n';
    file << key << '\n';
    file << data;
    file.close();
}

int main() {
    try {
        std::string input_filename = "inputdatafile.txt";
        std::string encrypted_filename = "encrypteddatafile.txt";
        std::string decrypted_filename = "decrypteddatafile.txt";

        std::cout << "Enter encryption key: ";
        std::string key;
        std::getline(std::cin, key);

        if (key.empty()) {
            throw std::invalid_argument("Encryption key cannot be empty.");
        }

        std::string source_text = read_file(input_filename);
        std::string student_name = get_student_name(source_text);

        Cipher cipher;
        std::string encrypted = cipher.encrypt(source_text, key);
        save_data_file(encrypted_filename, student_name, key, encrypted);

        std::string decrypted = cipher.decrypt(encrypted, key);
        save_data_file(decrypted_filename, student_name, key, decrypted);

        std::cout << "Encryption and decryption completed successfully.\n";
        std::cout << "Files written: " << encrypted_filename << " and " << decrypted_filename << std::endl;

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
