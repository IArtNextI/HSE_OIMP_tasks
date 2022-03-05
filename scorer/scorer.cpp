#include "scorer.h"

bool operator<(const Event &a, const Event &b) {
    return std::tie(a.time, a.event_type) < std::tie(b.time, b.event_type);
}

ScoreTable GetScoredStudents(const Events &events, time_t score_time) {
    ScoreTable result;
    std::map<StudentName, std::set<Event>> events_by_student;
    for (const auto &event : events) {
        events_by_student[event.student_name].insert(event);
    }
    for (const auto &[student, associated_events] : events_by_student) {
        std::map<TaskName, bool> didnt_pass_last_check;
        std::map<TaskName, bool> has_opened_merge_request;
        for (const auto &event : associated_events) {
            if (event.time > score_time) {
                break;
            }
            if (event.event_type == EventType::CheckFailed) {
                didnt_pass_last_check[event.task_name] = true;
                result[student].erase(event.task_name);
            } else if (event.event_type == EventType::CheckSuccess) {
                didnt_pass_last_check[event.task_name] = false;
                if (!has_opened_merge_request[event.task_name]) {
                    result[student].insert(event.task_name);
                }
            } else if (event.event_type == EventType::MergeRequestOpen) {
                has_opened_merge_request[event.task_name] = true;
                result[student].erase(event.task_name);
            } else if (event.event_type == EventType::MergeRequestClosed) {
                has_opened_merge_request[event.task_name] = false;
                if (!didnt_pass_last_check[event.task_name]) {
                    result[student].insert(event.task_name);
                }
            }
        }
    }
    std::set<StudentName> empty_rows;
    for (const auto &[student, solved_tasks] : result) {
        if (solved_tasks.empty()) {
            empty_rows.insert(student);
        }
    }
    for (auto &row : empty_rows) {
        result.erase(row);
    }
    return result;
}
