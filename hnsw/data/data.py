import requests
import json

# OpenAI API相关信息
API_KEY = 'YOUR_OPENAI_API_KEY'  # 替换为您的OpenAI API密钥
MODEL_NAME = 'text-embedding-ada-002'

# 输入和输出文件路径
input_file_path = 'input.txt'  # 替换为输入文件路径
output_file_path = 'output.txt'  # 替换为输出文件路径

def get_embedding(text):
    # 构建请求数据
    data = {
        "input": text,
        "model": MODEL_NAME
    }

    # 发送POST请求获取向量
    headers = {
        'Content-Type': 'application/json',
        'Authorization': 'Bearer ' + API_KEY
    }
    try:
        response = requests.post('https://api.openai.com/v1/embeddings', headers=headers, data=json.dumps(data))
        response_json = response.json()
        embedding = response_json['data'][0]['embedding']
    except requests.exceptions.RequestException as e:
        print(f"请求失败: {e}")
        return None
    except (KeyError, ValueError) as e:
        print(f"解析响应数据失败: {e}")
        print(f"响应数据: {response.text}")
        return None


    return embedding

def main():
    # 读取输入文件，获取每一行的内容并请求向量
    with open(input_file_path, 'r') as input_file:
        lines = input_file.readlines()

        # 处理每一行文本
        embeddings = []
        for i, line in enumerate(lines, start=1):
            line = line.strip()  # 去除换行符和空格
            print(f"Processing line {i} {line}...")
            embedding = get_embedding(line)
            embeddings.append(embedding)

    # 将内容和向量按行输出到输出文件
    with open(output_file_path, 'w') as output_file:
        for i, (line, embedding) in enumerate(zip(lines, embeddings), start=1):
            print(f"Writing line {i}...")
            output_file.write(f"{i}:{line.strip()}:{','.join(str(x) for x in embedding)}\n")

if __name__ == '__main__':
    main()