/*
 * Copyright 2019 Väinö Lappi
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /*
  * My base class for exceptions. Might contain some usefull additions for debugging.
  */
#ifndef _DJAP_UTILS_EXCEPTION_HPP_
#define _DJAP_UTILS_EXCEPTION_HPP_
#include <stdexcept>
#include <list>
namespace DjapUtils {
class Exception : public std::exception {
public:
    Exception(const std::string& error);
    ~Exception();
    const std::string& error_message();
private:
    std::list<std::string> _stack_trace;    // TODO might or might not fill in this later.
    std::string _error;
};

} // namespace DjapUtils
#endif // _DJAP_UTILS_EXCEPTION_HPP_
