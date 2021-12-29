
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>


int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Invalid arguments" << std::endl;
    }

    std::ifstream file(argv[1]);
    std::vector <
        std::pair<
        std::string,
        int
        >
    > dict;
    std::string f_string;
    std::string s_string;
    int value;

    if (file.is_open())
    {
        while (std::getline(file, f_string))
        {
            if (f_string[f_string.size() - 1] == '\r')
                f_string.erase(f_string.size() - 1);
            if (!f_string.empty())
            {
                s_string = f_string.substr(0, f_string.find(','));
                value = std::stoi(f_string.substr(f_string.find(' ') + 1));
                dict.push_back(std::pair<std::string, int>(s_string, value));
            }
        }

    }
    else
    {
        throw std::invalid_argument("File Load Failed");
    }

    std::ifstream input(argv[2]);
    if (!input.is_open())
        throw std::invalid_argument("Can't load file");

    std::vector<std::string> phrase;
    if (std::getline(input, f_string) && !(f_string.length() == 1 && isspace(f_string[0])))
    {
        phrase.clear();
        f_string.erase(std::remove(f_string.begin(), f_string.end(), '\r'), f_string.end());
        std::string word;
        std::stringstream sstream(f_string);
        while (getline(sstream, word, ' '))
        {
            phrase.push_back(word);
        }
        std::map<std::string, std::vector<std::pair<std::string, int>>> permWords;
        for (auto word : phrase)
        {
            std::cout << "Permutations for original word " << word << ": ";
            permWords.insert(std::pair<std::string, std::vector<std::pair<std::string, int>>>(word, {}));
            for (auto elem : dict)
            {
                if (std::is_permutation(word.begin(), word.end(), elem.first.begin(), elem.first.end()))
                {
                    std::cout << elem.first << " weight = " << elem.second << ", ";
                    permWords.at(word).emplace_back(elem.first, elem.second);
                }
            }
            if (permWords.at(word).empty())
            {
                input.close();
                throw std::runtime_error("Permutaions has not Found");
            }
            std::cout << std::endl;
            std::sort(permWords.at(word).begin(), permWords.at(word).end(), [](std::pair<std::string, int> lhs,
                std::pair<std::string, int> rhs)
                {return lhs.second > rhs.second; }
            );
        }
        std::cout << std::endl;

        std::vector<size_t>availableIdx(phrase.size(), 0);
        int count = 1;
        do {
            std::vector<std::string> permutationPhrases;
            std::vector<int> permutationSentenceWeights(phrase.size(), 0);
            int phraseWeight = 0;
            for (size_t i = 0; i < phrase.size(); ++i)
            {
                permutationPhrases.push_back(permWords.at(phrase[i])[availableIdx[i]].first);
                permutationSentenceWeights[i] = permWords.at(phrase[i])[availableIdx[i]].second;
            }
            std::sort(permutationPhrases.begin(), permutationPhrases.end());
            do
            {
                std::cout << count << " ";
                for (auto word : permutationPhrases)
                {
                    std::cout << word << " ";
                }
                std::cout << std::endl;
                ++count;
            } while (std::next_permutation(permutationPhrases.begin(), permutationPhrases.end()));
            size_t k = 0;
            for (;;)
            {
                if ((availableIdx[k] + 1) % permWords.at(phrase[k]).size() != 0)
                {
                    availableIdx[k] += 1;
                    break;
                }
                else
                {
                    availableIdx[k] = 0;
                    if (k < availableIdx.size())
                    {
                        ++k;
                        if (k == availableIdx.size()) {
                            break;
                        }
                    }
                }
            } 
        } while (!(
            all_of(availableIdx.begin(),
                availableIdx.end(),
                [&](int i)
                {return i == availableIdx[0]; })));
        std::cout << std::endl;
    }
    input.close();
    return 0;
}





