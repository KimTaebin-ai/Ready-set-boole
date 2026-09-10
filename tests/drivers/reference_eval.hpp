#ifndef REFERENCE_EVAL_HPP
# define REFERENCE_EVAL_HPP

# include <string>

// 테스트가 정답 기준으로 쓰는 독립 평가기.
//
// ex03 부터 ex07 까지는 common/ast 의 파서와 평가기를 공유합니다. 그 코드를
// 기준으로 삼으면 같은 코드로 같은 코드를 검사하는 셈이 되므로, 리팩토링
// 이전 ex03 에 있던 스택 기반 평가기를 이 파일로 옮겨 보존했습니다.
// 검사 대상과 공유하는 코드가 한 줄도 없습니다.
//
// 0 과 1 리터럴만 받습니다. 변수가 있으면 던집니다.
bool reference_eval(const std::string& formula);

#endif
