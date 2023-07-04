#!/bin/bash

# 获取当前脚本所在目录的绝对路径
current_dir=$(cd $(dirname $0) && pwd)

# 输出文件名
output_file="${current_dir}/all_cpp_files.txt"

# 删除旧的输出文件
rm -f "${output_file}"

exclude_dirs=("build" "benchmarks" "test")

# 遍历当前目录的父目录下的所有目录，输出其中的 .h 和 .cpp 文件
find "${current_dir}/../" -type d \( -name "${exclude_dirs[0]}" -o -name "${exclude_dirs[1]}" -o -name "${exclude_dirs[2]}" \) -prune -o \( -type f \( -name "*.h" -o -name "*.cpp" \) \) -print | while read file; do
  # 获取文件相对于父目录的路径
  relative_path="${file#${current_dir}/../}"

  # 输出文件路径
  echo "\n" >> "${output_file}"
  echo "${relative_path}" >> "${output_file}"

  # 删除文件中以 # 或 // 开头的行，并删除空行，并输出到输出文件中
  sed -e '/^#/d' -e '/^\/\//d' -e '/^\s*$/d' "${file}" >> "${output_file}"
  
done