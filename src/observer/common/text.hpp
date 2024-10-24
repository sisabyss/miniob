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
#ifndef TEXT_H_
#define TEXT_H_

#include <cstddef>
#include <string>

static constexpr int MAX_TEXT_LENGTH = 4096;

struct Text {
    size_t id;
    size_t len;

    inline std::string to_string() const;
};

inline std::string Text::to_string() const {
    return std::to_string(id) + ":" + std::to_string(len);
}

#endif // TEXT_H_
