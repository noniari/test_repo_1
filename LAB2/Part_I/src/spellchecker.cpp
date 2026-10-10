#include "spellchecker.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>

SpellChecker::SpellChecker(const std::string& dictionaryPath) {
	std::ifstream dictionary(dictionaryPath);
	if (!dictionary) {
		throw std::runtime_error("Cannot open dictionary: " + dictionaryPath);
	}

	std::string line;
	while (std::getline(dictionary, line)) {
		const std::string word = normalize(line);
		if (!word.empty()) {
			++words_[word];
		}
	}
	for (const auto& entry : words_) {
		wordsByLength_[entry.first.size()].push_back({entry.first, entry.second});
	}
}

std::string SpellChecker::normalize(const std::string& word) {
	std::string normalized;
	normalized.reserve(word.size());
	for (unsigned char character : word) {
		if (std::isalnum(character)) {
			normalized.push_back(static_cast<char>(std::tolower(character)));
		}
	}
	return normalized;
}

std::size_t SpellChecker::distance(const std::string& first, const std::string& second,
								   std::size_t maxDistance) {
	if (first.size() > second.size() + maxDistance ||
		second.size() > first.size() + maxDistance) {
		return maxDistance + 1;
	}

	const std::size_t infinity = maxDistance + 1;
	std::vector<std::size_t> previous(second.size() + 1);
	std::vector<std::size_t> current(second.size() + 1);
	for (std::size_t column = 0; column <= second.size(); ++column) {
		previous[column] = column <= maxDistance ? column : infinity;
	}

	for (std::size_t row = 1; row <= first.size(); ++row) {
		current[0] = row <= maxDistance ? row : infinity;
		const std::size_t begin = row > maxDistance ? row - maxDistance : 1;
		const std::size_t end = std::min(second.size(), row + maxDistance);
		if (begin > 1) {
			current[begin - 1] = infinity;
		}
		std::size_t rowMinimum = current[0];
		for (std::size_t column = begin; column <= end; ++column) {
			const std::size_t substitutionCost = first[row - 1] == second[column - 1] ? 0 : 2;
			current[column] = std::min({previous[column] + 1, current[column - 1] + 1,
										previous[column - 1] + substitutionCost});
			rowMinimum = std::min(rowMinimum, current[column]);
		}
		if (end < second.size()) {
			current[end + 1] = infinity;
		}
		if (rowMinimum > maxDistance) {
			return infinity;
		}
		previous.swap(current);
	}
	return previous[second.size()];
}

std::string SpellChecker::matchCase(const std::string& replacement, const std::string& original) {
	bool hasLetter = false;
	bool allUpper = true;
	for (unsigned char character : original) {
		if (std::isalpha(character)) {
			hasLetter = true;
			allUpper = allUpper && std::isupper(character);
		}
	}
	if (hasLetter && allUpper) {
		std::string result = replacement;
		std::transform(result.begin(), result.end(), result.begin(), [](unsigned char character) {
			return static_cast<char>(std::toupper(character));
		});
		return result;
	}

	if (!original.empty() && std::isupper(static_cast<unsigned char>(original.front()))) {
		std::string result = replacement;
		result.front() = static_cast<char>(std::toupper(static_cast<unsigned char>(result.front())));
		return result;
	}
	return replacement;
}

std::string SpellChecker::suggestion(const std::string& word, std::size_t maxDistance) const {
	const std::string normalized = normalize(word);
	if (words_.find(normalized) != words_.end()) {
		return word;
	}

	std::string best;
	std::size_t bestDistance = maxDistance + 1;
	std::size_t bestFrequency = 0;
	const std::size_t minimumLength = normalized.size() > maxDistance
										  ? normalized.size() - maxDistance
										  : 0;
	const std::size_t maximumLength = normalized.size() + maxDistance;
	for (std::size_t length = minimumLength; length <= maximumLength; ++length) {
		const auto bucket = wordsByLength_.find(length);
		if (bucket == wordsByLength_.end()) {
			continue;
		}
		for (const WordEntry& candidate : bucket->second) {
			const std::size_t candidateDistance = distance(normalized, candidate.word, maxDistance);
			if (candidateDistance < bestDistance ||
				(candidateDistance == bestDistance && candidate.frequency > bestFrequency) ||
				(candidateDistance == bestDistance && candidate.frequency == bestFrequency &&
				 candidate.word < best)) {
				best = candidate.word;
				bestDistance = candidateDistance;
				bestFrequency = candidate.frequency;
			}
		}
	}
	return bestDistance <= maxDistance ? matchCase(best, word) : word;
}

CorrectionResult SpellChecker::correct(const std::string& text, std::size_t maxDistance) const {
	CorrectionResult result;
	result.text.reserve(text.size());
	for (std::size_t position = 0; position < text.size();) {
		if (!std::isalnum(static_cast<unsigned char>(text[position]))) {
			result.text.push_back(text[position++]);
			continue;
		}

		const std::size_t start = position;
		while (position < text.size() &&
			   std::isalnum(static_cast<unsigned char>(text[position]))) {
			++position;
		}
		const std::string original = text.substr(start, position - start);
		const std::string replacement = suggestion(original, maxDistance);
		if (replacement != original) {
			++result.corrections;
		}
		result.text += replacement;
	}
	return result;
}
