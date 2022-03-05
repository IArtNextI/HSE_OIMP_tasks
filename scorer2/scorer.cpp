#include "scorer.h"

ScoreTable Scorer::GetScoreTable() const {
    return current_state_;
}

void Scorer::Reset() {
    current_state_.clear();
    didnt_pass_last_check_.clear();
    has_opened_merge_request_.clear();
}

void Scorer::CheckAndErase(const StudentName &student_name) {
    if (current_state_[student_name].empty()) {
        current_state_.erase(student_name);
    }
}

void Scorer::OnCheckFailed(const StudentName &student_name, const TaskName &task_name) {
    didnt_pass_last_check_[std::make_pair(student_name, task_name)] = true;
    current_state_[student_name].erase(task_name);
    CheckAndErase(student_name);
}

void Scorer::OnCheckSuccess(const StudentName &student_name, const TaskName &task_name) {
    didnt_pass_last_check_[std::make_pair(student_name, task_name)] = false;
    if (!has_opened_merge_request_[std::make_pair(student_name, task_name)]) {
        current_state_[student_name].insert(task_name);
        CheckAndErase(student_name);
    }
}

void Scorer::OnMergeRequestOpen(const StudentName &student_name, const TaskName &task_name) {
    has_opened_merge_request_[std::make_pair(student_name, task_name)] = true;
    current_state_[student_name].erase(task_name);
    CheckAndErase(student_name);
}

void Scorer::OnMergeRequestClosed(const StudentName &student_name, const TaskName &task_name) {
    has_opened_merge_request_[std::make_pair(student_name, task_name)] = false;
    if (!didnt_pass_last_check_[std::make_pair(student_name, task_name)]) {
        current_state_[student_name].insert(task_name);
        CheckAndErase(student_name);
    }
}