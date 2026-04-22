/**
 * @file Applicant.hpp
 * @brief Заголовочный файл класса Applicant (абитуриент).
 * @details Содержит объявление класса и операторов сравнения.
 */

#pragma once
#include <string>
#include <iostream>

/**
 * @class Applicant
 * @brief Класс, описывающий абитуриента.
 * @details Поля: ФИО, факультет, специальность, сумма баллов.
 *          Сравнение выполняется по правилам варианта 16:
 *          - сначала сумма баллов по убыванию,
 *          - затем ФИО по возрастанию,
 *          - затем факультет по возрастанию,
 *          - затем специальность по возрастанию.
 */
class Applicant {
public:
    std::string fullName;   ///< ФИО абитуриента
    std::string faculty;    ///< Факультет
    std::string specialty;  ///< Специальность
    int totalScore;         ///< Сумма баллов

    /** @brief Конструктор по умолчанию. */
    Applicant();

    /**
     * @brief Конструктор с параметрами.
     * @param name ФИО
     * @param fac Факультет
     * @param spec Специальность
     * @param score Сумма баллов
     */
    Applicant(const std::string& name, const std::string& fac,
              const std::string& spec, int score);

    /// @name Операторы сравнения
    /// @{
    bool operator<(const Applicant& other) const;
    bool operator>(const Applicant& other) const;
    bool operator<=(const Applicant& other) const;
    bool operator>=(const Applicant& other) const;
    /// @}

    /// @name Ввод/вывод в формате CSV
    /// @{
    friend std::ostream& operator<<(std::ostream& os, const Applicant& a);
    friend std::istream& operator>>(std::istream& is, Applicant& a);
    /// @}
};