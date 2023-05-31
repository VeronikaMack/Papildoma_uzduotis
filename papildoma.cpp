#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <regex>

bool arskaicius(const std::string& word) {
    for (char a : word) {
        if (!std::isdigit(a)) {
            return false;
        }
    }
    return true;
}


std::vector<std::string> findURLs(const std::string& line) {
    std::regex urlRegex("\\b(?:https?://\\S+|www\\.\\S+|\\S+\\.(?:lt|gov|com|site|org|net|int|edu|mil))\\b");
    std::smatch urlMatch;
    std::vector<std::string> urls;

    std::string::const_iterator searchStart(line.cbegin());
    while (std::regex_search(searchStart, line.cend(), urlMatch, urlRegex)) {
        urls.push_back(urlMatch.str());
        searchStart = urlMatch.suffix().first;
    }

    return urls;
}

int main() {
    std::string input = "tekstas.txt"; 
    std::string output = "rezultatas.txt"; 
    std::ifstream inputFile(input);
    if (!inputFile) {
        std::cout << "Nepavyko atidaryti failo: tekstas.txt" << std::endl;
        return 1;
    }

   
    std::map<std::string, int> wordMap;
    std::map<std::string, std::vector<int> > LineMap;
    std::map<std::string, std::vector<int> > urlMap;
    std::string line;
    int lineNumber = 1;
    while (std::getline(inputFile, line)) {
        std::istringstream s(line);
        std::string word;
        while (s >> word) {
        
            word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());

           
            if (arskaicius(word)) {
                continue;
            }

            ++wordMap[word];

           
            if (wordMap[word] >= 1) {
                LineMap[word].push_back(lineNumber);
            }
           
        
        }

        std::vector<std::string> urls = findURLs(line);
        for (const std::string& url : urls) {
            urlMap[url].push_back(lineNumber);
        }

        ++lineNumber;
    }

    
    inputFile.close();

    std::ofstream outputFile(output);
    if (!outputFile) {
        std::cout << "Nepavyko atidaryti failo: rezultatas.txt" << std::endl;
        return 1;
    }

    for (const auto& pair : wordMap) {
        const std::string& word = pair.first;
        int count = pair.second;

        if (count > 1) {
            outputFile << word << ": " << count << " (Lines: ";
            const std::vector<int>& lineNumbers = LineMap[word];
            for (size_t i = 0; i < lineNumbers.size(); ++i) {
                if (i > 0) {
                    outputFile << ", ";
                }
                outputFile << lineNumbers[i];
            }
            outputFile << ")" << std::endl;
        }
    }


    for (const auto& pair : urlMap) {
        const std::string& url = pair.first;
        outputFile << url << std::endl;
    }

    outputFile.close();

    std::cout << "Rezultata rasite faile: " << output << std::endl;

    return 0;
}
