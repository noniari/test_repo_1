#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

struct CorrectionResult {
	std::string text;
	std::size_t corrections = 0;
};

class SpellChecker {
public:
	explicit SpellChecker(const std::string& dictionaryPath);

	CorrectionResult correct(const std::string& text, std::size_t maxDistance) const;

private:
	struct WordEntry {
		std::string word;
		std::size_t frequency;
	};

	std::unordered_map<std::string, std::size_t> words_;
	std::unordered_map<std::size_t, std::vector<WordEntry>> wordsByLength_;

	static std::string normalize(const std::string& word);
	static std::size_t distance(const std::string& first, const std::string& second,
								std::size_t maxDistance);
	static std::string matchCase(const std::string& replacement, const std::string& original);
	std::string suggestion(const std::string& word, std::size_t maxDistance) const;
};
