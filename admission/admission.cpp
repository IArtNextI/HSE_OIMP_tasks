#include "admission.h"

#include <map>

bool operator<(const Date& a, const Date& b) {
    return std::tie(a.year, a.month, a.day) < std::tie(b.year, b.month, b.day);
}

bool operator<(const Applicant& a, const Applicant& b) {
    return std::tie(a.points, b.student.birth_date, b.student.name) >
           std::tie(b.points, a.student.birth_date, a.student.name);
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<Applicant> sorted_applicants = applicants;
    sort(sorted_applicants.begin(), sorted_applicants.end());
    std::map<std::string, size_t> students_left;
    for (const auto& university : universities) {
        students_left[university.name] = university.max_students;
    }
    AdmissionTable result;
    for (size_t i = 0; i < applicants.size(); ++i) {
        for (const auto& university : applicants[i].wish_list) {
            if (students_left[university] >= 1) {
                result[university].push_back(&applicants[i].student);
                --students_left[university];
                break;
            }
        }
    }
    for (auto& [university, students] : result) {
        sort(students.begin(), students.end(), [](const Student* a, const Student* b) {
            return std::tie(a->name, a->birth_date) < std::tie(b->name, b->birth_date);
        });
    }
    return result;
}
