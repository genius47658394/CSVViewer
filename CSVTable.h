//
// Created by kucer on 28.09.2026.
//

#pragma once
#include <sstream>
#include <vector>

#include "CSVColumn.h"

namespace csv {
    class CSVTable {
        std::string_view m_name{};
        std::vector<CSVColumn> m_cols{}; // mb replace with unique_ptr in future

    public:
        CSVTable() = default;

        explicit CSVTable(const std::string_view name) : m_name(name) {}

        CSVTable(const CSVTable& other) = default;

        CSVTable(CSVTable&& other) noexcept :
                    m_name(std::exchange(other.m_name, "")), m_cols(std::move(other.m_cols)) {}

        CSVTable& operator=(const CSVTable& other) {
            if (std::addressof(other) == this)
                return *this;

            this->m_name = other.m_name;
            this->m_cols = other.m_cols;

            return *this;
        }

        CSVTable& operator=(CSVTable&& other) noexcept {
            if (std::addressof(other) == this)
                return *this;

            this->m_name = std::exchange(other.m_name, "");
            this->m_cols = std::move(other.m_cols);

            return *this;
        }

        void add_header(const std::string_view header_name) {
            m_cols.emplace_back(header_name);
        }

        void add_headers(const std::vector<std::string>& headers) {
            for (const auto& header: headers)
                add_header(header);
        }

        void add_row(const std::vector<std::string>& row) {
            std::size_t i = 0;
            for (const auto& part: row) {
                this->m_cols[i].add_row(part);
                i++;
            }
        }

        [[nodiscard]] const std::string_view& name() const {
            return m_name;
        }

        [[nodiscard]] const std::vector<CSVColumn>& columns() const {
            return m_cols;
        }

        [[nodiscard]] std::size_t size() const {
            return m_cols.size();
        }

        friend std::ostream& operator<<(std::ostream& os, const CSVTable& table) {
            os << "Table name: " << table.name() << '\n';
            os << "Table size: " << table.size() << '\n';

            for (const auto& col: table.columns())
                os << col << '\n';

            return os;
        }

        ~CSVTable() = default;
    };

    inline CSVTable from_csv(const std::string_view file_path, char delim = ',') {
        auto split = [&delim](const std::string& line) {
            std::stringstream ss{line};
            std::string buffer{};
            std::vector<std::string> out{};

            while (std::getline(ss, buffer, delim))
                out.emplace_back(buffer);

            return out;
        };

        if (file_path.empty()) throw std::runtime_error("Can't get csv table by empty file path");

        std::ifstream f(file_path.data());

        if (!f) throw std::runtime_error(std::format("Can't get csv table from file at path {}", file_path));

        auto table = CSVTable(file_path.substr(2));

        std::string row{};

        std::getline(f, row);

        table.add_headers(split(row));

        while (std::getline(f, row))
            table.add_row(split(row));

        return table;
    }
}
