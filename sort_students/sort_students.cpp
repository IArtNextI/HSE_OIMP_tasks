#include "sort_students.h"
#include <algorithm>

bool operator<(const Date &a, const Date &b) {
    return std::tie(a.year, a.month, a.day) < std::tie(b.year, b.month, b.day);
}

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    if (sortKind == SortKind::Name) {
        sort(students.begin(), students.end(), [](const Student &a, const Student &b) {
            return std::tie(a.last_name, a.name, a.birth_date) < tie(b.last_name, b.name, b.birth_date);
        });
    } else if (sortKind == SortKind::Date) {
        sort(students.begin(), students.end(), [](const Student &a, const Student &b) {
            return std::tie(a.birth_date, a.last_name, a.name) < tie(b.birth_date, b.last_name, b.name);
        });
    }
}
