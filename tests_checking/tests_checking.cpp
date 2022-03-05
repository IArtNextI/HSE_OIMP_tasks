#include "tests_checking.h"

#include <deque>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::deque<std::string> order_of_papers;
    for (const auto& action : student_actions) {
        if (action.side == Side::Top) {
            order_of_papers.push_front(action.name);
        } else {
            order_of_papers.push_back(action.name);
        }
    }
    std::vector<std::string> result;
    for (const auto& query : queries) {
        result.push_back(order_of_papers[query - 1]);
    }
    return result;
}
