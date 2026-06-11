#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

class ScoreManager {
public:
    static constexpr int maxScores = 5;

    void load(const std::string& path = "scores.dat") {
        filePath = path;
        scores.clear();
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cout << "[Info] No score file found, starting fresh\n";
            return;
        }
        int s;
        while (file >> s)
            scores.push_back(s);
        file.close();
        sortAndTrim();
        std::cout << "[Info] Loaded " << scores.size() << " scores\n";
    }

    void save() {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "[Error] Could not save scores\n";
            return;
        }
        for (int s : scores)
            file << s << "\n";
        file.close();
        std::cout << "[Info] Scores saved\n";
    }

    void addScore(int score) {
        scores.push_back(score);
        sortAndTrim();
        save();
    }

    const std::vector<int>& getScores() const { return scores; }

    bool isHighScore(int score) const {
        if (static_cast<int>(scores.size()) < maxScores) return true;
        return score > scores.back();
    }

private:
    void sortAndTrim() {
        std::sort(scores.begin(), scores.end(), std::greater<int>());
        if (static_cast<int>(scores.size()) > maxScores)
            scores.resize(maxScores);
    }

    std::vector<int> scores;
    std::string      filePath = "scores.dat";
};