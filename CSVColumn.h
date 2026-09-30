//
// Created by kucer on 28.09.2026.
//

#pragma once

#include <format>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace csv {
    class CSVColumn {
        std::unique_ptr<std::string> m_header = nullptr;
        std::vector<std::string> m_rows{}; // mb in future replace with unique_ptr

    public:
        CSVColumn() = default;

        explicit CSVColumn(const std::string_view name) : m_header(std::move(std::make_unique<std::string>(name))) {}

        CSVColumn(const CSVColumn& other) : m_header(std::move(std::make_unique<std::string>(*other.m_header))),
                                                     m_rows(other.m_rows) {}

        CSVColumn(CSVColumn&& other) noexcept : m_header(std::move(other.m_header)), m_rows(std::move(other.m_rows)) {}

        CSVColumn& operator=(const CSVColumn& other) {
            if (this == std::addressof(other))
                return *this;

            this->m_header = std::move(std::make_unique<std::string>(*other.m_header));
            this->m_rows = other.m_rows;

            return *this;
        }

        CSVColumn& operator=(CSVColumn&& other) noexcept {
            if (this == std::addressof(other))
                return *this;

            m_header = std::move(other.m_header);
            m_rows = std::move(other.m_rows);

            return *this;
        }

        void add_row(const std::string& row) {
            m_rows.emplace_back(row);
        }

        void add_row(const std::string_view row) {
            m_rows.emplace_back(row);
        }

        [[nodiscard]] std::string& header() {
            return *m_header;
        }

        [[nodiscard]] std::vector<std::string>& rows() {
            return m_rows;
        }

        [[nodiscard]] const std::string& header() const {
            return *m_header;
        }

        [[nodiscard]] const std::vector<std::string>& rows() const {
            return m_rows;
        }

        std::string& operator[](const std::size_t index) {
            return m_rows.at(index);
        }

        const std::string& operator[](const std::size_t index) const {
            return m_rows.at(index);
        }

        friend std::ostream& operator<<(std::ostream& os, const CSVColumn& col) {
            os << "Column header: " << col.header() << '\n';
            os << "column size: " << col.rows().size() << '\n';

            for (const auto& row: col.rows())
                os << row << '\n';

            return os;
        }

        [[nodiscard]] std::size_t size() const {
            return m_rows.size();
        }

        class CSVIter {
            CSVColumn* m_column;
            std::size_t m_pos = 0;

        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = std::string;
            using difference_type = std::ptrdiff_t;
            using pointer = std::string*;
            using reference = std::string&;

            CSVIter(CSVColumn* column, std::size_t pos)
                : m_column(column), m_pos(pos) {}

            reference operator*() {
                return m_column->m_rows[m_pos];
            }

            pointer operator->() {
                return std::addressof(m_column->m_rows[m_pos]);
            }

            CSVIter& operator++() {
                ++m_pos;
                return *this;
            }

            CSVIter& operator--() {
                --m_pos;
                return *this;
            }

            CSVIter& operator+=(difference_type n) {
                m_pos += n;
                return *this;
            }

            CSVIter& operator-=(difference_type n) {
                m_pos -= n;
                return *this;
            }

            reference operator[](difference_type n) {
                return m_column->m_rows[m_pos + n];
            }
        };

        CSVIter begin() { return {this, 0}; }

        CSVIter end() { return {this, m_rows.size()}; }

        // TODO: add API
        // TODO: add compare operators

        ~CSVColumn() = default;
    };
}
