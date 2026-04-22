/**
 * @file Applicant.cpp
 * @brief Реализация методов класса Applicant.
 */

#include "Applicant.hpp"
#include <sstream>
#include <vector>

Applicant::Applicant() : totalScore(0) {}

Applicant::Applicant(const std::string& name, const std::string& fac,
                     const std::string& spec, int score)
    : fullName(name), faculty(fac), specialty(spec), totalScore(score) {}

bool Applicant::operator<(const Applicant& other) const {
    // 1. Сумма баллов по убыванию
    if (totalScore != other.totalScore)
        return totalScore > other.totalScore;
    // 2. ФИО по возрастанию
    if (fullName != other.fullName)
        return fullName < other.fullName;
    // 3. Факультет по возрастанию
    if (faculty != other.faculty)
        return faculty < other.faculty;
    // 4. Специальность по возрастанию
    return specialty < other.specialty;
}

bool Applicant::operator>(const Applicant& other) const {
    return other < *this;
}

bool Applicant::operator<=(const Applicant& other) const {
    return !(*this > other);
}

bool Applicant::operator>=(const Applicant& other) const {
    return !(*this < other);
}

std::ostream& operator<<(std::ostream& os, const Applicant& a) {
    os << a.fullName << "," << a.faculty << "," << a.specialty << "," << a.totalScore;
    return os;
}

std::istream& operator>>(std::istream& is, Applicant& a) {
    std::string line;
    if (std::getline(is, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ','))
            tokens.push_back(token);
        if (tokens.size() == 4) {
            a.fullName = tokens[0];
            a.faculty = tokens[1];
            a.specialty = tokens[2];
            a.totalScore = std::stoi(tokens[3]);
        } else {
            a = Applicant();
        }
    }
    return is;
}