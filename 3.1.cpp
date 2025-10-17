/*№3  Задача32.Считать текст из файла.С клавиатуры вводится символ.
 Распечатать в порядке убывания числа встреч введенного 
 символа в словах текста и данные слова.*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

// 分隔符检查 - 包含俄语标点
bool isDelimiter(char c) {
    const string delimiters = " ,.;:!?'\"-()[]{}\n\t\r";
    return delimiters.find(c) != string::npos;
}
 
bool ends_with_dot_txt(const string& str) {
    const char* dot = strrchr(str.c_str(), '.');
    return dot && !strcmp(dot, ".txt");
}

int main() {
    // 俄语界面
    std::cout << "Введите полный путь к файлу: ";
    string filePath; // check this, filePath should have ".txt" in the end
    getline(cin, filePath);
    if (!ends_with_dot_txt(filePath)) {
        cerr << "Файл должен иметь расширение .txt !" << endl;
        return 1;
    }

    // 读取文件（UTF-8编码）
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл" << endl;
        return 1;
    }

    // 读取文件内容
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    cout << "Файл успешно прочитан. Размер: " << text.size() << " байт" << endl;

    // 输入要统计的字符
    cout << "Введите символ для подсчета: ";
    string input;
    getline(cin, input);

    if (input.empty()) {
        cerr << "Ошибка: Не введен символ" << endl;
        return 1;
    }

    // 目标字符
    string targetChar = input;

    // 单词分割和统计
    vector<string> words;
    map<string, int> wordCount;
    int totalCharOccurrences = 0;

    string currentWord;
    for (size_t i = 0; i < text.length(); i++) {
        char c = text[i];

        if (isDelimiter(c)) {
            if (!currentWord.empty()) {
                words.push_back(currentWord);

                // 统计当前单词中目标字符的出现次数
                int count = 0;
                for (size_t j = 0; j < currentWord.length(); j++) {
                    bool match = true;
                    for (size_t k = 0; k < targetChar.length(); k++) {
                        if (j + k >= currentWord.length() || currentWord[j + k] != targetChar[k]) {
                            match = false;
                            break;
                        }
                    }
                    if (match) {
                        count++;
                        // 跳过已匹配的字符
                        j += targetChar.length() - 1;
                    }
                }
                wordCount[currentWord] = count;
                totalCharOccurrences += count;

                currentWord.clear();
            }
        }
        else {
            currentWord += c;
        }
    }

    // 处理最后一个单词
    if (!currentWord.empty()) {
        words.push_back(currentWord);
        int count = 0;
        for (size_t j = 0; j < currentWord.length(); j++) {
            bool match = true;
            for (size_t k = 0; k < targetChar.length(); k++) {
                if (j + k >= currentWord.length() || currentWord[j + k] != targetChar[k]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                count++;
                j += targetChar.length() - 1;
            }
        }
        wordCount[currentWord] = count;
        totalCharOccurrences += count;
    }

    // 转换为vector并排序
    vector<pair<string, int>> sortedWords(wordCount.begin(), wordCount.end());

    // 按字符出现次数降序排序
    sort(sortedWords.begin(), sortedWords.end(),
        [](const pair<string, int>& a, const pair<string, int>& b) {
            if (a.second != b.second) {
                return a.second > b.second;
            }
            return a.first < b.first;
        });

    // 输出结果
    cout << "\nСлова, отсортированные по количеству вхождений символа:" << endl;
    cout << "==================================" << endl;

    bool foundWordsWithChar = false;
    for (const auto& item : sortedWords) {
        if (item.second > 0) {
            cout << item.first << " - " << item.second << " раз" << endl;
            foundWordsWithChar = true;
        }
    }

    if (!foundWordsWithChar) {
        cout << "Нет слов, содержащих введенный символ" << endl;
    }

    // 统计信息
    int wordsWithChar = count_if(sortedWords.begin(), sortedWords.end(),
        [](const pair<string, int>& item) { return item.second > 0; });

   /* cout << "\nСтатистика:" << endl;
    cout << "==================================" << endl;
    cout << "Всего слов: " << words.size() << endl;
    cout << "Уникальных слов: " << wordCount.size() << endl;
    cout << "Слов, содержащих символ: " << wordsWithChar << endl;
    cout << "Слов, не содержащих символ: " << words.size() - wordsWithChar << endl;
    cout << "Общее количество вхождений символа: " << totalCharOccurrences << endl;*/

    return 0;
}
