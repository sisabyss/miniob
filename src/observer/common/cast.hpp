/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#ifndef CAST_H_
#define CAST_H_

#include <sstream>
#include <string>

// str2int: 将 const char* 字符串前导部分转换为整数，如果无法转换则返回 0
inline int str2int(const char* str) {
    std::string s(str);  // 将 const char* 转换为 std::string
    std::istringstream stream(s);
    int value = 0;
    if (!(stream >> value)) {
        return 0;  // 无法转换则返回 0
    }
    return value;
}

// str2float: 将 const char* 字符串前导部分转换为浮点数，如果无法转换则返回 0
inline float str2float(const char* str) {
    std::string s(str);  // 将 const char* 转换为 std::string
    std::istringstream stream(s);
    float value = 0.0;
    if (!(stream >> value)) {
        return 0.0;  // 无法转换则返回 0.0
    }
    return value;
}

#endif // CAST_H_
