#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

using StudentName = std::string;
using TaskName = std::string;

using ScoreTable = std::map<StudentName, std::set<TaskName>>;

class Scorer {
private:
    ScoreTable current_state_;
    std::map<std::pair<StudentName, TaskName>, bool> didnt_pass_last_check_;
    std::map<std::pair<StudentName, TaskName>, bool> has_opened_merge_request_;

public:
    void OnCheckFailed(const StudentName& student_name, const TaskName& task_name);
    void OnCheckSuccess(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name);
    void CheckAndErase(const StudentName& student_name);
    void Reset();

    ScoreTable GetScoreTable() const;
};
