//
// Created by kucer on 28.09.2026.
//

#pragma once

#include <vector>

#include "CSVColumn.h"

namespace csv {
    class CSVTable;

    inline CSVTable from_csv(const std::string& file_path, char delim = ',');
    inline void to_csv_file(const CSVTable& table, char delim = ',');

    class CSVTable {
        std::string m_path{};
        std::vector<CSVColumn> m_cols{}; // mb replace with unique_ptr in future

    public:
        CSVTable() = default;

        explicit CSVTable(const std::string& path) {
            *this = std::move(from_csv(path));
        }

        CSVTable(const CSVTable& other) = default;

        CSVTable(CSVTable&& other) noexcept :
                    m_path(std::exchange(other.m_path, "")), m_cols(std::move(other.m_cols)) {}

        CSVTable& operator=(const CSVTable& other) {
            if (std::addressof(other) == this)
                return *this;

            this->m_path = other.m_path;
            this->m_cols = other.m_cols;

            return *this;
        }

        CSVTable& operator=(CSVTable&& other) noexcept {
            if (std::addressof(other) == this)
                return *this;

            this->m_path = std::exchange(other.m_path, "");
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
                if (i >= size()) throw std::runtime_error(std::format(
                "index of column is more than count of columns\n\t count of column = {}, index of column = {}",
                size(), i));

                this->m_cols[i].add_row(part);
                i++;
            }
        }

        [[nodiscard]] const std::string& name() const {
            return m_path;
        }

        [[nodiscard]] std::string& name() {
            return m_path;
        }

        [[nodiscard]] std::vector<std::string> headers() const {
            std::vector<std::string> out{};

            for (const auto& col: columns())
                out.emplace_back(col.header());

            return out;
        }

        [[nodiscard]] const std::vector<CSVColumn>& columns() const {
            return m_cols;
        }

        CSVColumn& operator[](const std::size_t index) {
            if (index >= size())
                throw std::runtime_error(
                    std::format("Index out of range\n\tindex={}, count of columns = {}", index, size())
                    );

            return m_cols[index];
        }

        const CSVColumn& operator[](const std::size_t index) const {
            if (index >= size())
                throw std::runtime_error(
                    std::format("Index out of range\n\tindex={}, count of columns = {}", index, size())
                    );

            return m_cols[index];
        }

        [[nodiscard]] bool empty() const {
            return m_cols.empty();
        }

        // count of columns in table
        [[nodiscard]] std::size_t size() const {
            return m_cols.size();
        }

        // count of rows in column (all columns have one count of rows)
        [[nodiscard]] std::size_t size_of_column() const {
            if (empty()) return 0;

            return m_cols[0].size();
        }

        friend std::ostream& operator<<(std::ostream& os, const CSVTable& table) {
            os << "Table name: " << table.name() << '\n';
            os << "Table size: " << table.size() << '\n';

            for (const auto& col: table.columns())
                os << col << '\n';

            return os;
        }

        ~CSVTable() {
            to_csv_file(*this);
        }

        [[nodiscard]] std::vector<std::string> get_row(std::size_t col_idx) const {
            if (col_idx >= size_of_column()) throw std::runtime_error(std::format(
                "Can't get row cuz column index i more than size of column\n\t column index = {}, size of column = {}",
                col_idx, size_of_column()));

            std::vector<std::string> out{};

            for (const auto& col: m_cols)
                out.emplace_back(col[col_idx]);

            return out;
        }
    };

    inline CSVTable from_csv(const std::string& file_path, char delim) {
        auto split = [&delim](const std::string& line) {
            std::vector<std::string> out{};
            std::string buffer{};

            for (auto it = line.begin(); it != line.end(); ++it) {
                if (*it == '\"') {
                    ++it;

                    while (*it != '\"') {
                        buffer += *it;
                        ++it;
                    }

                    continue;
                }

                if (*it == delim) /* replace in future cuz delimiter would be const char* */ {
                    out.emplace_back(buffer);
                    buffer.clear();
                    continue;
                }

                buffer += *it;
            }

            out.emplace_back(buffer);
            buffer.clear();

            return out;
        };

        if (file_path.empty()) throw std::runtime_error("Can't get csv table by empty file path");

        std::ifstream f(file_path);

        if (!f) throw std::runtime_error(std::format("Can't get csv table from file at path {}", file_path));

        std::string name = std::string(file_path.begin(), file_path.end());

        auto table = CSVTable();
        table.name() = std::move(name);

        std::string row{};

        std::getline(f, row);

        table.add_headers(split(row));

        while (std::getline(f, row))
            table.add_row(split(row));

        return table;
    }

    inline void to_csv_file(const CSVTable& table, char delim) {
        if (table.empty()) return;

        auto vec_to_str = [&delim](const std::vector<std::string>& vec) {
            std::string out{};

            for (const auto& part: vec)
                out += part + delim;

            out = std::string(out.begin(), out.end() - 1 );
            return out;
        };

        const std::string path = std::move(std::string(table.name()));

        std::ofstream out(path);

        if (!out) throw std::runtime_error(std::format("Can't open file to write table by path \"{}\"", path));

        out.clear();

        out << vec_to_str(table.headers()) << std::endl;

        for (std::size_t i = 0; i < table.size_of_column(); ++i)
            out << vec_to_str(table.get_row(i)) << std::endl;
    }
}
