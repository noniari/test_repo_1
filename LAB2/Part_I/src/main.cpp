#include "spellchecker.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    const std::filesystem::path executableDirectory =
        std::filesystem::path(argv[0]).parent_path();
    std::string dictionaryPath = (executableDirectory / "words.txt").string();
    std::string inputPath;
    std::string inlineText;
    std::size_t maxDistance = 2;

    try {
        for (int index = 1; index < argc; ++index) {
            const std::string argument = argv[index];
            if (argument == "--dict" && index + 1 < argc) {
                dictionaryPath = argv[++index];
            } else if (argument == "--max-distance" && index + 1 < argc) {
                maxDistance = static_cast<std::size_t>(std::stoul(argv[++index]));
            } else if (argument == "--input" && index + 1 < argc) {
                inputPath = argv[++index];
            } else if (argument == "--help") {
                std::cout << "Usage: spellchecker [--dict FILE] [--max-distance N] [--input FILE] [TEXT]\n"
                             "If neither --input nor TEXT is given, text is read from stdin.\n";
                return 0;
            } else if (!argument.empty() && argument[0] == '-') {
                throw std::invalid_argument("Unknown option: " + argument);
            } else {
                if (!inlineText.empty()) {
                    inlineText += ' ';
                }
                inlineText += argument;
            }
        }

        std::string text;
        if (!inputPath.empty()) {
            std::ifstream input(inputPath);
            if (!input) {
                throw std::runtime_error("Cannot open input file: " + inputPath);
            }
            std::ostringstream contents;
            contents << input.rdbuf();
            text = contents.str();
        } else if (!inlineText.empty()) {
            text = inlineText;
        } else {
            std::ostringstream contents;
            contents << std::cin.rdbuf();
            text = contents.str();
        }

        SpellChecker spellChecker(dictionaryPath);
        const auto start = std::chrono::steady_clock::now();
        const CorrectionResult result = spellChecker.correct(text, maxDistance);
        const auto finish = std::chrono::steady_clock::now();

        std::cout << result.text;
        if (result.text.empty() || result.text.back() != '\n') {
            std::cout << '\n';
        }
        std::cout << "Corrections: " << result.corrections << '\n'
                  << "Time: "
                  << std::chrono::duration<double, std::milli>(finish - start).count()
                  << " ms\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}