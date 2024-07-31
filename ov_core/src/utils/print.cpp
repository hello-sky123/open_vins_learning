/*
 * OpenVINS: An Open Platform for Visual-Inertial Research
 * Copyright (C) 2018-2023 Patrick Geneva
 * Copyright (C) 2018-2023 Guoquan Huang
 * Copyright (C) 2018-2023 OpenVINS Contributors
 * Copyright (C) 2018-2019 Kevin Eckenhoff
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "print.h"

using namespace ov_core;

// Need to define the static variable for everything to work
// 静态成员变量在类的所有实例之间共享，并且必须在类定义的外部进行初始化，且所有非静态成员变量之前初始化
// 初始化时，需要使用类名和作用域解析运算符::作为前缀，如果是常量静态成员变量，可以在类定义中初始化
Printer::PrintLevel Printer::current_print_level = PrintLevel::INFO; // 默认为INFO

void Printer::setPrintLevel(const std::string &level) {
  if (level == "ALL")
    setPrintLevel(PrintLevel::ALL);
  else if (level == "DEBUG")
    setPrintLevel(PrintLevel::DEBUG);
  else if (level == "INFO")
    setPrintLevel(PrintLevel::INFO);
  else if (level == "WARNING")
    setPrintLevel(PrintLevel::WARNING);
  else if (level == "ERROR")
    setPrintLevel(PrintLevel::ERROR);
  else if (level == "SILENT")
    setPrintLevel(PrintLevel::SILENT);
  else {
    std::cout << "Invalid print level requested: " << level << std::endl;
    std::cout << "Valid levels are: ALL, DEBUG, INFO, WARNING, ERROR, SILENT" << std::endl;
    std::exit(EXIT_FAILURE); // 终止当前程序的执行，并返回一个状态码给操作系统
  }
}

void Printer::setPrintLevel(PrintLevel level) {
  Printer::current_print_level = level;
  std::cout << "Setting printing level to: ";
  switch (current_print_level) {
  case PrintLevel::ALL:
    std::cout << "ALL";
    break;
  case PrintLevel::DEBUG:
    std::cout << "DEBUG";
    break;
  case PrintLevel::INFO:
    std::cout << "INFO";
    break;
  case PrintLevel::WARNING:
    std::cout << "WARNING";
    break;
  case PrintLevel::ERROR:
    std::cout << "ERROR";
    break;
  case PrintLevel::SILENT:
    std::cout << "SILENT";
    break;
  default:
    std::cout << std::endl;
    std::cout << "Invalid print level requested: " << level << std::endl;
    std::cout << "Valid levels are: ALL, DEBUG, INFO, WARNING, ERROR, SILENT" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::cout << std::endl;
}

void Printer::debugPrint(PrintLevel level, const char location[], const char line[], const char *format, ...) {
  // Only print for the current debug level，如果请求的打印级别小于当前打印级别，则不打印
  if (static_cast<int>(level) < static_cast<int>(Printer::current_print_level)) {
    return;
  }

  // Print the location info first for our debug output
  // Truncate the filename to the max size for the filepath
  if (static_cast<int>(Printer::current_print_level) <= static_cast<int>(Printer::PrintLevel::DEBUG)) {
    std::string path(location); // 将文件路径转换为std::string类型
    std::string base_filename = path.substr(path.find_last_of("/\\") + 1); // 找到最后一个斜杠或反斜杠的位置，并获取其后面的子字符串
    if (base_filename.size() > MAX_FILE_PATH_LEGTH) {
      printf("%s", base_filename.substr(base_filename.size() - MAX_FILE_PATH_LEGTH, base_filename.size()).c_str());
    } else {
      printf("%s", base_filename.c_str());
    }
    printf(":%s ", line);
  }

  // Print the rest of the args
  va_list args; // 声明一个可变参数列表
  // format是va_start需要的最后一个固定参数的名称，它用来确定参数包的起始位置
  va_start(args, format); // 初始化可变参数列表，将format后面的参数存储到args中
  vprintf(format, args); // vprintf函数是printf函数的变体，它允许你使用va_list来访问可变数量的参数
  va_end(args); // va_end宏用于清理va_list指针args
}
